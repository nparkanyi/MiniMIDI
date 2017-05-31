/*  MiniMIDI: A simple, lightweight, crossplatform MIDI editor.
 *  Copyright (C) 2016 Nicholas Parkanyi
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <vector>
#include <sstream>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Viewport.h"
#include "MIDI.h"
#include "MIDILoader.h"
#include "libmidi/libmidi.h"

MIDILoader::MIDILoader(std::string filename, Viewport* view)
                      : filename(filename), view(view)
{
    int r = MIDIFile_load(&midi_file, filename.c_str());
    switch (r) {
        case MIDIError::FILE_IO_ERROR:
            throw LibmidiError(std::string("Failed to open file!"));
        case MIDIError::FILE_INVALID:
            throw LibmidiError(std::string("Invalid MIDI file!"));
        case MIDIError::MEMORY_ERROR:
            throw LibmidiError(std::string("Memory allocation error!"));
    }
}

MIDILoader::~MIDILoader()
{
    MIDIFile_delete(&midi_file);
}


std::mutex t_err_mutex;
int t_err = MIDIError::SUCCESS;

void MIDILoader::load()
{
    std::vector<std::thread> workers(midi_file.header.num_tracks);
    for (int i = 0; i < midi_file.header.num_tracks; i++) {
        view->getMIDIData()->newTrack();
    }
    for (int i = 0; i < midi_file.header.num_tracks; i++){
        workers[i] = std::thread(&MIDILoader::loadTrack, this,
                                 view->getMIDIData()->getTrack(i), i);
    }

    for (auto &t : workers) {
        t.join();
    }

    {
        std::lock_guard<std::mutex> lk(t_err_mutex);
        switch (t_err) {
        case MIDIError::FILE_INVALID:
            t_err = MIDIError::SUCCESS;
            throw LibmidiError(std::string("Invalid MIDI file in worker thread!"));
        case MIDIError::FILE_IO_ERROR:
            t_err = MIDIError::SUCCESS;
            throw LibmidiError(std::string("File IO error in worker thread!"));
        case MIDIError::MEMORY_ERROR:
            t_err = MIDIError::SUCCESS;
            throw LibmidiError(std::string("Memory allocation error in worker thread!"));
        }
    }
}

void deleteLibmidiTrack(MIDITrack* trk)
{
    MIDITrack_delete_events(trk);
    delete trk;
}

void MIDILoader::loadTrack(Track* midi_data_track, int tracknum)
{
    MIDIFile new_midi; //each thread needs its own separate MIDIFile handler
    //locally reopen the same file
    int r = MIDIFile_load(&new_midi, filename.c_str());
    if (r != MIDIError::SUCCESS) {
        std::lock_guard<std::mutex> lk(t_err_mutex);
        t_err = r;
        std::fclose(new_midi.file);
        return;
    }

    //skip to the correct track
    for (int i = 0; i < tracknum; i++) {
        r = MIDITrack_skip(new_midi.file);
        if (r != MIDIError::SUCCESS) {
            std::lock_guard<std::mutex> lk(t_err_mutex);
            t_err = r;
            std::fclose(new_midi.file);
            return;
        }
    }

    //now load the actual track
    uint32_t conversion = MIDIHeader_getTempoConversion(&new_midi.header, 500000);
    MIDITrack track_;
    track_.list = nullptr;
    std::shared_ptr<MIDITrack> track(&track_, MIDITrack_delete_events);

    r = MIDITrack_load(track.get(), new_midi.file);
    if (r != MIDIError::SUCCESS) {
        std::lock_guard<std::mutex> lk(t_err_mutex);
        t_err = r;
        std::fclose(new_midi.file);
        return;
    }

    MIDIEventIterator iter = MIDIEventList_get_start_iter(track->list);
    MIDIEvent* ev = MIDIEventList_get_event(iter);
    unsigned long time = 0;
    std::vector<NoteOn*> note_ons(128); //store NoteOns so we can update their durations
                                   // when NoteOff is encountered.
    for (int i = 0; i < 128; i++){
      note_ons[i] = nullptr;
    }

    while (ev->type != META_END_TRACK){
        time += (ev->delta_time * conversion);
        //noteOn with non-zero velocity
        if (ev->type == EV_NOTE_ON && static_cast<MIDIChannelEventData*>(ev->data)->param2){
            NoteOn* tmp = new NoteOn(view, midi_data_track, time / 1000,
                                     static_cast<MIDIChannelEventData*>(ev->data)->channel,
                                     static_cast<MIDIChannelEventData*>(ev->data)->param1,
                                     static_cast<MIDIChannelEventData*>(ev->data)->param2,
                                     0);
            note_ons[static_cast<MIDIChannelEventData*>(ev->data)->param1] = tmp;
            midi_data_track->appendEvent(std::shared_ptr<Event>(tmp));
        //NoteOffs
        } else if (ev->type == EV_NOTE_ON || ev->type == EV_NOTE_OFF){
            short channel = static_cast<MIDIChannelEventData*>(ev->data)->channel;
            short value = static_cast<MIDIChannelEventData*>(ev->data)->param1;
            midi_data_track->appendEvent(
                    std::shared_ptr<Event>(new NoteOff(view, midi_data_track, time / 1000,
                                                       channel, value)));

            if (note_ons[value]){
              note_ons[value]->setDuration(time / 1000 - note_ons[value]->getTime());
              note_ons[value] = nullptr;
            }
        } else if (ev->type == EV_PROGRAM_CHANGE){
            short channel = static_cast<MIDIChannelEventData*>(ev->data)->channel;
            short voice = static_cast<MIDIChannelEventData*>(ev->data)->param1;
            midi_data_track->appendEvent(
                    std::shared_ptr<Event>(new ProgramChange(view, midi_data_track, time / 1000,
                                                             channel, voice)));
        } else if (ev->type == META_TEMPO_CHANGE){
            conversion = MIDIHeader_getTempoConversion(&new_midi.header, *(uint32_t*)(ev->data));
        }

        iter = MIDIEventList_next_event(iter);
        ev = MIDIEventList_get_event(iter);
    }

    MIDIFile_delete(&new_midi);
}

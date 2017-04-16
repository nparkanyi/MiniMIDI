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
#include <sstream>
#include <memory>
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

void MIDILoader::load()
{
    for (int i = 0; i < midi_file.header.num_tracks; i++){
        view->getMIDIData()->newTrack();
        loadTrack(view->getMIDIData()->getTrack(i));
    }
}

void deleteLibmidiTrack(MIDITrack* trk)
{
    MIDITrack_delete_events(trk);
    delete trk;
}

void MIDILoader::loadTrack(Track* midi_data_track)
{
    uint32_t conversion = MIDIHeader_getTempoConversion(&midi_file.header, 500000);
    MIDITrack track_;
    std::shared_ptr<MIDITrack> track(&track_, MIDITrack_delete_events);

    int r = MIDITrack_load(track.get(), midi_file.file);
    switch (r){
        case MIDIError::FILE_INVALID:
            throw LibmidiError(std::string("Invalid track data!"));
        case MIDIError::FILE_IO_ERROR:
            throw LibmidiError(std::string("File IO error!"));
        case MIDIError::MEMORY_ERROR:
            throw LibmidiError(std::string("Failed to allocate memory!"));
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
            conversion = MIDIHeader_getTempoConversion(&midi_file.header, *(uint32_t*)(ev->data));
        }

        iter = MIDIEventList_next_event(iter);
        ev = MIDIEventList_get_event(iter);
    }
}

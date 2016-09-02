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
#include "MIDI.h"
#include "MIDILoader.h"
#include "libmidi/libmidi.h"

MIDILoader::MIDILoader(std::string filename, MIDIData* midi_data)
                      : filename(filename), midi_data(midi_data)
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
        midi_data->newTrack();
        loadTrack(midi_data->getTrack(i));
    }
}

void deleteLibmidiTrack(MIDITrack* trk)
{
    MIDITrack_delete_events(trk);
    delete trk;
}

void MIDILoader::loadTrack(Track* midi_data_track)
{
    float conversion = MIDIHeader_getTempoConversion(&midi_file.header, 500000);
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
}

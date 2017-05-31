#ifndef MIDILOADER_H
#define MIDILOADER_H
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
#include <string>
#include <exception>
#include "libmidi/libmidi.h"

class Viewport;
class Track;

class MIDILoader {
public:
    MIDILoader(std::string filename, Viewport* view);
    ~MIDILoader();

    void load();
    void write();

    class LibmidiError : public std::exception {
    public:
        LibmidiError(std::string error) : error(error) {}
        virtual const char* what() const noexcept { return error.c_str(); }
    private:
        std::string error;
    };

private:
    void loadTrack(Track* midi_data_track, int tracknum);

    std::string filename;
    Viewport* view;
    MIDIFile midi_file;
    MIDITrack track;

};
#endif /* MIDILOADER_H */

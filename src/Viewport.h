#ifndef VIEWPORT_H
#define VIEWPORT_H
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
#ifndef DEFAULT_SF2
#define DEFAULT_SF2 "soundfonts/GeneralUser_GS_v1.47.sf2"
#endif
#ifndef DEFAULT_DRIVER
#define DEFAULT_DRIVER "dsound"
#endif

#include <array>
#include <Fl/Fl.H>
#include <Fl/Fl_Box.H>
#include "MIDI.h"
#include "NoteEditor.h"


class Keyboard {
public:
    Keyboard(int x, int y, int w, int h, Viewport* view);

    void setKey(short key, bool value, int r, int g, int b);
    //releases all keys
    void clear();
    void draw() const;
    void move(int x, int y);
    void resize(int w, int h);

private:
    Viewport* view;
    std::array<bool, 88> key_states;
    std::array<int, 264> key_colours; //stored as { r, g, b, r, g, b, ...}
    int x, y, w, h;
    int key_width;
    int n; //due to truncation of key_width, there is a gap at the right of keyboard,
           //so every nth white note gets extra pixel to fill it.
};


class Viewport : public Fl_Box {
public:
    Viewport(int x, int y, int w, int h);

    Keyboard* getKeyboard();
    NoteEditor* getEditor();
    Playback* getPlayback();
    MIDIData* getMIDIData();
    virtual void draw();
    virtual void resize(int x, int y, int w, int h);
    virtual int handle(int event);

    static void cbEveryFrame(void* v);

private:
    Keyboard keyboard;
    NoteEditor editor;
    MIDIData data;
    Playback play;
};

#endif /* VIEWPORT_H */

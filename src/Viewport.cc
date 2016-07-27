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
#include <cmath>
#include <cstdio>
#include <memory>
#include <Fl/fl_draw.H>
#include <Fl/fl_ask.H>
#include <Fl/Fl_Preferences.H>
#include "Viewport.h"

Keyboard::Keyboard(int x, int y, int w, int h, Viewport* view) : x(x), y(y), w(w), h(h), view(view)
{
    key_width = w / 52; //key_width is the width of a white key
    n = std::ceil(2.0f / (static_cast<float>(w) / 52.0f - key_width));
    key_states.fill(false);
}

void Keyboard::setKey(short key, bool value)
{
    //keyboard contains midi values 21 through 108
    if (key >= 21 && key <= 108){
        key_states[key - 21] = value;
    }
}

void Keyboard::clear()
{
    key_states.fill(false);
}

void Keyboard::draw() const
{
    int key_width = this->key_width;
    int offset = 0; //only increments after each white note, black keys drawn
                    //relative to previous white note.
    int whites = 0; //counts white keys for making nth white key wider
    int black_width = (2 * key_width) / 3;
    int black_height = (2 * h) / 3;
    Fl_Color colour;

    fl_rectf(x, y, w, h, 100, 100, 100);
    for (int i = 0; i < 88; i++){
        if (key_states[i])
            colour = fl_rgb_color(200, 30, 30);
        else
            colour = fl_rgb_color(255, 255, 255);
        if (i % 12 != 1 && i % 12 != 6 && i % 12 != 11 && i % 12 != 4
            && i % 12 != 9){ //white notes
            key_width = this->key_width;
            if (whites % n == 0)
                key_width += 2;
            fl_rectf(x + offset + 1, y, key_width - 2, h, colour);
            offset += key_width;
            whites++;
        }
    }

    offset = 0;
    whites = 0;
    for (int i = 0; i < 88; i++){
        key_width = this->key_width;
        colour = fl_rgb_color(200, 30, 30);
        if (i % 12 == 1 || i % 12 == 6 || i % 12 == 11){ //Bb, Eb, Ab
            fl_rectf(x + offset - key_width / 3, y, black_width, black_height,
                     0, 0, 0);
            if (!key_states[i])
                colour = fl_rgb_color(0, 0, 0);
            fl_rectf(2 + x + offset - key_width / 3, 2 + y, black_width - 4, black_height - 4,
                     colour);
        } else if (i % 12 == 4 || i % 12 == 9){ //Db and Gb
            fl_rectf(x + offset - key_width * 3 / 8, y, black_width, black_height,
                     0, 0, 0);
            if (!key_states[i])
                colour = fl_rgb_color(0, 0, 0);
            fl_rectf(2 + x + offset - key_width * 3 / 8, y, black_width - 4, black_height - 4,
                     colour);
        } else {
            if (whites % n == 0)
                key_width += 2;
            offset += key_width;
            whites++;
        }
    }
}

void Keyboard::move(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Keyboard::resize(int w, int h)
{
    this->w = w;
    this->h = h;
    key_width = w / 52;
    n = std::ceil(2.0f / (static_cast<float>(w) / 52.0f - key_width));
}

Viewport::Viewport(int x, int y, int w, int h)
                   : Fl_Box(FL_EMBOSSED_FRAME, x, y, w, h, ""),
                     keyboard(x, y + 3 * h / 4, w, h / 4, this), editor(x, y, w, 3 * h / 4, this),
                     data(this), play(this)
{
    std::shared_ptr<Fl_Preferences> prefs(new Fl_Preferences(Fl_Preferences::USER,
                                                             "MiniMIDI", "MiniMIDI"));
    char* sf2;

    prefs->get("soundfont", sf2, DEFAULT_SF2);
    try {
        play.getSynth()->load(DEFAULT_DRIVER, std::string(sf2));
    } catch (std::exception &e){
        fl_alert(e.what());
    }
    data.fillTrack();
    Fl::add_timeout(0.001, Viewport::cbEveryFrame, this);
}

Keyboard* Viewport::getKeyboard()
{
    return &keyboard;
}

NoteEditor* Viewport::getEditor()
{
    return &editor;
}

Playback* Viewport::getPlayback()
{
    return &play;
}

MIDIData* Viewport::getMIDIData()
{
    return &data;
}

void Viewport::draw()
{
    editor.draw();
    keyboard.draw();
    Fl_Box::draw();
}

void Viewport::resize(int x, int y, int w, int h)
{
    Fl_Box::resize(x, y, w, h);
    editor.move(x, y);
    editor.resize(w, 3 * h / 4);
    keyboard.move(x, y + 3 * h / 4);
    keyboard.resize(w, h / 4);
}

int Viewport::handle(int event)
{
    if (event == Fl_Event::FL_PUSH){
        editor.mouseDown(Fl::event_x(), Fl::event_y());
    }
    return Fl_Box::handle(event);
}

void Viewport::cbEveryFrame(void* v)
{
    Viewport* view = static_cast<Viewport*>(v);
    view->getPlayback()->everyFrame();
    view->redraw();
    Fl::repeat_timeout(0.001, Viewport::cbEveryFrame, v);
}

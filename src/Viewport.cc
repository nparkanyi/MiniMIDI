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
#include "Viewport.h"
#include <Fl/fl_draw.H>


Keyboard::Keyboard(int x, int y, int w, int h) : x(x), y(y), w(w), h(h)
{
  key_width = w / 52; //key_width is the width of a white key
  n = std::ceil(1.0f / (static_cast<float>(w) / 52.0f - key_width));
}


void Keyboard::draw(std::array<bool, 88> &key_states) const
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
        key_width++;
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
        key_width++;
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
  n = std::ceil(1.0f / (static_cast<float>(w) / 52.0f - key_width));
}


void Keyboard::draw() const
{
  std::array<bool, 88> keys;
  keys.fill(false);
  draw(keys);
}


Viewport::Viewport(int x, int y, int w, int h) : Fl_Box(FL_EMBOSSED_FRAME,
                                                        x, y, w, h, ""),
                                                 keyboard(x, y + 3 * h / 4,
                                                          w, h / 4)
{
}


void Viewport::draw()
{
  fl_rectf(x(), y(), w(), h(), 0, 0, 0);

  fl_color(fl_rgb_color(255, 0, 0));
  for (int i = x(); i < x() + w(); i+= 20){
    fl_line(i, y(), i, y() + h() - 1);
  }
  keyboard.draw();
  Fl_Box::draw();
}


void Viewport::resize(int x, int y, int w, int h)
{
  keyboard.move(x, y + 3 * h / 4);
  keyboard.resize(w, h / 4);

  Fl_Box::resize(x, y, w, h);
}


int Viewport::handle(int event)
{
  return Fl_Box::handle(event);
}

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
#include "Viewport.h"
#include <Fl/fl_draw.H>

Viewport::Viewport(int x, int y, int w, int h) : Fl_Box(FL_EMBOSSED_FRAME,
                                                        x, y, w, h, "")
{
}

void Viewport::draw()
{
  fl_rectf(x(), y(), w(), h(), 0, 0, 0);

  fl_color(fl_rgb_color(255, 0, 0));
  for (int i = x(); i < x() + w(); i+= 20){
    fl_line(i, y(), i, y() + h() - 1);
  }
  Fl_Box::draw();
}

int Viewport::handle(int event)
{
  return Fl_Box::handle(event);
}

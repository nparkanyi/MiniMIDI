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
#include "AboutDialog.h"
#include <Fl/Fl_Group.H>
#include "license_text.h"
#define RESX 600
#define RESY 400

AboutDialog::AboutDialog() : Fl_Window(RESX, RESY)
{
  label("About MiniMIDI");
  Fl_Return_Button* btn = new Fl_Return_Button(RESX - 60, RESY - 40, 50, 30);
  btn->callback(cbClose, this);
  btn->label("OK");

  Fl_Text_Buffer* buf = new Fl_Text_Buffer();
  buf->text(gpl_v3);
  Fl_Text_Display* dsp = new Fl_Text_Display(10, 70, RESX - 20, RESY - 120);
  dsp->buffer(buf);
}

void AboutDialog::cbClose(Fl_Widget* w, void* v)
{
  static_cast<AboutDialog*>(v)->hide();
}

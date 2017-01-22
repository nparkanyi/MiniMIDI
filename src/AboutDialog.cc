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
#include <Fl/Fl_Box.H>
#include <Fl/Fl_Pixmap.H>
#include "license_text.h"
#include "notes_pixmap.h"
#define RESX 600
#define RESY 380

AboutDialog::AboutDialog() : Fl_Window(RESX, RESY)
{
    label("About MiniMIDI");

    Fl_Return_Button* btn = new Fl_Return_Button(RESX - 60, RESY - 40, 50, 30);
    btn->callback(cbClose, this);
    btn->label("OK");

    Fl_Pixmap* pixmap = new Fl_Pixmap(notes_pixmap);
    Fl_Box* logo = new Fl_Box(FL_UP_BOX, RESX / 2 - 50, 10, 100, 80, "MiniMIDI");
    logo->image(pixmap);
    logo->labeltype(FL_NORMAL_LABEL);
    logo->labelsize(18);

    Fl_Box* version = new Fl_Box(FL_NO_BOX, RESX / 2 - 100, 100, 200, 20, VERSION_STRING);
    version->align(FL_ALIGN_CENTER);

    Fl_Box* copyright = new Fl_Box(FL_NO_BOX, RESX / 2 - 100, 120, 200, 20, "Copyright © 2016 Nicholas Parkanyi");
    copyright->align(FL_ALIGN_CENTER);

    Fl_Text_Buffer* buf = new Fl_Text_Buffer();
    buf->text(gpl_v3);
    Fl_Text_Display* dsp = new Fl_Text_Display(10, 150, RESX - 20, RESY - 200);
    dsp->buffer(buf);
}

void AboutDialog::cbClose(Fl_Widget* w, void* v)
{
    static_cast<AboutDialog*>(v)->hide();
}

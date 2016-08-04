#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H
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
#define MINIMIDI_VERSION "dev build"
#include <Fl/Fl_Window.H>
#include <Fl/Fl_Text_Display.H>
#include <Fl/Fl_Return_Button.H>

class AboutDialog : public Fl_Window {
public:
    AboutDialog();

    //v is a pointer to AboutDialog
    static void cbClose(Fl_Widget* w, void* v);
};

#endif /* ABOUTDIALOG_H */

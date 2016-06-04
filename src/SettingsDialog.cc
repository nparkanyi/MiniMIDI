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
#include <Fl/Fl.H>
#include "SettingsDialog.h"
#include <Fl/Fl_Choice.H>
#include <Fl/Fl_Menu_Item.H>
#include <Fl/Fl_Return_Button.H>
#define RESX 700
#define RESY 100

SettingsDialog::SettingsDialog() : Fl_Window(RESX, RESY)
{
  label("Settings");

  Fl_Menu_Item schemes[] = {{ "Gtk+", 0, 0, 0},
                         { "Plastic", 0, 0, 0},
                         { "Gleam", 0, 0, 0},
                         { "Standard", 0, 0, 0},
                         { 0 }};
  Fl_Choice* scheme_choice = new Fl_Choice(150, 10, 70, 20, "Widget Scheme:");
  scheme_choice->copy(schemes);
  scheme_choice->callback(cbChangeScheme);

  Fl_Return_Button* btn = new Fl_Return_Button(RESX - 60, RESY - 40, 50, 30);
  btn->callback(cbClose, this);
  btn->label("OK");
}

void SettingsDialog::cbChangeScheme(Fl_Widget* w, void *v)
{
  int choice = static_cast<Fl_Choice*>(w)->value();

  switch(choice){
    case 0:
      Fl::scheme("gtk+");
      break;
    case 1:
      Fl::scheme("plastic");
      break;
    case 2:
      Fl::scheme("gleam");
      break;
    case 3:
      Fl::scheme("standard");
      break;
  }
}

void SettingsDialog::cbClose(Fl_Widget* w, void* v)
{
  static_cast<SettingsDialog*>(v)->hide();
}

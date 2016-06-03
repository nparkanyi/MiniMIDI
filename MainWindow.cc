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
#include <Fl/Fl_Menu_Bar.H>
#include "MainWindow.h"

#define RES_X 1024
#define RES_Y 768

MainWindow::MainWindow() : Fl_Window(RES_X, RES_Y)
{
  Fl_Menu_Item items[] = { { "&File", 0, 0, 0, FL_SUBMENU},
                           { "&Quit", 0, 0, 0},
                           { 0 },
                           { "&Help", 0, 0, 0, FL_SUBMENU},
                           { "Manual", 0, 0, 0},
                           { 0 },
                           { 0 } };
  menu = new Fl_Menu_Bar(0, 0, RES_X, 30);
  menu->copy(items);
}

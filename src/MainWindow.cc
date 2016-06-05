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
#include <Fl/Fl_Pixmap.H>
#include <Fl/Fl_Image.H>
#include "MainWindow.h"
#include "notes_pixmap.h"

#define RES_X 1024
#define RES_Y 600

MainWindow::MainWindow() : Fl_Window(RES_X, RES_Y)
{
  label("MiniMIDI");

  Fl_Pixmap* px = new Fl_Pixmap(notes_pixmap);
  Fl_RGB_Image* icon_image = new Fl_RGB_Image(px);
  icon(icon_image);

  about_dialog = new AboutDialog();
  begin();

  settings_dialog = new SettingsDialog();
  begin();

  Fl_Menu_Item items[] = { { "&File", 0, 0, 0, FL_SUBMENU},
                           { "&Quit", FL_COMMAND + 'q', cbQuit, this},
                           { 0 },
                           { "&Edit", 0, 0, 0, FL_SUBMENU},
                           { "&Settings", 0, cbSettings, this},
                           { 0 },
                           { "&Help", 0, 0, 0, FL_SUBMENU},
                           { "&Manual", 0, 0, 0},
                           { "&About", 0, cbAbout, this},
                           { 0 },
                           { 0 } };
  menu = new Fl_Menu_Bar(0, 0, RES_X, 30);
  menu->copy(items);

  end();
}

void MainWindow::cbAbout(Fl_Widget* w, void* v)
{
  static_cast<MainWindow*>(v)->about_dialog->show();
}

void MainWindow::cbSettings(Fl_Widget* w, void* v)
{
  static_cast<MainWindow*>(v)->settings_dialog->show();
}

void MainWindow::cbQuit(Fl_Widget* w, void* v)
{
  static_cast<MainWindow*>(v)->hide();
}

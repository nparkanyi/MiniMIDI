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
#include <Fl/Fl_Button.H>
#include "MainWindow.h"
#include "notes_pixmap.h"

#define RES_X 1024
#define RES_Y 600


PlaybackControls::PlaybackControls(int x, int y, int w, int h, Viewport* view) :
                                    Fl_Group(x, y, w, h), view(view)
{
  resizable(NULL);
  Fl_Button* rwd = new Fl_Button(x, y, w / 3 - 5, w / 3 - 5, "@<<");
  rwd->callback(cbRwd, view);

  Fl_Button* play = new Fl_Button(x + w / 3 + 5, y, w / 3 - 5, w / 3 - 5, "@>");
  play->callback(cbPlay, view);

  Fl_Button* fwd = new Fl_Button(x + 2 * w / 3 + 10, y, w / 3 - 5, w / 3 - 5, "@>>");
  fwd->callback(cbRwd, view);
}


void PlaybackControls::cbPlay(Fl_Widget* w, void* v)
{
  static bool playing = false;
  Fl_Button* play = static_cast<Fl_Button*>(w);

  if (!playing){
    playing = true;
    play->label("@||");
  } else {
    playing = false;
    play->label("@>");
  }
}


void PlaybackControls::cbRwd(Fl_Widget* w, void* v)
{
}


void PlaybackControls::cbFwd(Fl_Widget* w, void* v)
{
}


MainWindow::MainWindow() : Fl_Window(RES_X, RES_Y)
{
  label("MiniMIDI");
  size_range(920, 600);

  Fl_Pixmap px(notes_pixmap);
  Fl_RGB_Image icon_image(&px);
  icon(&icon_image);

  view = new Viewport(10, 40, RES_X - 20, RES_Y - 100);
  resizable(view);

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

  //padding to keep the buttons centred
  Fl_Group* left_pad = new Fl_Group(0, RES_Y - 50, RES_X / 2, 40);
  left_pad->resizable(new Fl_Box(0, RES_Y - 50, RES_X / 2 - 70, 40));

  PlaybackControls* playback = new PlaybackControls(RES_X / 2 - 70, RES_Y - 50,
                                                    140, 40, view);
  playback->end();

  Fl_Group* right_pad = new Fl_Group(RES_X / 2, RES_Y - 50, RES_X / 2, 40);
  right_pad->end();

  end();
}

void MainWindow::quit()
{
  about_dialog->hide();
  settings_dialog->hide();
  hide();
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
  static_cast<MainWindow*>(v)->quit();
}

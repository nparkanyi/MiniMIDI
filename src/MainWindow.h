#ifndef MAINWINDOW_H
#define MAINWINDOW_H
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
#include <Fl/Fl_Window.H>
#include "Viewport.h"
#include "AboutDialog.h"
#include "SettingsDialog.h"

class Fl_Menu_Bar;

class PlaybackControls : public Fl_Group {
public:
  PlaybackControls(int x, int y, Viewport *view);
  virtual void resize(int x, int y, int w, int h);

private:
  Viewport* view;

  static void cbPlay(Fl_Widget* w, void* v);
  static void cbRwd(Fl_Widget* w, void* v);
  static void cbFwd(Fl_Widget* w, void* v);
};


class MainWindow : public Fl_Window {
public:
  MainWindow();
  void quit(); //closes all child windows before quitting
  virtual void resize(int x, int y, int w, int h);

  //v pointer to the MainWindow
  static void cbAbout(Fl_Widget* w, void* v);
  static void cbSettings(Fl_Widget* w, void* v);
  static void cbQuit(Fl_Widget* w, void* v);

private:
  Fl_Menu_Bar* menu;
  Viewport* view;
  PlaybackControls* controls;
  AboutDialog* about_dialog;
  SettingsDialog* settings_dialog;
};

#endif

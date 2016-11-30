#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
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
#include <string>
#include <Fl/Fl_Window.H>
#include <Fl/Fl_Choice.H>
#include <Fl/Fl_Box.H>
#include <Fl/Fl_Native_File_Chooser.H>

class Viewport;

class SettingsDialog : public Fl_Window
{
public:
    SettingsDialog(Viewport* view);

    static void cbClose(Fl_Widget* w, void* v);
    static void cbChangeColour(Fl_Widget* w, void* v);
    static void cbChangeScheme(Fl_Widget* w, void* v);
    static void cbFileChooser(Fl_Widget* w, void* v);

private:
    //these return the dropdown index of the current widget and colour schemes
    int schemeIndex();
    int colourIndex();
    void updateSF2Filename();

    Viewport* view;
    Fl_Box* sf2_filename;
    std::string file;
    Fl_Native_File_Chooser chooser;
};

#endif /* SETTINGSDIALOG_H */

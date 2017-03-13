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
#include <memory>
#include <Fl/Fl.H>
#include <Fl/Fl_Preferences.H>
#include "MainWindow.h"

void loadPrefs()
{
    std::shared_ptr<Fl_Preferences> prefs(new Fl_Preferences(Fl_Preferences::USER,
            "MiniMIDI", "MiniMIDI"));
    char* scheme;
    prefs->get("scheme", scheme, "gtk+");
    Fl::scheme(scheme);

    int r, g, b;
    prefs->get("bg_r", r, 192); prefs->get("bg_g", g, 192); prefs->get("bg_b", b, 192);
    Fl::background(r, g, b);
    prefs->get("bg2_r", r, 255); prefs->get("bg2_g", g, 255); prefs->get("bg2_b", b, 255);
    Fl::background2(r, g, b);
    prefs->get("fg_r", r, 0); prefs->get("fg_g", g, 0); prefs->get("fg_b", b, 0);
    Fl::foreground(r, g, b);
}


int main(int argc, char** argv)
{
    loadPrefs();

    MainWindow* win = new MainWindow();
    win->end();
    win->show(argc, argv);
    return Fl::run();
}

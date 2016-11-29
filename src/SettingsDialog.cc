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
#include <cstring>
#include <exception>
#include <memory>
#include <Fl/Fl.H>
#include "SettingsDialog.h"
#include "Viewport.h"
#include "Synth.h"
#include <Fl/Fl_Choice.H>
#include <Fl/Fl_Menu_Item.H>
#include <Fl/Fl_Return_Button.H>
#include <Fl/Fl_Preferences.H>
#include <Fl/fl_ask.H>
#define RESX 700
#define RESY 130

SettingsDialog::SettingsDialog(Viewport* view) : Fl_Window(RESX, RESY), view(view)
{
    label("Settings");

    Fl_Menu_Item schemes[] = {{ "Gtk+", 0, 0, 0},
                         { "Plastic", 0, 0, 0},
                         { "Gleam", 0, 0, 0},
                         { "Standard", 0, 0, 0},
                         { 0 }};
    Fl_Choice* scheme_choice = new Fl_Choice(150, 10, 100, 30, "Widget Scheme:");
    scheme_choice->copy(schemes);
    scheme_choice->callback(cbChangeScheme);
    scheme_choice->value(schemeIndex());

    Fl_Menu_Item colours[] = {{ "Light", 0, 0, 0},
                         { "Medium", 0, 0, 0},
                         { "Dark", 0, 0, 0},
                         { "UNIX Wizard", 0, 0, 0},
			 { "XP", 0, 0, 0},
                         { 0 }};
    Fl_Choice* colour_choice = new Fl_Choice(410, 10, 100, 30, "Colour Scheme:");
    colour_choice->copy(colours);
    colour_choice->callback(cbChangeColour);
    colour_choice->value(colourIndex());

    Fl_Return_Button* btn = new Fl_Return_Button(RESX - 60, RESY - 40, 50, 30);
    btn->callback(cbClose, this);
    btn->label("OK");

    sf2_filename = new Fl_Box(41, 50, 400, 30);
    sf2_filename->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    updateSF2Filename();

    Fl_Button* open_chooser = new Fl_Button(450, 50, 100, 30);
    open_chooser->callback(cbFileChooser, this);
    open_chooser->label("Choose");

    chooser = new Fl_File_Chooser("./", "SF2 Files (*.sf2)",
                                  Fl_File_Chooser::SINGLE,
                                  "Choose soundfont");
    chooser->preview(0);
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
            Fl::scheme("none");
            break;
    }
    Fl::redraw();
}

void SettingsDialog::cbChangeColour(Fl_Widget* w, void* v)
{
    int choice = static_cast<Fl_Choice*>(w)->value();

    switch(choice){
        case 0: //Light
            Fl::background(242, 241, 240);
            Fl::background2(255, 255, 255);
            Fl::foreground(0, 0, 0);
            break;
        case 1: //Medium
            Fl::background(192, 192, 192);
            Fl::background2(255, 255, 255);
            Fl::foreground(0, 0, 0);
            break;
        case 2: //Dark
            Fl::background(74, 72, 66);
            Fl::background2(90, 90, 90);
            Fl::foreground(223, 219, 210);
            break;
        case 3: //UNIX Wizard
            Fl::background(0, 0, 0);
            Fl::background2(30, 30, 30);
            Fl::foreground(0, 255, 0);
            break;
	case 4: //XP
	    Fl::background(235, 234, 217);
	    Fl::background2(255, 255, 255);
	    Fl::foreground(0, 0, 0);
	    break;
    }
    Fl::redraw();
}

void SettingsDialog::cbClose(Fl_Widget* w, void* v)
{
    std::shared_ptr<Fl_Preferences> prefs(new Fl_Preferences(Fl_Preferences::USER,
                                                             "MiniMIDI", "MiniMIDI"));
    SettingsDialog* diag = static_cast<SettingsDialog*>(v);
    const char* widget_scheme = Fl::scheme();

    if (widget_scheme){
        prefs->set("scheme", widget_scheme);
    } else {
        prefs->set("scheme", "none");
    }

    unsigned char r, g, b;
    Fl::get_color(FL_BACKGROUND_COLOR, r, g, b);
    prefs->set("bg_r", r);
    prefs->set("bg_g", g);
    prefs->set("bg_b", b);
    Fl::get_color(FL_BACKGROUND2_COLOR, r, g, b);
    prefs->set("bg2_r", r);
    prefs->set("bg2_g", g);
    prefs->set("bg2_b", b);
    Fl::get_color(FL_FOREGROUND_COLOR, r, g, b);
    prefs->set("fg_r", r);
    prefs->set("fg_g", g);
    prefs->set("fg_b", b);

    prefs->set("soundfont", diag->view->getPlayback()->getSynth()->getSF().c_str());
    prefs->flush();

    diag->hide();
}

void SettingsDialog::cbFileChooser(Fl_Widget* w, void* v)
{
    SettingsDialog* diag = static_cast<SettingsDialog*>(v);
    Synth* synth = diag->view->getPlayback()->getSynth();
    std::string driver = synth->getDriver();

    if (driver == std::string("")){
        driver = std::string(DEFAULT_DRIVER);
    }

    diag->chooser->show();
    while (diag->chooser->shown()){
        Fl::wait();
    }
    if (diag->chooser->value() != NULL){
        try {
            synth->reload(driver, std::string(diag->chooser->value()));
        } catch (std::exception &e){
            fl_alert(e.what());
        }
        diag->updateSF2Filename();
    }
}

int SettingsDialog::schemeIndex()
{
    const char* scheme = Fl::scheme();

    if (!scheme){
        return 3;
    } else if (std::strcmp("gtk+", scheme) == 0){
        return 0;
    } else if (std::strcmp("plastic", scheme) == 0){
        return 1;
    } else if (std::strcmp("gleam", scheme) == 0){
        return 2;
    } else {
        return 0;
    }
}

int SettingsDialog::colourIndex()
{
    unsigned char r, g, b;

    Fl::get_color(FL_BACKGROUND_COLOR, r, g, b);

    switch(r){ //identify colour scheme from r value
        case 242: //Light
            return 0;
            break;
        case 192: //Medium
            return 1;
            break;
        case 74: //Dark
            return 2;
            break;
        case 0: //UNIX Wizard
            return 3;
            break;
	default: //XP
	    return 4;
	    break;
    }
}

void SettingsDialog::updateSF2Filename()
{
    file = view->getPlayback()->getSynth()->getSF();
    if (file.size() > 40){
        file = std::string("Soundfont: ...") + file.substr(file.size() - 38, 37);
    } else {
        file = std::string("Soundfont: ") + file;
    }
    sf2_filename->label(file.c_str());
}

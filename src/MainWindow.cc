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
#include <Fl/Fl_File_Chooser.H>
#include "MainWindow.h"
#include "MIDILoader.h"
#include "notes_pixmap.h"

#define RES_X 1024
#define RES_Y 600


PlaybackControls::PlaybackControls(int x, int y, Viewport* view) :
                                    Fl_Group(x, y, 140, 40), view(view)
{
    resizable(NULL);
    Fl_Button* rwd = new Fl_Button(x, y, 40, 40, "@<<");
    rwd->callback(cbRwd, view);

    Fl_Button* play = new Fl_Button(x + 50, y, 40, 40, "@>");
    play->callback(cbPlay, view);

    Fl_Button* fwd = new Fl_Button(x + 100, y, 40, 40, "@>>");
    fwd->callback(cbRwd, view);
}

void PlaybackControls::resize(int x, int y, int w, int h)
{
    //ignore w and h, prevent widget from being resized
    //otherwise, it gets shrunk with the window and the buttons
    //fall outside the dimensions, so the user can't click them
    Fl_Group::resize(x, y, 140, 40);
}

void PlaybackControls::cbPlay(Fl_Widget* w, void* v)
{
    static bool playing = false;
    Fl_Button* play = static_cast<Fl_Button*>(w);
    Viewport* view = static_cast<Viewport*>(v);

    if (!playing){
        playing = true;
        play->label("@||");
        view->getPlayback()->play();
    } else {
        playing = false;
        play->label("@>");
        view->getPlayback()->pause();
    }
}


void PlaybackControls::cbRwd(Fl_Widget* w, void* v)
{
    Viewport* view = static_cast<Viewport*>(v);
    view->getPlayback()->seek(0);
}


void PlaybackControls::cbFwd(Fl_Widget* w, void* v)
{
}


MainWindow::MainWindow() : Fl_Double_Window(RES_X, RES_Y)
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

    settings_dialog = new SettingsDialog(view);
    begin();

    Fl_Menu_Item items[] = { { "&File", 0, 0, 0, FL_SUBMENU},
                           { "&Open MIDI", FL_COMMAND + 'o', cbOpenMIDIFile, this},
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

    controls = new PlaybackControls(RES_X / 2 - 70, RES_Y - 50, view);
    controls->end();

    midi_chooser = new Fl_File_Chooser("./", "MIDI Files (*.mid)", Fl_File_Chooser::SINGLE,
                                       "Choose MIDI file");
    midi_chooser->preview(0);
}

void MainWindow::quit()
{
    about_dialog->hide();
    settings_dialog->hide();
    hide();
}


void MainWindow::resize(int x, int y, int w, int h)
{
    Fl_Double_Window::resize(x, y, w, h);
    //keep the playback controls centred
    controls->position(w / 2 - 70, controls->y());
}


void MainWindow::cbAbout(Fl_Widget* w, void* v)
{
    static_cast<MainWindow*>(v)->about_dialog->show();
}


void MainWindow::cbSettings(Fl_Widget* w, void* v)
{
    static_cast<MainWindow*>(v)->settings_dialog->show();
}


void MainWindow::cbOpenMIDIFile(Fl_Widget* w, void* v)
{
    MainWindow* mw = static_cast<MainWindow*>(v);

    mw->midi_chooser->show();
    while (mw->midi_chooser->shown()){
        Fl::wait();
    }
    if (mw->midi_chooser->value() != NULL){
        try {
            //load midi file
            MIDILoader loader(std::string(mw->midi_chooser->value()),
                              mw->view);
            loader.load();
        } catch (std::exception &e){
            fl_alert(e.what());
        }
    }
}

void MainWindow::cbQuit(Fl_Widget* w, void* v)
{
    static_cast<MainWindow*>(v)->quit();
}


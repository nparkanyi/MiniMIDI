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
#ifndef NOTEEDITOR_H
#define NOTEEDITOR_H
#include <memory>

class Viewport;
class Fl_Widget;
class Fl_Scrollbar;
class Fl_Slider;
class Event;

class NoteEditor {
public:
    NoteEditor(int x, int y, int w, int h, Viewport* view);

    void draw() const;
    void move(int x, int y);
    void resize(int w, int h);

    //report mouse events
    void mouseDown(int mouse_x, int mouse_y);
    void mouseDrag(int mouse_x, int mouse_y);
    void mouseRelease(int mouse_x, int mouse_y);
    void rightRelease(int mouse_x, int mouse_y);

    //sets the thickness of the drawn black notes
    void setThickness(int thickness);
    //sets the number of milliseconds per pixel
    void setMsPerPixel(int ms);
    int getMsPerPixel() const;
    //returns absolute y position of this note on the NoteEditor
    void getNotePos(int note_value, unsigned long time, int &x, int &y) const;
    //returns the thickness of this note
    int getNoteThickness(int note_value) const;

    //data should be set to view
    static void cbSeeker(Fl_Widget* w, void* data);
    static void cbScroll(Fl_Widget* w, void* data);

private:
    bool isBlackNote(int note_value) const;
    void drawNotes() const;
    void drawNoteName(int note, int x, int y) const;
    //get the MIDI note value of the note at this y value
    int noteFromPos(int pos_y) const;

    int x, y, w, h;
    Viewport* view;
    Fl_Scrollbar* scroll_vert;
    Fl_Slider* seeker;
    int note_thickness;
    int ms_per_pixel;

    std::shared_ptr<Event> drag_note;
};

#endif // NOTEEDITOR_H

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
#include <iostream>
#include <sstream>
#include <Fl/fl_draw.H>
#include <Fl/Fl_Scrollbar.H>
#include <Fl/Fl_Slider.H>
#include "NoteEditor.h"
#include "Viewport.h"
#include <Fl/fl_ask.H>

#define BAROFFSET 300
#define SCROLLWIDTH 20
#define SEEKERHEIGHT 20

NoteEditor::NoteEditor(int x, int y, int w, int h, Viewport* view) : x(x), y(y), w(w), h(h),
                       view(view), note_thickness(10), ms_per_pixel(10)
{
    scroll_vert = new Fl_Scrollbar(x + w - SCROLLWIDTH - 2, y + 1, SCROLLWIDTH, h - 2);
    scroll_vert->value(40, 30, 0, 127);
    scroll_vert->linesize(2);
    scroll_vert->callback(cbScroll, view);

    seeker = new Fl_Slider(x + 2, y + h - SEEKERHEIGHT, w - SCROLLWIDTH - 4, SEEKERHEIGHT);
    seeker->type(FL_HOR_NICE_SLIDER);
    seeker->callback(cbSeeker, view);
}

void NoteEditor::draw() const
{
    int line_y = 0;
    int start_note = scroll_vert->value();

    fl_push_clip(x, y, w, h);
    fl_rectf(x, y, w, h, 0, 0, 0);
    fl_color(150, 150, 150);

    //draw the grey lines separating the notes
    for (int i = start_note; i <= 127; i++){
        if (i % 12 == 1 || i % 12 == 3 || i % 12 == 6 || i % 12 == 8 || i % 12 == 10){
            fl_line(x, y + line_y, x + w, y + line_y);
            line_y += note_thickness;
        } else {
            fl_line(x, y + line_y, x + w, y + line_y);
            line_y += note_thickness + 4;
        }
        drawNoteName(i, x + 4, y + line_y - 2);
        fl_color(150, 150, 150);
    }

    drawNotes();
    fl_color(0, 50, 200);
    fl_line(x + BAROFFSET, y, x + BAROFFSET, y + h);
    scroll_vert->redraw();

    //update seeker value and range as necessary
    seeker->range(0.0, static_cast<double>(view->getMIDIData()->getTrack(0)->getDuration()));
    seeker->value(view->getPlayback()->getTime());
    seeker->redraw();

    fl_pop_clip();
}

void NoteEditor::move(int x, int y)
{
    this->x = x;
    this->y = y;
    scroll_vert->resize(x + w - SCROLLWIDTH - 2, y + 1, SCROLLWIDTH, h - 2);
}

void NoteEditor::resize(int w, int h)
{
    this->w = w;
    this->h = h;
    scroll_vert->resize(x + w - SCROLLWIDTH - 2, y + 1, SCROLLWIDTH, h - 2);
}

void NoteEditor::mouseDown(int mouse_x, int mouse_y)
{
    long time = getMsPerPixel() * (mouse_x - x - BAROFFSET) + static_cast<signed long>(view->getPlayback()->getTime());
    drag_note.reset(new NoteOn(view, time, noteFromPos(mouse_y), 100, 20));
    if (time > 0){
        view->getMIDIData()->getTrack(0)->addEvent(drag_note);
    }
}

void NoteEditor::mouseDrag(int mouse_x, int mouse_y)
{
    long time = getMsPerPixel() * (mouse_x - x - BAROFFSET) + static_cast<signed long>(view->getPlayback()->getTime());
    long start_time = drag_note->getTime();
    if (time < start_time) time = 0;
    drag_note->setDuration(time - start_time);
    view->redraw();
}

void NoteEditor::mouseRelease(int mouse_x, int mouse_y)
{
    if (drag_note){
        long time = getMsPerPixel() *  (mouse_x - x - BAROFFSET) + static_cast<signed long>(view->getPlayback()->getTime());
        std::shared_ptr<Event> ev(new NoteOff(view, time, static_cast<NoteOn*>(drag_note.get())->getValue()));
        if (time > drag_note->getTime() + 10){
            view->getMIDIData()->getTrack(0)->addEvent(ev);
        } else {
            //user tried to drag left of note start; invalid, so we remove the NoteOn added earlier
            view->getMIDIData()->getTrack(0)->removeEvent(drag_note);
        }
        drag_note.reset();
    }
}

void NoteEditor::rightRelease(int mouse_x, int mouse_y)
{
    long time = getMsPerPixel() * (mouse_x - x - BAROFFSET) + static_cast<signed long>(view->getPlayback()->getTime());
    int value = noteFromPos(mouse_y);
    if (time > 0){
        view->getMIDIData()->getTrack(0)->removeNotesAt(time, value);
    }
    view->redraw();
}

void NoteEditor::setThickness(int thickness)
{
    note_thickness = thickness;
    view->redraw();
}

void NoteEditor::setMsPerPixel(int ms)
{
    ms_per_pixel = ms;
}

int NoteEditor::getMsPerPixel() const
{
    return ms_per_pixel;
}

void NoteEditor::getNotePos(int note_value, unsigned long time, int &x, int &y) const
{
    int start_note = scroll_vert->value();
    x = this->x + ((signed long)time - (signed long)view->getPlayback()->getTime()) / ms_per_pixel + 300;

    if (note_value < start_note){
        y = this->y - 20; //outside clip area, so invisible
        return;
    }
    y = this->y;
    for (int i = start_note; i < note_value; i++) {
        if (isBlackNote(i)) {
            y += note_thickness;
        } else {
            y += note_thickness + 4;
        }
    }
}

int NoteEditor::getNoteThickness(int note_value) const
{
    if (isBlackNote(note_value)){
        return note_thickness;
    } else {
        return note_thickness + 4;
    }
}

void NoteEditor::cbSeeker(Fl_Widget* w, void* v)
{
    Fl_Slider* seeker = static_cast<Fl_Slider*>(w);
    Viewport* view = static_cast<Viewport*>(v);

    view->getPlayback()->seek(static_cast<unsigned long>(seeker->value()));
}

void NoteEditor::cbScroll(Fl_Widget* w, void* v)
{
    static_cast<Viewport*>(v)->redraw();
}

bool NoteEditor::isBlackNote(int i) const
{
    return (i % 12 == 1 || i % 12 == 3 || i % 12 == 6 || i % 12 == 8 || i % 12 == 10);
}

void NoteEditor::drawNotes() const
{
    long draw_from = view->getPlayback()->getTime() - BAROFFSET * ms_per_pixel;
    int num_tracks = view->getMIDIData()->numTracks();
    int num_events;
    Track* track;
    char r, g, b;

    for (int i = 0; i < num_tracks; i++){
        track = view->getMIDIData()->getTrack(i);
        num_events = track->numEvents();
        track->getColour(r, g, b);
        fl_color(r, g, b);

        for (int idx = 0; idx < num_events; idx++){
            //stop when when the notes are off-screen
            if ((signed long)(track->getEvent(idx)->getTime()) >
                   draw_from + this->w * ms_per_pixel){
                break;
            }
            if ((signed long)(track->getEvent(idx)->getTime()) >= draw_from ||
                    (signed long)(track->getEvent(idx)->getTime()) +
                    track->getEvent(idx)->getDuration() >= draw_from){
                track->getEvent(idx)->draw();
            }
        }
    }
}

void NoteEditor::drawNoteName(int note, int x, int y) const
{
    int value = note % 12;
    fl_color(50, 230, 50);
    fl_font(FL_COURIER|FL_BOLD|FL_ITALIC, 15);

    switch(value){
        case 0:
            if (note == 60){ //middle C
                fl_color(200, 50, 50);
            } else {
                fl_color(0, 255, 255); //every other C is cyan
            }
            fl_draw("C", x, y);
            break;
        case 2:
            fl_draw("D", x, y);
            break;
        case 4:
            fl_draw("E", x, y);
            break;
        case 5:
            fl_draw("F", x, y);
            break;
        case 7:
            fl_draw("G", x, y);
            break;
        case 9:
            fl_draw("A", x, y);
            break;
        case 11:
            fl_draw("B", x, y);
            break;
        default:
            break;
    }
}

int NoteEditor::noteFromPos(int pos_y) const
{
    int px = y;
    for (int note = scroll_vert->value(); note <= 127; note++){
        if (isBlackNote(note)){
            px += note_thickness;
        } else {
            px += note_thickness + 4;
        }

        if (pos_y <= px){
            return note;
        }
    }
    return -1;
}

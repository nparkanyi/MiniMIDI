#include <Fl/fl_draw.H>
#include "NoteEditor.h"
#include "Viewport.h"

NoteEditor::NoteEditor(int x, int y, int w, int h, Viewport* view) : x(x), y(y), w(w), h(h),
                       view(view), start_note(75), note_thickness(10), ms_per_pixel(10)
{}

void NoteEditor::draw() const
{
    int line_y = 0;

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
    }

    drawNotes();
    fl_color(0, 50, 200);
    fl_line(x + 300, y, x + 300, y + h);
    fl_pop_clip();
}

void NoteEditor::move(int x, int y)
{
    this->x = x;
    this->y = y;
}

void NoteEditor::resize(int w, int h)
{
    this->w = w;
    this->h = h;
}

void NoteEditor::scroll(int note_value)
{
    start_note = note_value;
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
    x = this->x + (time - view->getPlayback()->getTime()) / 10 + 300;

    y = this->y;
    for (int i = 0; i < note_value; i++) {
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

bool NoteEditor::isBlackNote(int i) const
{
    return (i % 12 == 1 || i % 12 == 3 || i % 12 == 6 || i % 12 == 8 || i % 12 == 10);
}

void NoteEditor::drawNotes() const
{
    long draw_from = view->getPlayback()->getTime() - 300 * ms_per_pixel;
    int num_tracks = view->getMIDIData()->numTracks();
    int num_events;
    Track* track;
    char r, g, b;

    for (int i = 0; i < num_tracks; i++){
        track = view->getMIDIData()->getTrack(i);
        num_events = track->numEvents();
        track->getColour(r, g, b);
        fl_color(r, g, b);

        for (int idx = track->getEventAt(draw_from); idx < num_events; idx++){
            //stop when when the notes are off-screen
            if (track->getEvent(idx)->getTime() > draw_from + this->w * ms_per_pixel){
                break;
            }
            track->getEvent(idx)->draw();
        }
    }
}

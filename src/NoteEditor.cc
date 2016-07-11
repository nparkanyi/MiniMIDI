#include <Fl/fl_draw.H>
#include "NoteEditor.h"

NoteEditor::NoteEditor(int x, int y, int w, int h, Viewport* view) : x(x), y(y), w(w), h(h),
                       view(view), start_note(75), note_thickness(10)
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

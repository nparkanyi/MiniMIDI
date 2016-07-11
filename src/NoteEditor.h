#ifndef NOTEEDITOR_H
#define NOTEEDITOR_H

class Viewport;

class NoteEditor {
public:
    NoteEditor(int x, int y, int w, int h, Viewport* view);

    void draw() const;
    void move(int x, int y);
    void resize(int w, int h);
    //sets the top note drawn on screen, by its MIDI value (0-127).
    void scroll(int note_value);
    //sets the thickness of the drawn black notes
    void setThickness(int thickness);
    //returns absolute y position of this note on the NoteEditor
    int getNotePos(int note_value) const;

private:
    int x, y, w, h;
    Viewport* view;
    int start_note;
    int note_thickness;
};

#endif // NOTEEDITOR_H

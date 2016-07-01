#ifdef MIDI_H
#define MIDI_H
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
#include <vector>
#include <string>
#include <memory>
#include <map>

class MIDIPlayback;
class NoteEditor;

class Event {
public:
    Event(MIDIPlayback* play, NoteEditor* editor, std::string type, unsigned long time)
          : play(play), editor(editor), type(type), time(time) {}
    virtual ~Event(){}
    
    std::string getType() const { return type; }
    unsigned long getTime() const { return time; }
    //executed when we reach this event during playback
    virtual void run() = 0;
    //
    virtual void draw() = 0;

private:
    MIDIPlayback* play;
    NoteEditor* editor;
    std::string type;
    //absolute time the event occurs, in ms
    unsigned long time;
}

class Note : public Event {
public:
    Note(MIDIPlayback* play, NoteEditor* editor, short value, int duration);
    virtual ~Note();
    
    short getValue() const;
    int getDuration() const;
    virtual void run();
    virtual void draw();
    
private:
    short value;
    int duration;
}

class Track {
public:
    Track();
        
    //returns the total duration of this track in ms
    unsigned long getDuration() const;
    void addEvent(std::shared_ptr<Event> ev);
    void removeEvent(std::shared_ptr<Event> ev);
    //returns vector of events occuring at this time
    std::vector<std::shared_ptr<Event>>* getEventsAt(unsigned long time) const;
    //this track's notes will be drawn in this colour on the NoteEditor
    void setColour(char r, char g, char b);
    void getColour(char &r, char &g, char &b) const;
    
private:
    std::vector<std::shared_ptr<Event>> events;
    char r, g, b;
}

class Playback {
public:
    Playback(MIDI* parent);
    
    //current playback time
    unsigned long getTime() const;
    void seek(unsigned long time);
    void pause();
    void play();
    
private:
    MIDI* parent;
    unsigned long start_time;
    bool playing;
}

#endif /* MIDI_H */
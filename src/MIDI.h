#ifndef MIDI_H
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
#include "Synth.h"

class Playback;
class NoteEditor;
class MIDI;

class Event {
public:
    Event(Playback* play, NoteEditor* editor, std::string type, unsigned long time)
          : play(play), editor(editor), type(type), time(time) {}
    virtual ~Event(){}

    std::string getType() const { return type; }
    unsigned long getTime() const { return time; }
    //executed when we reach this event during playback
    virtual void run() = 0;
    //
    virtual void draw() = 0;

protected:
    Playback* play;
    NoteEditor* editor;

private:
    std::string type;
    //absolute time the event occurs, in ms
    unsigned long time;
};

class NoteOn : public Event {
public:
    NoteOn(Playback* play, NoteEditor* editor, unsigned long time, short value,
         short velocity, int duration);

    short getValue() const;
    int getDuration() const;
    virtual void run();
    virtual void draw();

private:
    short value;
    short velocity;
    //time until associated noteOff event, stored to simplify drawing
    int duration;
};

class NoteOff : public Event {
public:
    NoteOff(Playback* play, NoteEditor* editor, unsigned long time, short value);

    short getValue() const;
    virtual void run();
    virtual void draw();

private:
    short value;
};

class Track {
public:
    Track();

    //returns the total duration of this track in ms
    unsigned long getDuration() const;
    void addEvent(std::shared_ptr<Event> ev);
    void removeEvent(std::shared_ptr<Event> ev);
    //returns vector of events occurring at this time
    std::vector<std::shared_ptr<Event>>& getEventsAt(unsigned long time) const;
    //this track's NoteOns will be drawn in this colour on the NoteOnEditor
    void setColour(char r, char g, char b);
    void getColour(char &r, char &g, char &b) const;

private:
    std::vector<std::shared_ptr<Event>> events;
    char r, g, b;
};

class Playback {
public:
    Playback(MIDI* instance, Synth* synth);

    //current playback time
    unsigned long getTime() const;
    Synth* getSynth() const;
    void seek(unsigned long time);
    void pause();
    void play();

private:
    MIDI* instance;
    Synth* synth;
    unsigned long start_time;
    bool playing;
};

class MIDI {
public:
    MIDI();
};

#endif /* MIDI_H */

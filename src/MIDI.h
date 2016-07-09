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
#include <chrono>
#include "Synth.h"

class Viewport;
class Playback;
class NoteEditor;
class MIDIData;

class Event {
public:
    Event(Viewport* view, std::string type, unsigned long time)
          : view(view), type(type), time(time) {}
    virtual ~Event(){}

    std::string getType() const { return type; }
    unsigned long getTime() const { return time; }
    //executed when we reach this event during playback
    virtual void run() = 0;
    //executed when the note is to be drawn on the note editor
    virtual void draw() = 0;

protected:
    Viewport* view;

private:
    std::string type;
    //absolute time the event occurs, in ms
    unsigned long time;
};

class NoteOn : public Event {
public:
    NoteOn(Viewport* view, unsigned long time, short value,
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
    NoteOff(Viewport* view, unsigned long time, short value);

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
    int numEvents() const;
    std::shared_ptr<Event> getEvent(int index) const;
    //returns vector of events occurring at this time
    std::vector<std::shared_ptr<Event>> getEventsAt(unsigned long time) const;

    //this track's NoteOns will be drawn in this colour on the NoteOnEditor
    void setColour(char r, char g, char b);
    void getColour(char &r, char &g, char &b) const;

private:
    Viewport* view;
    std::vector<std::shared_ptr<Event>> events;
    char r, g, b;
    int index;
};

class Playback {
public:
    Playback(Viewport* view);

    //current playback time
    unsigned long getTime() const;
    Synth* getSynth();
    void seek(unsigned long time);
    void pause();
    void play();
    //method called every frame, this actually plays notes
    void everyFrame();

private:
    Viewport* view;
    Synth synth;
    std::chrono::steady_clock::time_point start_time;
    //for storing the time when we pause
    unsigned long time_elapsed;
    bool playing;
    std::vector<int> track_indices;
};

class MIDIData {
public:
    MIDIData(Viewport* view);

    int numTracks() const;
    Track* getTrack(int index);
    void newTrack();
    void fillTrack();

private:
    Viewport* view;
    std::vector<Track> tracks;
    std::string filename;
};

#endif /* MIDI_H */

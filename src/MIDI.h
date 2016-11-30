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
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include "Synth.h"

class Viewport;
class Playback;
class NoteEditor;
class Track;
class MIDIData;

class Event {
public:
    Event(Viewport* view, Track* track, std::string type, unsigned long time)
          : view(view), track(track), type(type), time(time) {}
    virtual ~Event(){}

    std::string getType() const { return type; }
    unsigned long getTime() const { return time; }
    virtual int getDuration() const { return 0; }
    virtual void setDuration(int duration) { return; }
    //executed when we reach this event during playback
    virtual void run() = 0;
    //executed when the note is to be drawn on the note editor
    virtual void draw() = 0;

protected:
    Viewport* view;
    Track* track;

private:
    std::string type;
    //absolute time the event occurs, in ms
    unsigned long time;
};

class ChannelEvent : public Event {
public:
    ChannelEvent(Viewport* view, Track* track, std::string type,
                 unsigned long time, short channel)
                 : Event(view, track, type, time), channel(channel) {}
    virtual ~ChannelEvent(){}

    short getChannel() const { return channel; }

private:
    short channel;
};

class NoteOn : public ChannelEvent {
public:
    NoteOn(Viewport* view, Track* track, unsigned long time, short channel, short value,
         short velocity, int duration);

    short getValue() const;
    virtual int getDuration() const;
    virtual void setDuration(int duration);
    virtual void run();
    virtual void draw();

private:
    short channel;
    short value;
    short velocity;
    //time until associated noteOff event, stored to simplify drawing
    int duration;
};

class NoteOff : public ChannelEvent {
public:
    NoteOff(Viewport* view, Track *track, unsigned long time, short channel, short value);

    short getValue() const;
    virtual void run();
    virtual void draw();

private:
    short value;
};

class ProgramChange : public ChannelEvent {
public:
    ProgramChange(Viewport* view, Track *track, unsigned long time, short channel, short voice);

    short getVoice() const;
    virtual void run();
    virtual void draw();

private:
    short voice;
};

class Track {
public:
    Track();

    //returns the total duration of this track in ms
    unsigned long getDuration() const;
    void addEvent(std::shared_ptr<Event> ev);
    void removeEvent(std::shared_ptr<Event> ev);
    //removes the NoteOn and NoteOff events of any note of this value occurring at time
    void removeNotesAt(unsigned long time, int value);
    int numEvents() const;
    std::shared_ptr<Event> getEvent(int index) const;
    //returns index of first event occurring at or after this time, or -1
    //if there are no such events
    int getEventAt(long time) const;

    //this track's NoteOns will be drawn in this colour on the NoteOnEditor
    void setColour(char r, char g, char b);
    void getColour(char &r, char &g, char &b) const;

private:
    std::vector<std::shared_ptr<Event>> events;
    char r, g, b;
};

class Playback {
public:
    Playback(Viewport* view);

    //current playback time
    unsigned long getTime() const;
    bool isPlaying() const { return playing; }
    Synth* getSynth();
    void seek(unsigned long time);
    void pause();
    void play();
    //adds new indices if necessary (i.e. if we added a track), leaves existing indices unmodified
    void updateIndices();
    //method called every frame, this actually plays notes
    void everyFrame();
    std::string getTimeString() const;

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
    void clear();

private:
    Viewport* view;
    std::vector<Track> tracks;
    std::string filename;
};

#endif /* MIDI_H */

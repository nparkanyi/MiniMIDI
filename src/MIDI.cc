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
#include <Fl/fl_draw.H>
#include "MIDI.h"
#include "Viewport.h"

NoteOn::NoteOn(Viewport* view, unsigned long time, short value,
           short velocity, int duration)
           : Event(view, "NoteOn", time), value(value), velocity(velocity),
             duration(duration)
{}

short NoteOn::getValue() const
{
    return value;
}

int NoteOn::getDuration() const
{
    return duration;
}

void NoteOn::run()
{
    view->getPlayback()->getSynth()->noteOn(value, velocity);
    view->getKeyboard()->setKey(value, true);
    view->redraw();
}

void NoteOn::draw()
{
    int x, y;
    NoteEditor* editor = view->getEditor();
    int w = duration / editor->getMsPerPixel();
    int h = editor->getNoteThickness(value);

    editor->getNotePos(value, getTime(), x, y);
    fl_rectf(x, y + 1, w, h - 1);
}

NoteOff::NoteOff(Viewport* view, unsigned long time, short value)
                 : Event(view, "NoteOff", time), value(value)
{
}

short NoteOff::getValue() const
{
    return value;
}

void NoteOff::run()
{
    view->getPlayback()->getSynth()->noteOff(value);
    view->getKeyboard()->setKey(value, false);
    view->redraw();
}

void NoteOff::draw()
{}

Track::Track() : r(255), g(255), b(255)
{}

unsigned long Track::getDuration() const
{
    int n = events.size();
    if (n == 0){
        return 0;
    } else {
        return events[n - 1]->getTime();
    }
}

void Track::addEvent(std::shared_ptr<Event> ev)
{
    //index of first event occurring at the same time or after the event
    //we are inserting
    int idx = getEventAt(ev->getTime());
    if (idx > 0){
        events.insert(events.begin() + idx, ev);
    } else {
        events.push_back(ev);
    }
}

void Track::removeEvent(std::shared_ptr<Event> ev)
{
    int size = events.size();
    for (int i = 0; i < size; i++){
        if (events[i] == ev){
            events.erase(events.begin() + i);
            break;
        }
    }
}

int Track::numEvents() const
{
    return events.size();
}

std::shared_ptr<Event> Track::getEvent(int index) const
{
    return events[index];
}

int Track::getEventAt(long time) const
{
    if (time < 0){
        return 0;
    }

    int size = events.size();
    int i = size / 2;
    int jump = i / 2;
    if (jump == 0) { jump = 1; }

    //if no event occurs before the given time, return -1
    if (events.empty() || events[size - 1]->getTime() < time){
        return -1;
    }
    //find first event that occurs at or after the given time
    while (i > 0 && i < size - 1 &&
           !(events[i-1]->getTime() < time && events[i]->getTime() >= time)){
        jump /= 2;
        if (jump == 0) { jump = 1; }
        if (events[i]->getTime() < time){
            i += jump;
        } else {
            i -= jump;
        }
    }

    return i;
}

void Track::setColour(char r, char g, char b)
{
    this->r = r;
    this->g = g;
    this->b = g;
}

void Track::getColour(char &r, char &g, char &b) const
{
    r = this->r;
    g = this->g;
    b = this->b;
}

Playback::Playback(Viewport* view) : view(view), time_elapsed(0), playing(false)
{}

unsigned long Playback::getTime() const
{
    if (playing){
        return std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::steady_clock::now() - start_time).count();
    } else {
        return time_elapsed;
    }
}

Synth* Playback::getSynth()
{
    return &synth;
}

void Playback::seek(unsigned long time)
{
    MIDIData* data = view->getMIDIData();
    Track* track;
    int num_tracks = data->numTracks();

    time_elapsed = time;
    updateIndices();
    for (int i = 0; i < num_tracks; i++){
        track = data->getTrack(i);
        track_indices[i] = track->getEventAt(time);
    }
    view->getKeyboard()->clear();
    view->redraw();
    synth.clear();

    if (playing)
        play();
}

void Playback::pause()
{
    time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>
                 (std::chrono::steady_clock::now() - start_time).count();
    playing = false;
}

void Playback::play()
{
    MIDIData* data = view->getMIDIData();
    std::chrono::milliseconds ms(time_elapsed);
    start_time = std::chrono::steady_clock::now() - ms;
    time_elapsed = 0;
    for (int i = 0; i < data->numTracks(); i++){
        updateIndices();
    }
    playing = true;
}

void Playback::updateIndices()
{
    int new_tracks = view->getMIDIData()->numTracks() - track_indices.size();
    for (int i = 0; i < new_tracks; i++){
        track_indices.push_back(0);
    }
}

void Playback::everyFrame()
{
    MIDIData* data = view->getMIDIData();
    int num_events;
    int num_tracks = data->numTracks();
    Track* track;
    if (playing){
        for (int i = 0; i < num_tracks; i++){
            track = data->getTrack(i);
            num_events = track->numEvents();
            while (track_indices[i] < num_events &&
                   track->getEvent(track_indices[i])->getTime() <= getTime()){
                track->getEvent(track_indices[i])->run();
                track_indices[i]++;
            }
        }
    }
}

MIDIData::MIDIData(Viewport* view) : view(view), filename("")
{}

void MIDIData::fillTrack()
{
    newTrack();
    tracks[0].addEvent(std::shared_ptr<Event>(new NoteOn(view, 0, 60, 100, 500)));
    tracks[0].addEvent(std::shared_ptr<Event>(new NoteOff(view, 500, 60)));
    tracks[0].addEvent(std::shared_ptr<Event>(new NoteOn(view, 501, 59, 100, 1500)));
    tracks[0].addEvent(std::shared_ptr<Event>(new NoteOff(view, 2001, 59)));
    tracks[0].addEvent(std::shared_ptr<Event>(new NoteOn(view, 2500, 57, 100, 3000)));
    tracks[0].addEvent(std::shared_ptr<Event>(new NoteOn(view, 3000, 30, 100, 3000)));
    tracks[0].addEvent(std::shared_ptr<Event>(new NoteOn(view, 3000, 100, 100, 3000)));
    tracks[0].addEvent(std::shared_ptr<Event>(new NoteOff(view, 5500, 57)));
    tracks[0].addEvent(std::shared_ptr<Event>(new NoteOff(view, 6000, 100)));
    tracks[0].addEvent(std::shared_ptr<Event>(new NoteOff(view, 6000, 30)));
    tracks[0].setColour(200, 0, 50);
}

int MIDIData::numTracks() const
{
    return tracks.size();
}

Track* MIDIData::getTrack(int index)
{
    return &tracks[index];
}

void MIDIData::newTrack()
{
    tracks.push_back(Track());
}

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
#include "MIDI.h"

NoteOn::NoteOn(Playback* play, NoteEditor* editor, unsigned long time, short value,
           short velocity, int duration)
           : Event(play, editor, "NoteOn", time), value(value), velocity(velocity),
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
    //(play->getSynth())->noteOn(value, velocity);
}

void NoteOn::draw()
{

}

NoteOff::NoteOff(Playback* play, NoteEditor* editor, unsigned long time, short value)
                 : Event(play, editor, "NoteOff", time), value(value)
{
}

short NoteOff::getValue() const
{
    return value;
}

void NoteOff::run()
{
    //(play->getSynth())->noteOff(value);
}

void NoteOff::draw()
{}

Track::Track() : r(255), g(255), b(255)
{}

unsigned long Track::getDuration() const
{
    int n = events.size();
    return events[n - 1]->getTime();
}

void Track::addEvent(std::shared_ptr<Event> ev)
{
    events.push_back(ev);
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

std::vector<std::shared_ptr<Event>> Track::getEventsAt(unsigned long time) const
{
    std::vector<std::shared_ptr<Event>> found;
    int size = events.size();
    int i = size / 2;
    int jump = i / 2;
    if (jump == 0) { jump = 1; }

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

    //from there, add events that occur at time, if any
    for ( ; i < size; i++){
        if (events[i]->getTime() > time){
            break;
        }
        found.push_back(events[i]);
    }
    return found;
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

Playback::Playback(std::string driver, std::string sf_file)
                   : synth(driver, sf_file), time_elapsed(0),
                     playing(false)
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

const Synth* Playback::getSynth() const
{
    return &synth;
}

void Playback::seek(unsigned long time)
{
    time_elapsed = time;
}

void Playback::pause()
{
    time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>
                 (std::chrono::steady_clock::now() - start_time).count();
    playing = false;
}

void Playback::play()
{
    std::chrono::milliseconds ms(time_elapsed);
    start_time = std::chrono::steady_clock::now() - ms;
    time_elapsed = 0;
    playing = true;
}

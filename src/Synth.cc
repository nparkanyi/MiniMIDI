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
#include "Synth.h"

Synth::Synth(std::string driver, std::string sf_file) : driver(driver), sf_file(sf_file)
{
    is_initialized = false;
    reload(driver, sf_file);
}

Synth::~Synth()
{
    fluid_synth_sfunload(synth, sf_handle, 0);
    delete_fluid_audio_driver(adriver);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
}

void Synth::reload(std::string driver, std::string sf_file)
{
    settings = new_fluid_settings();
    synth = new_fluid_synth(settings);
    fluid_settings_setstr(settings, "audio.driver", driver.c_str());
    adriver = new_fluid_audio_driver(settings, synth);
    sf_handle = fluid_synth_sfload(synth, sf_file.c_str(), 1);

    is_initialized = true;
}

std::string Synth::getDriver()
{
    return driver;
}

std::string Synth::getSF()
{
    return sf_file;
}

void Synth::noteOn(short value, int velocity)
{
    fluid_synth_noteon(synth, 0, value, velocity);
}

void Synth::noteOff(short value)
{
    fluid_synth_noteoff(synth, 0, value);
}

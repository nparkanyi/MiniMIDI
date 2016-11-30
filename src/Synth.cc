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

Synth::Synth() : is_initialized(false)
{}

Synth::~Synth()
{
    fluid_synth_sfunload(synth.get(), sf_handle, 0);
}

void Synth::load(std::string driver, std::string sf_file)
{
    settings.reset(new_fluid_settings(), delete_fluid_settings);
    synth.reset(new_fluid_synth(settings.get()), delete_fluid_synth);
    if (!synth){
	this->sf_file = std::string("none");
        throw FluidInitFail();
    }
    fluid_settings_setstr(settings.get(), "audio.driver", driver.c_str());
    adriver.reset(new_fluid_audio_driver(settings.get(), synth.get()), delete_fluid_audio_driver);
    if (!adriver){
	this->sf_file = std::string("none");
        throw FluidDriverFail();
    }
    sf_handle = fluid_synth_sfload(synth.get(), sf_file.c_str(), 1);
    if (sf_handle == FLUID_FAILED){
	this->sf_file = std::string("none");
        throw FluidSFFail();
    }

    this->sf_file = sf_file;
    this->driver = driver;
    is_initialized = true;
}

void Synth::reload(std::string driver, std::string sf_file)
{
    adriver.reset();
    synth.reset();
    settings.reset();
    load(driver, sf_file);
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
    fluid_synth_noteon(synth.get(), 0, value, velocity);
}

void Synth::noteOff(short value)
{
    fluid_synth_noteoff(synth.get(), 0, value);
}

void Synth::clear()
{
    for (int i = 0; i <= 127; i++){
        fluid_synth_noteoff(synth.get(), 0, i);
    }
}

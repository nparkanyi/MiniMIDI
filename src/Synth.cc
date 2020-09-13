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
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include "Synth.h"
#include <iostream>

typedef int (*PtrFluidSynthSfload)(fluid_synth_t*, const char*, int);
typedef int (*PtrFluidSynthSfunload)(fluid_synth_t*, int id, int reset_presets);
typedef fluid_settings_t* (*PtrNewFluidSettings)(void);
typedef void (*PtrDeleteFluidSettings)(fluid_settings_t*);
typedef int (*PtrFluidSettingsSetstr)(fluid_settings_t*, const char*, const char*);
typedef fluid_synth_t* (*PtrNewFluidSynth)(fluid_settings_t* settings);
typedef void (*PtrDeleteFluidSynth)(fluid_synth_t*);
typedef fluid_audio_driver_t* (*PtrNewFluidAudioDriver)(fluid_settings_t*, fluid_synth_t*);
typedef void (*PtrDeleteFluidAudioDriver)(fluid_audio_driver_t*);
typedef int (*PtrFluidSynthNoteon)(fluid_synth_t*, int, int, int);
typedef int (*PtrFluidSynthNoteoff)(fluid_synth_t*, int, int);
typedef int (*PtrFluidSynthProgramChange)(fluid_synth_t*, int, int);


//function pointers for fluidsynth calls
//(direct calls with gcc, runtime dynamic on MSVC)
PtrFluidSynthSfload __fluid_synth_sfload = nullptr;
PtrFluidSynthSfunload __fluid_synth_sfunload = nullptr;
PtrNewFluidSettings __new_fluid_settings = nullptr;
PtrDeleteFluidSettings __delete_fluid_settings = nullptr;
PtrFluidSettingsSetstr __fluid_settings_setstr = nullptr;
PtrNewFluidSynth __new_fluid_synth = nullptr;
PtrDeleteFluidSynth __delete_fluid_synth = nullptr;
PtrNewFluidAudioDriver __new_fluid_audio_driver = nullptr;
PtrDeleteFluidAudioDriver __delete_fluid_audio_driver = nullptr;
PtrFluidSynthNoteon __fluid_synth_noteon = nullptr;
PtrFluidSynthNoteoff __fluid_synth_noteoff = nullptr;
PtrFluidSynthProgramChange __fluid_synth_program_change = nullptr;

#ifdef _MSC_VER
#define FLUID_DLL "libfluidsynth-1.dll"
HINSTANCE fluidlib;
#endif
bool fluidloaded = false;

Synth::Synth() : is_initialized(false)
{
#ifdef _MSC_VER
    fluidlib = LoadLibrary(TEXT(FLUID_DLL));
    if (fluidlib) {
        fluidloaded = true;
        __fluid_synth_sfload = (PtrFluidSynthSfload)GetProcAddress(fluidlib, "fluid_synth_sfload");
        if (!__fluid_synth_sfload) fluidloaded = false;
        __fluid_synth_sfunload = (PtrFluidSynthSfunload)GetProcAddress(fluidlib, "fluid_synth_sfunload");
        if (!__fluid_synth_sfunload) fluidloaded = false;
        __new_fluid_settings = (PtrNewFluidSettings)GetProcAddress(fluidlib, "new_fluid_settings");
        if (!__new_fluid_settings) fluidloaded = false;
        __delete_fluid_settings = (PtrDeleteFluidSettings)GetProcAddress(fluidlib, "delete_fluid_settings");
        if (!__new_fluid_settings) fluidloaded = false;
        __fluid_settings_setstr = (PtrFluidSettingsSetstr)GetProcAddress(fluidlib, "fluid_settings_setstr");
        if (!__fluid_settings_setstr) fluidloaded = false;
        __new_fluid_synth = (PtrNewFluidSynth)GetProcAddress(fluidlib, "new_fluid_synth");
        if (!__new_fluid_synth) fluidloaded = false;
        __delete_fluid_synth = (PtrDeleteFluidSynth)GetProcAddress(fluidlib, "delete_fluid_synth");
        if (!__delete_fluid_synth) fluidloaded = false;
        __new_fluid_audio_driver = (PtrNewFluidAudioDriver)GetProcAddress(fluidlib, "new_fluid_audio_driver");
        if (!__new_fluid_audio_driver) fluidloaded = false;
        __delete_fluid_audio_driver = (PtrDeleteFluidAudioDriver)GetProcAddress(fluidlib, "delete_fluid_audio_driver");
        if (!__delete_fluid_audio_driver) fluidloaded = false;
        __fluid_synth_noteon = (PtrFluidSynthNoteon)GetProcAddress(fluidlib, "fluid_synth_noteon");
        if (!__fluid_synth_noteon) fluidloaded = false;
        __fluid_synth_noteoff = (PtrFluidSynthNoteoff)GetProcAddress(fluidlib, "fluid_synth_noteoff");
        if (!__fluid_synth_noteoff) fluidloaded = false;
        __fluid_synth_program_change = (PtrFluidSynthProgramChange)GetProcAddress(fluidlib, "fluid_synth_program_change");
        if (!__fluid_synth_program_change) fluidloaded = false;
    }
    else {
        MessageBox(NULL, "Failed to load " FLUID_DLL ", playback will be silent!",
            "MiniMIDI: Warning", MB_OK | MB_ICONWARNING);
    }
#else
    fluidloaded = true;
    __fluid_synth_sfload = fluid_synth_sfload;
    __fluid_synth_sfunload = fluid_synth_sfunload;
    __new_fluid_settings = new_fluid_settings;
    __delete_fluid_settings = delete_fluid_settings;
    __fluid_settings_setstr = fluid_settings_setstr;
    __new_fluid_synth = new_fluid_synth;
    __delete_fluid_synth = delete_fluid_synth;
    __new_fluid_audio_driver = new_fluid_audio_driver;
    __delete_fluid_audio_driver = delete_fluid_audio_driver;
    __fluid_synth_noteon = fluid_synth_noteon;
    __fluid_synth_noteoff = fluid_synth_noteoff;
    __fluid_synth_program_change = fluid_synth_program_change;
#endif
}

Synth::~Synth()
{
    if (fluidloaded) {
        __fluid_synth_sfunload(synth.get(), sf_handle, 0);
    }
#ifdef _MSC_VER
    if (fluidlib) {
        FreeLibrary(fluidlib);
    }
#endif
}

void Synth::load(std::string driver, std::string sf_file)
{
    if (fluidloaded) {
        settings.reset(__new_fluid_settings(), __delete_fluid_settings);
        synth.reset(__new_fluid_synth(settings.get()), __delete_fluid_synth);
        if (!synth) {
            this->sf_file = std::string("none");
            throw FluidInitFail();
        }
        __fluid_settings_setstr(settings.get(), "audio.driver", driver.c_str());
        adriver.reset(__new_fluid_audio_driver(settings.get(), synth.get()), __delete_fluid_audio_driver);
        if (!adriver) {
            this->sf_file = std::string("none");
            throw FluidDriverFail();
        }
        sf_handle = __fluid_synth_sfload(synth.get(), sf_file.c_str(), 1);
        if (sf_handle == FLUID_FAILED) {
            this->sf_file = std::string("none");
            throw FluidSFFail();
        }

        this->sf_file = sf_file;
        this->driver = driver;
        is_initialized = true;
    }
}

void Synth::reload(std::string driver, std::string sf_file)
{
    if (fluidloaded) {
        adriver.reset();
        synth.reset();
        settings.reset();
        load(driver, sf_file);
    }
}

std::string Synth::getDriver()
{
    return driver;
}

std::string Synth::getSF()
{
    return sf_file;
}

void Synth::noteOn(short channel, short value, int velocity)
{
    if (fluidloaded) {
        __fluid_synth_noteon(synth.get(), channel, value, velocity);
    }
}

void Synth::noteOff(short channel, short value)
{
    if (fluidloaded) {
        __fluid_synth_noteoff(synth.get(), channel, value);
    }
}

void Synth::programChange(short channel, short voice)
{
    if (fluidloaded){
        __fluid_synth_program_change(synth.get(), channel, voice);
    }
}

void Synth::clear()
{
    if (fluidloaded) {
        for (int i = 0; i <= 127; i++) {
            for (int j = 0; j < 16; j++) {
                __fluid_synth_noteoff(synth.get(), j, i);
            }
        }
    }
}

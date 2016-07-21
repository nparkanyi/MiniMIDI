#ifndef SYNTH_H
#define SYNTH_H
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
#include <string>
#include <exception>
#include <memory>
#include <fluidsynth.h>

class Synth {
public:
    Synth();
    ~Synth();

    //whether fluidsynth was successfully loaded, if it fails, playback will be silent
    bool initialized();
    //calling load more than once has undefined results, use reload()
    void load(std::string driver, std::string sf_file);
    void reload(std::string driver, std::string sf_file);
    std::string getDriver();
    std::string getSF();
    void noteOn(short value, int velocity);
    void noteOff(short value);
    void clear();

    class FluidInitFail : public std::exception {
    public:
        virtual const char* what() const noexcept
        {
            return "Failed to initialize FluidSynth!";
        }
    };

    class FluidDriverFail : public std::exception {
    public:
        virtual const char* what() const noexcept
        {
            return "Failed to load FluidSynth driver!";
        }
    };

    class FluidSFFail : public std::exception {
    public:
        virtual const char* what() const noexcept
        {
            return "Failed to load soundfont!";
        }
    };

private:
    bool is_initialized;
    std::string driver;
    std::string sf_file;
    std::shared_ptr<fluid_settings_t> settings;
    std::shared_ptr<fluid_synth_t> synth;
    std::shared_ptr<fluid_audio_driver_t> adriver;
    int sf_handle;
};

#endif /* SYNTH_H */

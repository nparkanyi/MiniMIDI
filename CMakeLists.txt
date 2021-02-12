cmake_minimum_required(VERSION 3.10)

project(MiniMIDI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(-DVERSION_STRING="dev build")
# TODO: actual config option
add_definitions(-DDEFAULT_DRIVER="pulseaudio")

set(MiniMIDI_SRCS
    src/AboutDialog.cc
    src/main.cc
    src/MainWindow.cc
    src/MIDI.cc
    src/MIDILoader.cc
    src/NoteEditor.cc
    src/SettingsDialog.cc
    src/Synth.cc
    src/Viewport.cc)

add_executable(MiniMIDI ${MiniMIDI_SRCS})

add_library(midi src/libmidi/libmidi.c)

find_package(FLTK MODULE REQUIRED) # Legacy-style CMake module
find_package(Threads MODULE REQUIRED)

target_include_directories(MiniMIDI PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SRC_DIR}/src/libmidi")

# TODO: need a proper CMake module for fluidsynth,
# as-is just blindly plunks '-lfluidsynth'
target_link_libraries(MiniMIDI PUBLIC
    midi
    ${FLTK_LIBRARIES}
    Threads::Threads
    fluidsynth)

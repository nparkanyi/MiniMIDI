FLAGS = --std=c++11 -g
DEFAULT_DRIVER = pulseaudio
VERSION_STRING = "dev build"
FLFLAGS = $(shell fltk-config --cxxflags) -DDEFAULT_DRIVER=\"$(DEFAULT_DRIVER)\" -DVERSION_STRING=\"$(VERSION_STRING)\"
FLSTATIC = $(shell fltk-config --ldstaticflags)
FLUID = -lfluidsynth


MiniMIDI: main.cc MainWindow.o AboutDialog.o SettingsDialog.o Viewport.o MIDI.o Synth.o NoteEditor.o libmidi.o MIDILoader.o
	${CXX} main.cc  $(FLAGS) MainWindow.o AboutDialog.o SettingsDialog.o Viewport.o MIDI.o Synth.o NoteEditor.o libmidi.o MIDILoader.o -o MiniMIDI $(FLFLAGS) $(FLSTATIC) $(FLUID)

MainWindow.o: MainWindow.h MainWindow.cc
	${CXX} MainWindow.cc $(FLAGS) -c $(FLFLAGS)

AboutDialog.o: AboutDialog.h AboutDialog.cc notes_pixmap.h license_text.h
	${CXX} AboutDialog.cc $(FLAGS) -c $(FLFLAGS)

SettingsDialog.o: SettingsDialog.h SettingsDialog.cc
	${CXX} SettingsDialog.cc $(FLAGS) -c $(FLFLAGS)

Viewport.o: Viewport.h Viewport.cc
	${CXX} Viewport.cc $(FLAGS) -c $(FLFLAGS)

MIDI.o: MIDI.h MIDI.cc
	${CXX} MIDI.cc $(FLAGS) -c $(FLFLAGS)

Synth.o: Synth.h Synth.cc
	${CXX} Synth.cc $(FLAGS) -c $(FLFLAGS)

NoteEditor.o: NoteEditor.h NoteEditor.cc
	${CXX} NoteEditor.cc $(FLAGS) -c $(FLFLAGS)

libmidi.o: libmidi/libmidi.h libmidi/libmidi.c
	${CXX} libmidi/libmidi.c -I./libmidi -c -o libmidi.o

MIDILoader.o: MIDILoader.h MIDILoader.cc
	${CXX} MIDILoader.cc $(FLAGS) -c

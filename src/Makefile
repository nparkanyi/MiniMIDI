FLAGS = --std=c++11 -g
FLFLAGS = `fltk-config --cxxflags`
FLSTATIC = `fltk-config --ldstaticflags`
FLUID = -lfluidsynth


MiniMIDI: main.cc MainWindow.o AboutDialog.o SettingsDialog.o Viewport.o MIDI.o Synth.o NoteEditor.o libmidi.o
	${CXX} main.cc  $(FLAGS) MainWindow.o AboutDialog.o SettingsDialog.o Viewport.o MIDI.o Synth.o NoteEditor.o libmidi.o -o MiniMIDI $(FLFLAGS) $(FLSTATIC) $(FLUID)

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
	
libmidi.o: libmidi/midi.h libmidi/midi.c
	${CC} libmidi/midi.c -I./libmidi -c -o libmidi.o -std=c99 -lfluidsynth

clean:
	rm MiniMIDI; rm *.o

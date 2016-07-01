FLAGS = --std=c++11 -g
FLFLAGS = `fltk-config --cxxflags`
FLSTATIC = `fltk-config --ldstaticflags`


MiniMIDI: main.cc MainWindow.o Viewport.o AboutDialog.o SettingsDialog.o
	${CXX} main.cc  $(FLAGS) MainWindow.o Viewport.o AboutDialog.o SettingsDialog.o -o MiniMIDI $(FLFLAGS) $(FLSTATIC)

MainWindow.o: MainWindow.h MainWindow.cc
	${CXX} MainWindow.cc $(FLAGS) -c $(FLFLAGS)

AboutDialog.o: AboutDialog.h AboutDialog.cc notes_pixmap.h license_text.h
	${CXX} AboutDialog.cc $(FLAGS) -c $(FLFLAGS)

SettingsDialog.o: SettingsDialog.h SettingsDialog.cc
	${CXX} SettingsDialog.cc $(FLAGS) -c $(FLFLAGS)

Viewport.o: Viewport.h Viewport.cc
	${CXX} Viewport.cc $(FLAGS) -c $(FLFLAGS)
	
midi.o: libmidi/midi.h libmidi/midi.c
	${CC} libmidi/midi.c -I./libmidi -c -std=c99 -lfluidsynth

clean:
	rm MiniMIDI; rm *.o

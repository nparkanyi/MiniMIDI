FLAGS = --std=c++11 -g
FLFLAGS = `fltk-config --cxxflags`

MiniMIDI: main.cc MainWindow.o Viewport.o AboutDialog.o SettingsDialog.o
	${CXX} main.cc  $(FLAGS) MainWindow.o Viewport.o AboutDialog.o SettingsDialog.o -o MiniMIDI $(fltk-config) `fltk-config --ldstaticflags`

MainWindow.o: MainWindow.h MainWindow.cc
	${CXX} MainWindow.cc $(FLAGS) -c $(FLFLAGS)

AboutDialog.o: AboutDialog.h AboutDialog.cc notes_pixmap.h license_text.h
	${CXX} AboutDialog.cc $(FLAGS) -c $(FLFLAGS)

SettingsDialog.o: SettingsDialog.h SettingsDialog.cc
	${CXX} SettingsDialog.cc $(FLAGS) -c $(FLFLAGS)

Viewport.o: Viewport.h Viewport.cc
	${CXX} Viewport.cc $(FLAGS) -c $(FLFLAGS)

clean:
	rm MiniMIDI; rm *.o

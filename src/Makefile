FLAGS = --std=c++11 -g

MiniMIDI: main.cc MainWindow.o Viewport.o AboutDialog.o SettingsDialog.o
	${CXX} main.cc  $(FLAGS) MainWindow.o Viewport.o AboutDialog.o SettingsDialog.o -o MiniMIDI `fltk-config --cxxflags --ldstaticflags`

MainWindow.o: MainWindow.h MainWindow.cc
	${CXX} MainWindow.cc $(FLAGS) -c `fltk-config --cxxflags --ldstaticflags`

AboutDialog.o: AboutDialog.h AboutDialog.cc notes_pixmap.h license_text.h
	${CXX} AboutDialog.cc $(FLAGS) -c `fltk-config --cxxflags --ldstaticflags`

SettingsDialog.o: SettingsDialog.h SettingsDialog.cc
	${CXX} SettingsDialog.cc $(FLAGS) -c `fltk-config --cxxflags --ldstaticflags`

Viewport.o: Viewport.h Viewport.cc
	${CXX} Viewport.cc $(FLAGS) -c `fltk-config --cxxflags --ldstaticflags`

clean:
	rm MiniMIDI; rm *.o

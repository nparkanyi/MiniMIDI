MiniMIDI: main.cc MainWindow.o AboutDialog.o SettingsDialog.o
	${CXX} main.cc MainWindow.o AboutDialog.o SettingsDialog.o -o MiniMIDI `fltk-config --cxxflags --ldstaticflags`

MainWindow.o: MainWindow.h MainWindow.cc
	${CXX} MainWindow.cc -c `fltk-config --cxxflags --ldstaticflags`

AboutDialog.o: AboutDialog.h AboutDialog.cc notes_pixmap.h license_text.h
	${CXX} AboutDialog.cc -c `fltk-config --cxxflags --ldstaticflags`

SettingsDialog.o: SettingsDialog.h SettingsDialog.cc
	${CXX} SettingsDialog.cc -c `fltk-config --cxxflags --ldstaticflags`

clean:
	rm MiniMIDI

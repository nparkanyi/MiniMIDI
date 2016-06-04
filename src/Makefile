MiniMIDI: main.cc MainWindow.o AboutDialog.o
	${CXX} main.cc MainWindow.o AboutDialog.o -o MiniMIDI `fltk-config --cxxflags --ldstaticflags`

MainWindow.o: MainWindow.h MainWindow.cc
	${CXX} MainWindow.cc -c `fltk-config --cxxflags --ldstaticflags`

AboutDialog.o: AboutDialog.h AboutDialog.cc
	${CXX} AboutDialog.cc -c `fltk-config --cxxflags --ldstaticflags`

clean:
	rm MiniMIDI

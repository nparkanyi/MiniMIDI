MiniMIDI: main.cc MainWindow.o
	${CXX} main.cc MainWindow.o -o MiniMIDI `fltk-config --cxxflags --ldflags`

MainWindow.o: MainWindow.h MainWindow.cc
	${CXX} MainWindow.cc -c `fltk-config --cxxflags --ldflags`

all: MiniMIDI

MiniMIDI: $(wildcard src/*.cc) $(wildcard src/*.h)
	cd src; make;
	cp src/MiniMIDI ./

clean:
	rm MiniMIDI*;
	rm src/*.o;
	rm src/MiniMIDI*;

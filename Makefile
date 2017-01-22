all: MiniMIDI

MiniMIDI: $(wildcard src/*.cc) $(wildcard src/*.h)
	cd src; make;
	cp src/MiniMIDI ./

.PHONY: clean

clean:
	rm MiniMIDI*;
	rm src/*.o;
	rm src/MiniMIDI*;

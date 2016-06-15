MiniMIDI: $(wildcard src/*.cc) $(wildcard src/*.h)
	cd src; make;
	cp src/MiniMIDI ./

clean:
	rm MiniMIDI;
	cd src; make clean

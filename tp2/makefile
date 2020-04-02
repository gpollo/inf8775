bin/piano: src/*.cpp src/*.hpp
	g++ -g src/*.cpp -Isrc/ --std=c++17 -Wall -o $@

remise: bin/piano
	zip -r $(MATRICULE)_tp2.zip makefile src/* bin/* songs/* cout_transition.txt tp.sh rapport.odt

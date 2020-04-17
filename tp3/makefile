build/pandemic: CMakeLists.txt source/* include/* tests/*
	mkdir -p build
	cd build && cmake .. && make

remise: build/pandemic scripts/* makefile rapport.odt data.ods tp.sh
	zip -r $(MATRICULE)_tp3.zip $? CMakeLists.txt source/* include/* tests/*

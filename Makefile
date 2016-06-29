CONFIG   =  Config.sh
PYTHON   =  /usr/bin/python
CFLAGS   =
CC       = g++
INCL	 = include/*
SRC 	 = src/Main.cpp src/Snapshot.cpp src/ReadParameterFile.cpp src/Globals.cpp src/StrFormat.cpp \
	src/Particle.cpp src/Galaxy.cpp

make:
	python make_macros.py
	$(CC) $(CFLAGS) $(SRC) -std=c++11 -Wall -O3 -lm -fopenmp -o bin/DiskStats++

clean:
	rm *~ src/*.o src/*~ include/*.o include/*~ bin/* \#* src/\#* include/\#* bin/\#* \
	python/*~ python/\#* OUTPUTS/*~ OUTPUTS/\#* OUTPUTS/*

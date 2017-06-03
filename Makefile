CONFIG   =  Config.sh
PYTHON   =  /usr/bin/python
CFLAGS   =
CC       = g++
INCL	 = include/*
SRC 	 = src/Main.cpp src/Snapshot.cpp src/ReadParameterFile.cpp src/Globals.cpp src/StrFormat.cpp \
	src/Particle.cpp src/Galaxy.cpp  src/LinAlg.cpp src/Histogram.cpp src/PlottingScripts.cpp

make:
	python make_macros.py
	$(CC) $(CFLAGS) $(SRC) -std=c++11 -Wall -O3 -lm -L/usr/lib64/libgomp.so.1 -o bin/DiskStats++
	$(CC) Scripts/merge_ics.cpp -o bin/merge_ics

clean:
	rm *~ src/*.o src/*~ include/*.o include/*~ bin/* \#* src/\#* include/\#* bin/\#* \
	python/*~ python/\#* OUTPUTS/*~ OUTPUTS/\#* OUTPUTS/* rm Plots/*

plots:
	gnuplot Plots/*.gnuplot

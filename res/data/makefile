IFLAGS=-I../../structures/include
CFILES=../../structures/src/Coord.cpp ../../structures/src/Vector2.cpp ../../structures/src/Trip.cpp

all: pkdd15-i/pkdd15-i.trips pkdd15-i/pkdd15-i.trips.bin

pkdd15-i/pkdd15-i.trips.bin: pkdd15-i/convertToBin pkdd15-i/pkdd15-i.trips
	pkdd15-i/convertToBin

pkdd15-i/convertToBin: pkdd15-i/convertToBin.cpp $(CFILES)
	g++ -Wall -std=c++17 pkdd15-i/convertToBin.cpp $(CFILES) -o $@ $(IFLAGS)

pkdd15-i/pkdd15-i.trips: pkdd15-i/process.py pkdd15-i/train.csv
	python3 pkdd15-i/process.py pkdd15-i/train.csv > $@

pkdd15-i/train.csv: pkdd15-i/train.zip
	unzip -o -d pkdd15-i $<
	touch $@

pkdd15-i/train.zip:
	curl https://dl.dropboxusercontent.com/s/7culexod1kn5xxr/train.zip --output $@

.DELETE_ON_ERROR :

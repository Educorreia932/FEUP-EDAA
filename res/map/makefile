IFLAGS=-Irapidxml -I../../structures/include -I../../utils/include
CFILES=../../structures/src/coord.cpp ../../structures/src/point.cpp ../../utils/src/getDirectory.cpp ../../utils/src/urlEncode.cpp

all: processed/AMP.nodes

processed/AMP.nodes: map.xml map_process
	./map_process processed/AMP < map.xml

map_process: map_process.cpp $(CFILES)
	g++ -Wall -std=c++17 map_process.cpp $(CFILES) -o $@ $(IFLAGS)

map.xml: map.zip
	unzip -o $<
	touch $@

map.zip:
	curl https://dl.dropboxusercontent.com/s/8hjnkr47u8f5x6v/map.zip --output $@

.DELETE_ON_ERROR :

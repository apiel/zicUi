SDL2=`sdl2-config --cflags --libs`
SDL2_ttf=`pkg-config --cflags --libs SDL2_ttf`
OSC=`pkg-config --cflags --libs liblo`

all:
	make build run

build:
	g++ -o zicUi zicUi.cpp $(SDL2) $(SDL2_ttf) $(OSC)

run:
	./zicUi
SDL2=`sdl2-config --cflags --libs`
SDL2_ttf=`pkg-config --cflags --libs SDL2_ttf`
OSC=`pkg-config --cflags --libs liblo`

ifneq ($(shell uname -m),x86_64)
RPI := -DIS_RPI=1
endif

all:
	make build run

build:
	g++ -fms-extensions -o zicUi zicUi.cpp -ldl $(SDL2) $(SDL2_ttf) $(OSC) $(RPI)

run:
	./zicUi
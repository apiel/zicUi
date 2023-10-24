SDL2=`sdl2-config --cflags --libs`
SDL2_ttf=`pkg-config --cflags --libs SDL2_ttf`

ifneq ($(shell uname -m),x86_64)
RPI := -DIS_RPI=1
endif

main:
	make build run

all:
	make libs build run

build:
	g++ -g -fms-extensions -o zicUi zicUi.cpp -ldl $(SDL2) $(SDL2_ttf) $(RPI)

run:
	./zicUi

libs:
	make -C plugins all
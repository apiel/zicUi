all:
	make clean_so OscInterface MidiEncoderInterface\
		clean_o

clean:
	make clean_so clean_o

clean_so:
	rm -f build/*.so

clean_o:
	rm -f *.o

compile:
	g++ -c -o $(LIBNAME).o plugin.cpp -fPIC -DPLUGIN_NAME=$(LIBNAME) -DPLUGIN_INCLUDE=\"$(LIBNAME).h\" $(EXTRA)
	g++ -shared -o build/libzic_$(LIBNAME).so $(LIBNAME).o $(EXTRA)

OscInterface:
	make compile LIBNAME=OscInterface EXTRA="$(shell pkg-config --cflags --libs liblo)"

MidiEncoderInterface:
	make compile LIBNAME=MidiEncoderInterface EXTRA="$(shell pkg-config --cflags --libs rtmidi)"

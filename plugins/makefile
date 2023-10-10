

all:
	make clean_so OscController MidiEncoderController\
		PadComponent EncoderComponent GranularComponent SequencerComponent ButtonComponent TriggerNoteComponent\
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

OscController:
	make compile LIBNAME=OscController EXTRA="$(shell pkg-config --cflags --libs liblo)"

MidiEncoderController:
	make compile LIBNAME=MidiEncoderController EXTRA="$(shell pkg-config --cflags --libs rtmidi)"

PadComponent:
	make compile LIBNAME=PadComponent EXTRA="-DPLUGIN_PROPS=ComponentInterface::Props"

EncoderComponent:
	make compile LIBNAME=EncoderComponent EXTRA="-DPLUGIN_PROPS=ComponentInterface::Props"

GranularComponent:
	make compile LIBNAME=GranularComponent EXTRA="-DPLUGIN_PROPS=ComponentInterface::Props"

SequencerComponent:
	make compile LIBNAME=SequencerComponent EXTRA="-DPLUGIN_PROPS=ComponentInterface::Props"

ButtonComponent:
	make compile LIBNAME=ButtonComponent EXTRA="-DPLUGIN_PROPS=ComponentInterface::Props"

TriggerNoteComponent:
	make compile LIBNAME=TriggerNoteComponent EXTRA="-DPLUGIN_PROPS=ComponentInterface::Props"

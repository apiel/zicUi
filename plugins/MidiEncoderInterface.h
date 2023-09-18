#ifndef _OSC_INTERFACE_H_
#define _OSC_INTERFACE_H_

#include <RtMidi.h>
#include <stdio.h>
#include <string.h>

#include "interface.h"

void midiHandler(double timeStamp, std::vector<unsigned char> *message, void *userData);

// For the moment everything is hardcoded, might want to make this more configurable...

class MidiEncoderInterface : public Interface
{
protected:
    RtMidiIn midi;

    void load(const char *portName)
    {
        if (midi.isPortOpen())
        {
            midi.closePort();
        }
        int port = -1;

        unsigned int portCount = midi.getPortCount();
        printf("List midi input devices:\n");
        for (unsigned int i = 0; i < portCount; i++)
        {
            printf("DEVICE=%s\n", midi.getPortName(i).c_str());
            if (midi.getPortName(i).find(portName) != std::string::npos)
            {
                port = i;
            }
        }

        if (port == -1)
        {
            printf("Midi input %s not found\n", portName);
            return;
        }

        midi.openPort(port);
        printf("Midi input loaded: %s\n", midi.getPortName(port).c_str());
    }

public:
    int values[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    MidiEncoderInterface(Props &props) : Interface(props)
    {
        midi.setCallback(midiHandler, this);
        midi.ignoreTypes(false, false, false);

        load("");
    }

    bool config(char *key, char *value)
    {
        if (strcmp(key, "DEVICE") == 0)
        {
            load(value);
            return true;
        }
        return false;
    }
};

void midiHandler(double timeStamp, std::vector<unsigned char> *message, void *userData)
{
    MidiEncoderInterface *plugin = (MidiEncoderInterface *)userData;

    int8_t direction = 1;
    if (message->at(2) < plugin->values[message->at(1)] || message->at(2) == 0)
    {
        direction = -1;
    }
    plugin->values[message->at(1)] = message->at(2);

    uint8_t id = 7 - message->at(1);

    printf("Midi encoder message: %d %d\n", id, direction);
    plugin->encoder(id, direction);
}

#endif
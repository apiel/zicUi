#ifndef _OSC_INTERFACE_H_
#define _OSC_INTERFACE_H_

#include <RtMidi.h>
#include <stdio.h>
#include <string.h>

#include "controllerInterface.h"

void midiHandler(double timeStamp, std::vector<unsigned char> *message, void *userData);

// For the moment everything is hardcoded, might want to make this more configurable...

class MidiEncoderInterface : public ControllerInterface
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
    struct Encoder
    {
        uint8_t value = 0;
        uint8_t encoderId = -1;
    } encoders[127];

    MidiEncoderInterface(Props &props) : ControllerInterface(props)
    {
        midi.setCallback(midiHandler, this);
        midi.ignoreTypes(false, false, false);

        encoders[0].encoderId = 0;
        encoders[1].encoderId = 1;
        encoders[2].encoderId = 2;
        encoders[3].encoderId = 3;
        encoders[4].encoderId = 4;
        encoders[5].encoderId = 5;
        encoders[6].encoderId = 6;
        encoders[7].encoderId = 7;

        load("");
    }

    bool config(char *key, char *value)
    {
        if (strcmp(key, "DEVICE") == 0)
        {
            load(value);
            return true;
        }
        else if (strcmp(key, "ENCODER_TARGET") == 0)
        {
            char *cc = strtok(value, " ");
            char *encoderId = strtok(NULL, " ");
            printf("ENCODER_TARGET: %s %s\n", cc, encoderId);
            encoders[atoi(cc)].encoderId = atoi(encoderId);
            return true;
        }
        return false;
    }
};

void midiHandler(double timeStamp, std::vector<unsigned char> *message, void *userData)
{
    MidiEncoderInterface *plugin = (MidiEncoderInterface *)userData;

    int8_t direction = 1;
    if (message->at(2) < plugin->encoders[message->at(1)].value || message->at(2) == 0)
    {
        direction = -1;
    }
    plugin->encoders[message->at(1)].value = message->at(2);

    // printf("Midi encoder message: %d %d\n", plugin->encoders[message->at(1)].encoderId, direction);
    plugin->encoder(plugin->encoders[message->at(1)].encoderId, direction);
}

#endif
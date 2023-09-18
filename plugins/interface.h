#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <stdint.h>
#include <unistd.h>
#include <vector>

class Interface
{
public:
    void (*midi)(std::vector<unsigned char> *message) = NULL;
    void (*encoder)(int id, int8_t direction) = NULL;

    struct Props
    {
        void (*midi)(std::vector<unsigned char> *message) = NULL;
        void (*encoder)(int id, int8_t direction) = NULL;
    };
    Interface(Props &props)
        : midi(props.midi), encoder(props.encoder)
    {
    }

    virtual bool config(char *key, char *value)
    {
        return false;
    }
};

#endif
#pragma once
#include <stdint.h>
#include "TimerZero.h"
#include "InputPin.h"
#include "ReceiveState.h"

class Receiver
{
private:
    InputPin *pin;
    TimerZero *timer;

    ReceiveState state;
    bool first;
    uint8_t interval;
    uint8_t index;
    uint8_t packetSize;
    bool *bitBuffer;

public:
    Receiver(InputPin *pin, TimerZero *timer, uint8_t packetSize) : pin(pin), timer(timer), first(false), interval(0), index(0), packetSize(packetSize)
    {
        this->setState(R_IDLE);
        this->bitBuffer = new bool(this->packetSize);
    }

    ~Receiver()
    {
        delete this->pin;
        delete this->timer;
        delete[] this->bitBuffer;
    }

    ReceiveState getState();
    void setState(ReceiveState);
    void act();
    bool *getBuffer();
    uint8_t getPacketSize();
    char getCharacter();
};

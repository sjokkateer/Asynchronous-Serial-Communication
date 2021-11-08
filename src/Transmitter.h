#pragma once
#include <stdint.h>
#include "TransmitState.h"
#include "OutputPin.h"
#include "Timer.h"

#define SIZE 8
#define LSB 0

class Transmitter
{
private:
    TransmitState state;
    OutputPin *pin;
    Timer *timer;
    uint8_t transmitBit;

    char data;

    void setState(TransmitState);
    void start();
    uint8_t bitValue();

public:
    Transmitter(OutputPin *pin, Timer *timer) : pin(pin), timer(timer), transmitBit(LSB)
    {
        this->setState(IDLE);
    }

    ~Transmitter()
    {
        delete this->pin;
        delete this->timer;
    }

    void transmit(char);
    void transmit(char *);
    bool isBusy();
    void act();
};

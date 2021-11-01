#pragma once
#include <stdint.h>
#include "TransmitState.h"
#include "OutputPin.h"

class Transmitter
{
private:
    TransmitState state;
    // Transmit pin
    OutputPin *pin;

    char data;

public:
    Transmitter(OutputPin *pin) : state(IDLE), pin(pin)
    {
        // Pull high for IDLE.
        this->pin->high();
    }

    ~Transmitter()
    {
        delete this->pin;
    }

    void high();
    void low();
    TransmitState getState();
    void setState(TransmitState);
    void transmit(char);
    void transmit(char *);
    bool isBusy();
};

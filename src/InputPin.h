#pragma once
#include <stdint.h>
#include <avr/io.h>

class InputPin
{
private:
    volatile uint8_t *port;
    uint8_t pin;

    void determineRegisters(char);

public:
    InputPin(char port, uint8_t pin) : pin(pin)
    {
        this->determineRegisters(port);
    }

    /** Returns the value of the input pin. */
    bool read();
};

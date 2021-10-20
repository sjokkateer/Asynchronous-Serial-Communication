#pragma once
#include <stdint.h>
#include <avr/io.h>

class OutputPin
{
private:
    volatile uint8_t *port;
    uint8_t pin;

    void determineRegisters(char);

public:
    OutputPin(char port, uint8_t pin) : pin(pin)
    {
        this->determineRegisters(port);
    }

    void high();
    void low();
};

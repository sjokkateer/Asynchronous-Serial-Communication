#include "OutputPin.h"

void OutputPin::determineRegisters(char port)
{
    switch (port)
    {
    case 'B':
        DDRB |= (1 << this->pin);
        this->port = &PORTB;
        break;
    case 'C':
        DDRC |= (1 << this->pin);
        this->port = &PORTC;
        break;
    case 'D':
        DDRD |= (1 << this->pin);
        this->port = &PORTD;
        break;
    default:
        break;
    }
}

void OutputPin::high()
{
    *this->port |= (1 << this->pin);
}

void OutputPin::low()
{
    *this->port &= ~(1 << this->pin);
}

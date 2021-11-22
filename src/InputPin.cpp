#include "InputPin.h"

void InputPin::determineRegisters(char port)
{
    switch (port)
    {
    case 'B':
        DDRB &= ~(1 << this->pin);
        PORTB |= (1 << this->pin);
        this->port = &PINB;
        break;
    case 'C':
        DDRC &= ~(1 << this->pin);
        PORTC |= (1 << this->pin);
        this->port = &PINC;
        break;
    case 'D':
        DDRD &= ~(1 << this->pin);
        PORTD |= (1 << this->pin);
        this->port = &PIND;
        break;
    default:
        break;
    }
}

bool InputPin::read()
{
    return *this->port &= (1 << this->pin);
}

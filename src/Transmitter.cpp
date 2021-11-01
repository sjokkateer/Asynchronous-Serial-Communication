#include "Transmitter.h"

void Transmitter::high()
{
    this->pin->high();
}

void Transmitter::low()
{
    this->pin->low();
}

TransmitState Transmitter::getState()
{
    return this->state;
}

void Transmitter::setState(TransmitState newState)
{
    this->state = newState;
}

void Transmitter::transmit(char data)
{
    this->data = data;
    this->low();
}

bool Transmitter::isBusy()
{
    return this->getState() != IDLE;
}

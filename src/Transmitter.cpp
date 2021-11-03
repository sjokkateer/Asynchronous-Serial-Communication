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

    switch (newState)
    {
    case IDLE:
        this->transmitBit = LSB;
        this->pin->high();
        break;
    default:
        break;
    }
}

bool Transmitter::isBusy()
{
    return this->getState() != IDLE;
}

void Transmitter::act(char transmitChar)
{
    switch (this->getState())
    {
    case TRANSMITTING:
        this->bitValue(transmitChar) ? this->high() : this->low();

        (this->transmitBit)++;

        if (this->transmitBit == 8)
        {
            this->setState(STOPPING);
        }

        break;
    case STOPPING:
        this->high();
        this->setState(RESETTING);
        break;
    default:
        break;
    }
}

uint8_t Transmitter::bitValue(char data)
{
    return data & (1 << this->transmitBit);
}

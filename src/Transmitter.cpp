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

void Transmitter::act(char transmitChar, uint8_t *transmitBit)
{
    switch (this->getState())
    {
    case TRANSMITTING:
        this->bitValue(transmitChar, *transmitBit) ? this->high() : this->low();

        (*transmitBit)++;

        if (*transmitBit == 8)
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

uint8_t Transmitter::bitValue(char data, uint8_t position)
{
    return data & (1 << position);
}

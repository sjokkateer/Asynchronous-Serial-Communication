#include "Transmitter.h"

void Transmitter::setState(TransmitState newState)
{
    this->state = newState;

    switch (newState)
    {
    case IDLE:
        this->transmitBit = LSB;
        this->pin->high();
        break;
    case STARTING:
        this->start();
        break;
    case TRANSMITTING:
        this->act();
        break;
    default:
        break;
    }
}

bool Transmitter::isBusy()
{
    return this->state != IDLE;
}

void Transmitter::act()
{
    switch (this->state)
    {
    case STARTING:
        this->setState(TRANSMITTING);
        break;
    case TRANSMITTING:
        this->bitValue() ? this->pin->high() : this->pin->low();

        (this->transmitBit)++;

        if (this->transmitBit == SIZE)
        {
            this->setState(STOPPING);
        }

        break;
    case STOPPING:
        this->pin->high();
        this->setState(RESETTING);
        break;
    case RESETTING:
        this->timer->disable();
        this->timer->reset();

        this->setState(IDLE);
        break;
    default:
        break;
    }
}

void Transmitter::transmit(char transmitChar)
{
    this->data = transmitChar;

    this->setState(STARTING);
}

uint8_t Transmitter::bitValue()
{
    return this->data & (1 << this->transmitBit);
}

void Transmitter::start()
{
    this->timer->reset();
    this->timer->enable();
    // Send start bit
    this->pin->low();
}

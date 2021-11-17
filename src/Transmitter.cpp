#include "Transmitter.h"
#include "string.h"

void Transmitter::setState(TransmitState newState)
{
    this->state = newState;

    // On entry
    switch (newState)
    {
    case TransmitState::IDLE:
        this->transmitBit = LSB;
        // Perhaps only pull pin high for idle
        // when the entire string is sent
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

void Transmitter::transmit(const char *string)
{
    int length = strlen(string);

    for (int i = 0; i < length; i++)
    {
        // Probably this was getting optimized by the
        // compiler when the body of the loop was empty.
        while (this->isBusy())
            ;

        this->transmit(string[i]);
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

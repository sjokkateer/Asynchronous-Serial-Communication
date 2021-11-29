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
        PORTD |= (1 << PD3);
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
        this->bitValue() ? PORTD |= (1 << PD3) : PORTD &= ~(1 << PD3);

        (this->transmitBit)++;

        if (this->transmitBit == SIZE)
        {
            this->setState(STOPPING);
        }

        break;
    case STOPPING:
        PORTD |= (1 << PD3);
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
        this->transmit(string[i]);
    }
}

void Transmitter::transmit(char transmitChar)
{
    this->data = transmitChar;
    this->setState(STARTING);

    // Block until done
    while (this->isBusy())
        ;
}

void Transmitter::transmitNewLine()
{
    this->transmit('\n');
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
    PORTD &= ~(1 << PD3);
}

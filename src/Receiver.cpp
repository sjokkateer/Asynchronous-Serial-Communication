#include "Receiver.h"

ReceiveState Receiver::getState()
{
    return this->state;
}

void Receiver::setState(ReceiveState state)
{
    this->state = state;

    switch (this->state)
    {
    case R_IDLE:
        this->timer->disable();
        this->interval = 0;
        this->index = 0;
        break;
    case RECEIVING:
        this->first = true;

        this->timer->setCompareValue(103);
        this->timer->reset();
        this->timer->enable();
        break;
    default:
        break;
    }
}

void Receiver::act()
{
    switch (this->state)
    {
    case R_IDLE:
        this->setState(RECEIVING);
        break;
    case RECEIVING:
        this->interval++;

        if (this->interval == 1)
        {
            this->timer->setCompareValue(206);
        }
        else if (this->interval == 10)
        {
            this->setState(COMPLETED);
        }

        this->bitBuffer[this->index] = this->pin->read();
        this->index++;

        break;
    case COMPLETED:
        this->setState(R_IDLE);
        break;
    default:
        break;
    }
}

bool *Receiver::getBuffer()
{
    return this->bitBuffer;
}

uint8_t Receiver::getPacketSize()
{
    return this->packetSize;
}

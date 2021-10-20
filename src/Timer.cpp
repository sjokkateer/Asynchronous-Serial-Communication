#include "Timer.h"

void Timer::reset()
{
    TCNT2 = 0;
}

void Timer::enable()
{
    TIMSK2 = 0 | (1 << OCIE2A);
}

void Timer::disable()
{
    TIMSK2 = 0;
}

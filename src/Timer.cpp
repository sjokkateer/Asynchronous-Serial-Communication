#include "Timer.h"

void Timer::reset()
{
    // Reset the current counter
    // of timer 2
    TCNT2 = 0;
}

void Timer::enable()
{
    // Timer interrupt flag register
    // requires us to write '1' to the
    // bit to reset potential interrupts.
    TIFR2 = 0 | (1 << OCF2A);
    // Enable compare match A interrupt
    TIMSK2 = 0 | (1 << OCIE2A);
}

void Timer::disable()
{
    // Disable all interrupts
    // in this case it will at least
    // disable the interrupts on compare match A
    // (assuming I-bit is written '1')
    TIMSK2 = 0;
}

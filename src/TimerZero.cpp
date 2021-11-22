#include "TimerZero.h"

void TimerZero::reset()
{
    // Reset the current counter
    // of timer 0
    TCNT0 = 0;
}

void TimerZero::enable()
{
    // Timer interrupt flag register
    // requires us to write '1' to the
    // bit to reset potential interrupts.
    TIFR0 = 0 | (1 << OCF0A);
    // Enable compare match A interrupt
    TIMSK0 = 0 | (1 << OCIE0A);
}

void TimerZero::disable()
{
    // Disable all interrupts
    // in this case it will at least
    // disable the interrupts on compare match A
    // (assuming I-bit is written '1')
    TIMSK0 = 0;
}

void TimerZero::setCompareValue(uint8_t compareValue)
{
    OCR0A = compareValue;
}

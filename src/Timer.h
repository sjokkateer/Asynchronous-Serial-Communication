#pragma once
#include <stdint.h>
#include <avr/io.h>

// Simplified class to only expose some methods
// that allow for bare minimum control to turn
// on/off timer interrupts and reset the timer's
// counter.
class Timer
{
public:
    Timer()
    {
        TCCR2A = 0 | (1 << WGM21);
        TCCR2B = 0 | (1 << CS21);
        OCR2A = 206;
    }

    // Resets the counter.
    void reset();
    // Enables compare match on A
    void enable();
    // Disables compare match on A
    void disable();
};

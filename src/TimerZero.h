#pragma once
#include <stdint.h>
#include <avr/io.h>

class TimerZero
{
public:
    /**
     * The constructer will INITIALIZE the timer/counter
     * by putting it in clear timer on compare match mode.
     * 
     * For the 8-bit timer to realize the opted delay/interval
     * the prescaler is set to 8 with a corresponding compare
     * value of 206.
     * 
     * The user will have to reset/enable etc. before activating
     * and using the timer.
     */
    TimerZero()
    {
        // Explicitly set the timer to 010
        // for clear timer on compare match mode
        TCCR0B &= ~(1 << WGM02);
        TCCR0A &= ~(1 << WGM00);
        TCCR0A |=  (1 << WGM01);

        // Set prescaler explicitly to 010
        // for for prescaler 8 (clock select bits)
        TCCR0B &= ~(1 << CS02);
        TCCR0B |=  (1 << CS01);
        TCCR0B &= ~(1 << CS00);

        // Reset the timer just in case.
        this->reset();
    }

    /**
     * Sets the current TC counter value to 0.
     */
    void reset();

    /**
     * Resets the output compare flag and
     * enables compare match A interrupts.
     */
    void enable();

    /**
     * Disables the interrupts for all in
     * the interrupt mask register.
     */
    void disable();

    void setCompareValue(uint8_t);
};

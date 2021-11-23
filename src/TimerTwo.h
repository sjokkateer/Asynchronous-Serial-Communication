#pragma once
#include <stdint.h>
#include <avr/io.h>

class TimerTwo
{
public:
    /**
     * The constructer will INITIALIZE the timer/counter2
     * by putting it in clear timer on compare match mode.
     * 
     * For the 8-bit timer to realize the opted delay/interval
     * the prescaler is set to 8 with a corresponding compare
     * value of 206.
     * 
     * The user will have to reset/enable etc. before activating
     * and using the timer.
     */
    TimerTwo()
    {
        // Explicitly set the timer to 010
        // for clear timer on compare match mode
        TCCR2B &= ~(1 << WGM22);
        TCCR2A &= ~(1 << WGM20);
        TCCR2A |=  (1 << WGM21);

        // Set prescaler explicitly to 010
        // for for prescaler 8
        TCCR2B &= ~(1 << CS22);
        TCCR2B |=  (1 << CS21);
        TCCR2B &= ~(1 << CS20);

        // Set output compare value that is
        // the result of measurements with
        // logic analyzer and theoretical.
        // (value is used for output compare interrupts)
        OCR2A = 206;

        // Reset the timer just in case.
        this->reset();
    }

    /**
     * Sets the current TC2 counter value to 0.
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
};

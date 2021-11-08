#include <Arduino.h>
#include "TransmitState.h"
#include "OutputPin.h"
#include "Timer.h"
#include "Transmitter.h"

const char *TEST_LINE = "Press 'S' or 's' to start the application.\n";

Transmitter *transmitter;
Timer *timer;

void setup()
{
    transmitter = new Transmitter(
        new OutputPin('D', 3),
        new Timer()
    );

    sei();
}

void loop()
{
    transmitter->transmit(TEST_LINE);
}

ISR(TIMER2_COMPA_vect)
{
    // Interrupt is triggered when sei(),
    // OCIE2A, and OCF2A are all '1'.

    // Hardware clears the OCF2A flag.
    transmitter->act();
}

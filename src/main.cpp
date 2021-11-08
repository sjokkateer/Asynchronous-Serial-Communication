#include <Arduino.h>
#include "TransmitState.h"
#include "OutputPin.h"
#include "Timer.h"
#include "Transmitter.h"

#define SIZE 8

#define MSB SIZE - 1

void idle();
bool bitValue(char, uint8_t);

volatile char transmitChar;

const char *TEST_LINE = "Press 'S' or 's' to start the application.\n";
uint8_t finalIndex;
uint8_t transmitCharIndex;

Transmitter *transmitter;
Timer *timer;

void setup()
{
    finalIndex = strlen(TEST_LINE) - 1;
    transmitCharIndex = -1;

    // If not dynamically constructed, the timer settings
    // through the constructor did not get set.
    transmitter = new Transmitter(
        new OutputPin('D', 3),
        new Timer()
    );

    sei();
}

void loop()
{
    if (transmitter->isBusy())
    {
        return;
    }

    if (transmitCharIndex == finalIndex)
    {
        transmitCharIndex = -1;
    }

    transmitCharIndex++;
    transmitChar = TEST_LINE[transmitCharIndex];

    transmitter->transmit(transmitChar);
}

ISR(TIMER2_COMPA_vect)
{
    // Interrupt is triggered when sei(),
    // OCIE2A, and OCF2A are all '1'.

    // Hardware clears the OCF2A flag.
    transmitter->act();
}

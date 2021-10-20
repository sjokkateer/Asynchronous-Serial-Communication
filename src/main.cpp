#include <Arduino.h>
#include "TransmitState.h"
#include "OutputPin.h"
#include "Timer.h"

#define SIZE 8

#define LSB 0
#define MSB SIZE - 1

void idle();
bool bitValue(char, uint8_t);

volatile TransmitState transmitState;
volatile uint8_t transmitBit;
volatile char transmitChar;

const char *TEST_LINE = "Press 'S' or 's' to start the application.\n";
uint8_t finalIndex;
uint8_t transmitCharIndex;

OutputPin transmitPin = OutputPin('D', 3);
Timer *timer;

void setup()
{
    finalIndex = strlen(TEST_LINE) - 1;
    transmitCharIndex = -1;

    timer = new Timer();

    idle();
    sei();
}

void loop()
{
    if (transmitState != IDLE)
    {
        return;
    }

    if (transmitCharIndex == finalIndex)
    {
        transmitCharIndex = -1;
    }

    transmitCharIndex++;
    transmitChar = TEST_LINE[transmitCharIndex];

    transmitState = TRANSMITTING;
    // Start bit
    timer->reset();
    timer->enable();
    transmitPin.low();
}

ISR(TIMER2_COMPA_vect)
{
    switch (transmitState)
    {
    case TRANSMITTING:
        bitValue(transmitChar, transmitBit) ? transmitPin.high() : transmitPin.low();

        transmitBit++;

        if (transmitBit == SIZE)
        {
            transmitState = STOPPING;
        }

        break;
    case STOPPING:
        transmitPin.high();
        transmitState = RESETTING;
        break;
    case RESETTING:
        idle();
        // Stop interrupts for this timer.
        timer->disable();
    default:
        break;
    }
}

void idle()
{
    transmitState = IDLE;
    transmitBit = LSB;
    timer->reset();
    transmitPin.high();
}

bool bitValue(char data, uint8_t position)
{
    return data & (1 << position);
}

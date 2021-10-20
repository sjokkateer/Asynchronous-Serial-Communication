#include <Arduino.h>
#include "TransmitState.h"
#include "OutputPin.h"
#include "Timer.h"
#include "Transmitter.h"

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

Transmitter *transmitter;
Timer *timer;

void setup()
{
    Serial.begin(9600);

    finalIndex = strlen(TEST_LINE) - 1;
    transmitCharIndex = -1;

    // If not dynamically constructed, the timer settings
    // through the constructor did not get set.
    transmitter = new Transmitter(new OutputPin('D', 3));
    timer = new Timer();

    idle();
    sei();
}

void loop()
{
    if (transmitter->getState() != IDLE)
    {
        return;
    }

    if (transmitCharIndex == finalIndex)
    {
        transmitCharIndex = -1;
    }

    transmitCharIndex++;
    transmitChar = TEST_LINE[transmitCharIndex];

    transmitter->setState(TRANSMITTING);
    // Start bit
    timer->reset();
    timer->enable();
    transmitter->low();
}

ISR(TIMER2_COMPA_vect)
{
    switch (transmitter->getState())
    {
    case TRANSMITTING:
        bitValue(transmitChar, transmitBit) ? transmitter->high() : transmitter->low();

        transmitBit++;

        if (transmitBit == SIZE)
        {
            transmitter->setState(STOPPING);
        }

        break;
    case STOPPING:
        transmitter->high();
        transmitter->setState(RESETTING);
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
    transmitter->setState(IDLE);
    transmitBit = LSB;
    timer->reset();
    transmitter->high();
}

bool bitValue(char data, uint8_t position)
{
    return data & (1 << position);
}

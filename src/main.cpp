#include <Arduino.h>
#include "TransmitState.h"
#include "OutputPin.h"

#define SIZE 8

#define LSB 0
#define MSB SIZE - 1

void idle();
void init();
void initTransmitPin();
void initTransmitTimer();

bool bitValue(char, uint8_t);

volatile TransmitState transmitState;
volatile uint8_t transmitBit;
volatile char transmitChar;

const char *TEST_LINE = "Press 'S' or 's' to start the application.\n";
uint8_t finalIndex;
uint8_t transmitCharIndex;

OutputPin transmitPin = OutputPin('D', 3);

void setup()
{
    finalIndex = strlen(TEST_LINE) - 1;
    transmitCharIndex = -1;

    idle();
    init();
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

    // This is equivalent to starting
    transmitState = TRANSMITTING;
    TIMSK2 |= (1 << OCIE2A);
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
        TIMSK2 = 0;
    default:
        break;
    }
}

void idle()
{
    // Resets the transmitter and related peripherals into an idle state
    transmitState = IDLE;
    transmitBit = LSB;
    TCNT2 = 0;
    transmitPin.high();
}

void init()
{
    initTransmitPin();
    initTransmitTimer();
}

void initTransmitPin()
{
    transmitPin.high();
}

void initTransmitTimer()
{
    TCCR2A = 0 | (1 << WGM21);
    TCCR2B = 0 | (1 << CS21);
    OCR2A = 206;
    TIMSK2 = 0 | (1 << OCIE2A);
}

bool bitValue(char data, uint8_t position)
{
    return data & (1 << position);
}

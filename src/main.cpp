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
    Serial.begin(9600);

    finalIndex = strlen(TEST_LINE) - 1;
    transmitCharIndex = -1;

    // If not dynamically constructed, the timer settings
    // through the constructor did not get set.
    transmitter = new Transmitter(new OutputPin('D', 3));

    // Timer now starts ticking and interrupted
    // although there would not be a direct reason
    // for this.
    // So the interrupt flag might already be set
    // before we actually care, and immediately get
    // interrupted
    timer = new Timer();
    timer->reset();

    _delay_ms(1500);

    Serial.println((TIFR2 & OCF2A) >> OCF2A);
    Serial.println(TCNT2);

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

    transmitter->setState(TRANSMITTING);
    // Start bit
    timer->reset();
    timer->enable();
    transmitter->low();
}

ISR(TIMER2_COMPA_vect)
{
    // Interrupt is triggered when sei(),
    // OCIE2A, and OCF2A are all '1'.

    // Hardware clears the OCF2A flag.
    switch (transmitter->getState())
    {
    case RESETTING:
        timer->disable();
        timer->reset();

        transmitter->setState(IDLE);
        break;
    default:
        transmitter->act(transmitChar);
        break;
    }
}

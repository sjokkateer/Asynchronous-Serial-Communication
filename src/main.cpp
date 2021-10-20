#include <Arduino.h>
#include "TransmitState.h"

#define SIZE 8

#define LSB 0
#define MSB SIZE - 1

#define TRANSMIT_DDR DDRD
#define TRANSMIT_PIN PD3
#define TRANSMIT_PORT PORTD

#define TRANSMIT_LOW TRANSMIT_PORT &= ~(1 << TRANSMIT_PIN)
#define TRANSMIT_HIGH TRANSMIT_PORT |= (1 << TRANSMIT_PIN)

#define TRANSMIT_PIN_VALUE TRANSMIT_PORT >> TRANSMIT_PIN

#define START_CHAR_VAL 65
#define MAX_CHAR_VAL 122

void reset();
void init();

bool bitValue(char, uint8_t);

volatile TransmitState transmitState;
volatile uint8_t transmitBit;
volatile char transmitChar;

void setup()
{
    cli();
    reset();
    init();
}

void loop()
{
}

ISR(TIMER2_COMPA_vect)
{
    switch (transmitState)
    {
    case IDLE:
        transmitState = STARTING;
        break;
    case STARTING:
        transmitState = TRANSMITTING;
        TRANSMIT_LOW;
        break;
    case TRANSMITTING:
        bitValue(transmitChar, transmitBit) ? TRANSMIT_HIGH : TRANSMIT_LOW;

        transmitBit++;

        if (transmitBit == SIZE)
        {
            transmitState = STOPPING;
        }

        break;
    case STOPPING:
        TRANSMIT_HIGH;
        transmitChar++;
        reset();
        break;
    default:
        break;
    }
}

void reset()
{
    transmitState = IDLE;
    transmitBit = LSB;

    if (transmitChar == MAX_CHAR_VAL)
    {
        transmitChar = START_CHAR_VAL;
    }

    TCNT2 = 0;
}

void init()
{
    // Define Tx as output pin
    TRANSMIT_DDR |= (1 << TRANSMIT_PIN);
    // Pull high to indicate idle
    TRANSMIT_HIGH;

    // Timer related setup
    TCCR2A = 0 | (1 << WGM21);
    TCCR2B = 0 | (1 << CS21);
    OCR2A = 206;
    TIMSK2 = 0 | (1 << OCIE2A);

    sei();
}

bool bitValue(char data, uint8_t position)
{
    return (data & (1 << position)) >> position;
}

void printCharAsByte(char data)
{
    for (int i = SIZE - 1; i >= 0; i--)
    {
        Serial.print((data & (1 << i)) >> i);
    }

    Serial.println();
}

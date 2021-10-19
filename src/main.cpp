#include <Arduino.h>
#include "TransmitState.h"

#define SIZE 8

#define LSB 0
#define MSB SIZE - 1

// Delay is a result of tweaking through logic analyzer
// to have it get closer to 1/9600.
#define DELAY_TIME 0.100

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

TransmitState transmitState;
uint8_t transmitBit;
char transmitChar;

void setup()
{
    reset();
    init();
}

bool once = false;

void loop()
{
    if (once) return;

    switch (transmitState)
    {
    case IDLE:
        transmitState = STARTING;
        break;
    case STARTING:
        transmitState = TRANSMITTING;
        TRANSMIT_LOW;
        _delay_ms(DELAY_TIME);
        break;
    case TRANSMITTING:
        bitValue(transmitChar, transmitBit) ? TRANSMIT_HIGH : TRANSMIT_LOW;

        transmitBit++;

        if (transmitBit == SIZE)
        {
            transmitState = STOPPING;
        }

        _delay_ms(DELAY_TIME);
        break;
    case STOPPING:
        TRANSMIT_HIGH;
        // Delay for stop bit
        _delay_ms(DELAY_TIME);
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

    if (transmitChar > MAX_CHAR_VAL)
    {
        // Transmit all characters from the range once to terminal client.
        once = true;
    }
}

void init()
{
    transmitChar = START_CHAR_VAL;
    // Define Tx as output pin
    TRANSMIT_DDR |= (1 << TRANSMIT_PIN);
    // Pull high to indicate idle
    TRANSMIT_HIGH;
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

#include <Arduino.h>
#include "ReceiveState.h"
#include "OutputPin.h"
#include "TimerTwo.h"
#include "Transmitter.h"
#include "TimerZero.h"
#include "InputPin.h"
#include "Receiver.h"

uint8_t characterValue;

void convertToBaseTen();
void printDetails();

Receiver *receiver;
Transmitter *transmitter;

// Presumably, if the program is transmitting data
// and a pin change interrupt occurs for receiving 
// a character, everything breaks.

// 

void setup()
{
    Serial.begin(9600);

    sei();
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT2);

    receiver = new Receiver(
        new InputPin('B', 2),
        new TimerZero(),
        10
    );

    transmitter = new Transmitter(
        new OutputPin('D', 3),
        new TimerTwo()
    );
}

bool receiveInterrupt = false;

ISR(PCINT0_vect)
{
    PCMSK0 &= ~(1 << PCINT2);

    receiveInterrupt = true;
}

ISR(TIMER0_COMPA_vect)
{
    receiver->act();
}

ISR(TIMER2_COMPA_vect)
{
    transmitter->act();
}

const char *string = "Application started!\n";

void loop()
{
    if (receiveInterrupt)
    {
        receiveInterrupt = false;
        receiver->act();
    }

    if (receiver->getState() == COMPLETED)
    {
        receiver->act();

        convertToBaseTen();
        Serial.println((char) characterValue);

        // while (transmitter->isBusy())
        // {
        // }

        // Turn on pin change interrupts for receive pin again
        PCMSK0 |= (1 << PCINT2);
    }
    transmitter->transmit("TEST STRING");
}

void convertToBaseTen()
{
    bool *buffer = receiver->getBuffer();

    uint8_t base = 1;
    characterValue = 0;

    for (uint8_t i = 1; i < receiver->getPacketSize() - 1; i++)
    {
        characterValue += base * buffer[i];
        base *= 2;
    }
}

void printDetails()
{
    bool *buffer = receiver->getBuffer();

    Serial.print("BYTE: ");
    for (uint8_t i = receiver->getPacketSize() - 2; i > 0; i--)
    {
        Serial.print(buffer[i]);
    }
    Serial.println();

    Serial.print("INTEGER: ");
    Serial.println(characterValue);

    Serial.print("CHARACTER: ");
    Serial.println((char)characterValue);

    Serial.println();
}

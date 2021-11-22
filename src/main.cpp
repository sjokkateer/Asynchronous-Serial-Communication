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

TimerZero *timer;
InputPin *pin;
Receiver *receiver;

void setup()
{
    Serial.begin(9600);

    sei();
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18);

    timer = new TimerZero();
    pin = new InputPin('D', 2);
    receiver = new Receiver(pin, timer, 10);
}

ISR(PCINT2_vect)
{
    PCMSK2 &= ~(1 << PCINT18);
    receiver->act();
}

ISR(TIMER0_COMPA_vect)
{
    receiver->act();
}

void loop()
{
    if (receiver->getState() == COMPLETED)
    {
        receiver->act();

        convertToBaseTen();
        printDetails();

        // Turn on pin change interrupts for receive pin again
        PCMSK2 |= (1 << PCINT18);
    }
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

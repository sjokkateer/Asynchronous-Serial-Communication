#include <Arduino.h>
#include "ReceiveState.h"
#include "OutputPin.h"
#include "TimerTwo.h"
#include "Transmitter.h"
#include "TimerZero.h"
#include "InputPin.h"
#include "Receiver.h"

char receivedChar;
bool receiveInterrupt = false;

void convertToBaseTen();
void printDetails();

Receiver *receiver;
Transmitter *transmitter;

void setup()
{
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
        receivedChar = receiver->getCharacter();

        // Echo the character to serial terminal client.
        transmitter->transmit(receivedChar);

        // Turn on pin change interrupts for receive pin again to unblock process.
        PCMSK0 |= (1 << PCINT2);
    }
}

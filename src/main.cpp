#include <Arduino.h>
#include "ReceiveState.h"
#include "OutputPin.h"
#include "TimerTwo.h"
#include "Transmitter.h"
#include "TimerZero.h"
#include "InputPin.h"

uint8_t interval;

// Is the size 1 start bit, 8 bit data frame, no parity bit, 1 stop bit aka 8n1
const uint8_t TOTAL_PACKET_SIZE = 10;
bool bitBuffer[TOTAL_PACKET_SIZE];
uint8_t index;
uint8_t characterValue;

volatile ReceiveState receiveState = R_IDLE;

void convertToBaseTen();
void printDetails();

TimerZero *timer;
InputPin *pin;

volatile bool first;

void setup()
{
    Serial.begin(9600);

    sei();
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18);

    timer = new TimerZero();
    pin = new InputPin('D', 2);
}

ISR(PCINT2_vect)
{
    PCMSK2 &= ~(1 << PCINT18);

    // Reset interval and index value since new receival
    interval = 0;
    index = 0;

    // Update receiver state
    receiveState = RECEIVING;

    first = true;

    timer->setCompareValue(103);
    timer->reset();
    timer->enable();
}

ISR(TIMER0_COMPA_vect)
{
    if (first)
    {
        timer->setCompareValue(206);

        first = false;
    }

    ++interval;

    if (interval >= 10)
    {
        receiveState = COMPLETED;
    }
    else
    {
        bitBuffer[index] = pin->read();
        index++;
    }
}

void loop()
{
    if (receiveState == COMPLETED)
    {
        // Turn off timer interrupts
        timer->disable();

        receiveState = R_IDLE;
        convertToBaseTen();
        printDetails();

        // Turn on pin change interrupts for receive pin again
        PCMSK2 |= (1 << PCINT18);
    }
}

void convertToBaseTen()
{
    uint8_t base = 1;
    characterValue = 0;

    for (uint8_t i = 1; i < TOTAL_PACKET_SIZE - 1; i++)
    {
        characterValue += base * bitBuffer[i];
        base *= 2;
    }
}

void printDetails()
{
    Serial.print("BYTE: ");
    for (uint8_t i = TOTAL_PACKET_SIZE - 2; i > 0; i--)
    {
        Serial.print(bitBuffer[i]);
    }
    Serial.println();

    Serial.print("INTEGER: ");
    Serial.println(characterValue);

    Serial.print("CHARACTER: ");
    Serial.println((char)characterValue);

    Serial.println();
}

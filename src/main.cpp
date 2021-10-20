#include <Arduino.h>
#include "ReceiveState.h"

#define RECEIVER PD2
#define RECEIVE_PIN_VAL (PIND & (1 << RECEIVER)) >> RECEIVER

uint8_t interval;

// Is the size 1 start bit, 8 bit data frame, no parity bit, 1 stop bit aka 8n1
const uint8_t TOTAL_PACKET_SIZE = 10;
bool bitBuffer[TOTAL_PACKET_SIZE];
uint8_t index;
uint8_t characterValue;

volatile ReceiveState receiveState = IDLE;

void convertToBaseTen();
void printDetails();

void setup()
{
    Serial.begin(9600);
    // RECEIVER input pin with internal pullup.
    DDRD &= ~(1 << RECEIVER);
    PORTD |= (1 << RECEIVER);

    sei();
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18);
}

ISR(PCINT2_vect)
{
    // Turn off pin change interrupt for input pin.
    PCMSK2 &= ~(1 << PCINT18);

    // CTC Mode for OCR1A
    TCCR0A = 0 | (1 << WGM01);
    // pre-scaler 8 to achieve our required delay
    TCCR0B = 0 | (1 << CS01);
    // Set compare value for interrupts
    // 0.05199 / (1 / 16000000 * 1000) / 8 rounded 104
    // minus 1 seemed to be more accurate on logic analyzer.
    OCR0A = 103;
    // Reset timer1 counter value
    TCNT0 = 0;
    // Enable timer compare interrupt
    TIMSK0 = 0 | (1 << OCIE0A);

    // Reset interval and index value since new receival
    interval = 0;
    index = 0;

    // Update receiver state
    receiveState = RECEIVING;
}

ISR(TIMER0_COMPA_vect)
{
    ++interval;

    if (interval >= 20)
    {
        receiveState = COMPLETED;
    }
    else if (interval % 2 == 1)
    {
        bitBuffer[index] = RECEIVE_PIN_VAL;
        index++;
    }
}

void loop()
{
    if (receiveState == COMPLETED)
    {
        // Turn off timer interrupts
        TIMSK0 &= ~(1 << OCIE0A);
        receiveState = IDLE;
        convertToBaseTen();
        printDetails();
        // Turn on pin change interrupts for receive pin again
        PCMSK2 |= (1 << PCINT18);
    }

    TCNT2 = 0;
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

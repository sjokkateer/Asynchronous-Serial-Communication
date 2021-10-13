#include <Arduino.h>

#define RECEIVER PD2
#define RECEIVE_PIN_VAL (PIND & (1 << RECEIVER)) >> RECEIVER

volatile bool interrupted = false;
uint8_t interval;

// Is the size 1 start bit, 8 bit data frame, no parity bit, 1 stop bit aka 8n1
const uint8_t TOTAL_PACKET_SIZE = 10;
bool bitBuffer[TOTAL_PACKET_SIZE];
uint8_t index;
uint8_t characterValue;

void convertToBaseTen();
void printDetails();

void setup()
{
    Serial.begin(9600);
    // RECEIVER input pin with internal pullup.
    DDRD &= ~(1 << RECEIVER);
    PORTD |= (1 << RECEIVER);

    // Perhaps on pin change interrupt flag and read in mainloop.
    sei();
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18);
}

ISR(PCINT2_vect)
{
    if (RECEIVE_PIN_VAL == 0)
    {
        interrupted = true;
        interval = 0;
        index = 0;
    }
}

void loop()
{
    if (!interrupted)
        return;

    // To not get interrupted by pin change interrupts while receiving data
    PCICR &= ~(1 << PCIE2);

    while (interval < 20)
    {
        ++interval;
        _delay_ms(0.05208);

        if (interval % 2 == 1)
        {
            bitBuffer[index] = RECEIVE_PIN_VAL;
            index++;
        }
    }

    interrupted = false;
    convertToBaseTen();
    printDetails();

    // Re-enable to get an interrupt for data receival.
    PCICR |= (1 << PCIE2);
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

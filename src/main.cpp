#include <Arduino.h>
#include "PinType.h"
#include "ApplicationState.h"

void init();
void transmit(uint8_t);
void transmitLine(const char *);
void transmitNewline();
uint8_t receive();
void act(char);
void displayMenu();
void displayDigitalInputPins();
void displayDigitalInputPin(int);
void displayInputPin(int, int, PinType);
void displayAnalogInputPins();
void displayAnalogInputPin(int);
void clearDisplay();

const char PIN_TYPE_MAP[] = { 'A', 'D' };
ApplicationState appState = AWAITING;

void setup()
{
  init();
}

char data;

void loop()
{
  data = receive();

  if (appState != STARTED)
  {
    if (data == 'S' || data == 's')
    {
      displayMenu();
      transmitNewline();

      appState = STARTED;
    }
    
    return;
  }

  act(data);
}

// USART Functions //
void init()
{
  UCSR0C = 0b00000110;
  // 9600 BPS
  UBRR0 = 103;
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
}

void transmit(uint8_t data)
{
  while (!(UCSR0A & (1 << UDRE0)))
    ;

  UDR0 = data;
}

void transmitLine(const char *line)
{
  for (unsigned int i = 0; i < strlen(line); i++)
  {
    transmit(line[i]);
  }

  transmitNewline();
}

void transmitNewline()
{
  transmit('\n');
}

uint8_t receive()
{
  while (!(UCSR0A & (1 << RXC0)))
    ;

  return UDR0;
}

// Application Functions //
void act(char data)
{
  switch (data)
  {
  case 'D':
  case 'd':
    displayDigitalInputPins();
    transmitNewline();
    break;
  case 'A':
  case 'a':
    displayAnalogInputPins();
    transmitNewline();
    break;
  case 'C':
  case 'c':
    clearDisplay();
    appState = AWAITING;
    break;
  default:
    break;
  }
}

const char *MENU_TITLE = "View current input levels:";

int const MENU_SIZE = 3;
const char *menuOptions[MENU_SIZE] = {
  "- D: Digital Inputs DI08 - DI13",
  "- A: Analog Inputs A0 - A5",
  "- C: Clear Screen"
};

void displayMenu()
{
  transmitLine(MENU_TITLE);
  for (uint8_t i = 0; i < MENU_SIZE; i++)
  {
    transmitLine(menuOptions[i]);
  }
}

const int DIGITAL_MIN_PIN = 8;
const int DIGITAL_MAX_PIN = 13;

void displayDigitalInputPins()
{
  for (int i = DIGITAL_MIN_PIN; i <= DIGITAL_MAX_PIN; i++)
  {
    displayDigitalInputPin(i);
    transmitNewline();
  }
}

char buffer[8];

void displayDigitalInputPin(int pin)
{
  displayInputPin(pin, digitalRead(pin), DIGITAL);
}

void displayInputPin(int pin, int value, PinType type)
{
  if (pin < 10) 
  {
    sprintf(buffer, "%cI0%i: %i", PIN_TYPE_MAP[type], pin, digitalRead(pin));
  } 
  else 
  {
    sprintf(buffer, "%cI%i: %i", PIN_TYPE_MAP[type], pin, digitalRead(pin));
  }
  transmitLine(buffer);
}

const int ANALOG_MIN_PIN = 0;
const int ANALOG_MAX_PIN = 5;

void displayAnalogInputPins()
{
  for (int i = ANALOG_MIN_PIN; i <= ANALOG_MAX_PIN; i++)
  {
    displayAnalogInputPin(i);
    transmitNewline();
  }
}

void displayAnalogInputPin(int pin)
{
  displayInputPin(pin, digitalRead(pin), ANALOG);
}

void clearDisplay()
{
  for (uint8_t i = 0; i < 54; i++)
  {
    transmitNewline();
  }
}

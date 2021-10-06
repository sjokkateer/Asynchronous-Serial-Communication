#include <Arduino.h>

void init();
void transmit(uint8_t);
void transmitLine(const char *);
uint8_t receive();
void act(char);
void displayMenu();

void setup()
{
  init();
}

char data;

void loop()
{
  data = receive();
  act(data);
}

// USART Functions //
void init()
{
  UCSR0C = 0b00000110;
  // 9600 BPS
  UBRR0 = 103;

  // Enable the Receiver
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

  transmit('\n');
}

uint8_t receive()
{
  // Polling
  while (!(UCSR0A & (1 << RXC0)))
    ;

  return UDR0;
}

// Application Functions //
// The application is in a waiting state/sleep until it receives S for start.
void act(char data)
{
  switch (data)
  {
  case 'S':
  case 's':
    // Display menu
    displayMenu();
    break;
  case 'D':
  case 'd':
    // Show the levels of the digital input pins 8 through 13
    break;
  case 'A':
  case 'a':
    // Show the levels of the analog input pins 0 through 5
    break;
  case 'C':
  case 'c':
    // Clear and stop terminal server
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

// Send the menu string character by character
void displayMenu()
{
  transmitLine(MENU_TITLE);
  for (uint8_t i = 0; i < MENU_SIZE; i++)
  {
    transmitLine(menuOptions[i]);
  }
  transmit('\n');
}

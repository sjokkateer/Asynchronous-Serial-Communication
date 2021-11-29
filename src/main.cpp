#include <Arduino.h>
#include "ReceiveState.h"
#include "OutputPin.h"
#include "TimerTwo.h"
#include "Transmitter.h"
#include "TimerZero.h"
#include "InputPin.h"
#include "Receiver.h"
#include "ApplicationState.h"
#include "PinType.h"

char receivedChar;
Receiver *receiver;
Transmitter *transmitter;

void init();
// Act for the application
// based on the received character.
void act(char);
void displayMenu();
void displayDigitalInputPins();
void displayDigitalInputPin(int);
void displayInputPin(int, int, PinType);
void displayAnalogInputPins();
void displayAnalogInputPin(int);
void clearDisplay();

const char PIN_TYPE_MAP[] = {'A', 'D'};
ApplicationState appState = AWAITING;

void setup()
{
  init();
}

ISR(PCINT0_vect)
{
  PCMSK0 &= ~(1 << PCINT2);
  receiver->act();
}

ISR(TIMER0_COMPA_vect)
{
  receiver->act();
}

ISR(TIMER2_COMPA_vect)
{
  transmitter->act();
}

bool once = false;

void loop()
{
  if (receiver->getState() == COMPLETED)
  {
    receiver->act();
    receivedChar = receiver->getCharacter();

    if (appState != STARTED)
    {
      if (receivedChar == 'S' || receivedChar == 's')
      {
        displayMenu();
        transmitter->transmitNewLine();

        appState = STARTED;
      }
    }

    if (appState != AWAITING) 
    {
      act(receivedChar);
    }

    // Turn on pin change interrupts for receive pin again to unblock process.
    PCMSK0 |= (1 << PCINT2);
  }

  if (appState == AWAITING && !once)
  {
    transmitter->transmit("Press 'S' or 's' to start the application.");
    transmitter->transmitNewLine();

    once = true;
  }
}

void init()
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

// Application Functions //
void act(char data)
{
  switch (data)
  {
  case 'D':
  case 'd':
    displayDigitalInputPins();
    transmitter->transmitNewLine();
    break;
  case 'A':
  case 'a':
    displayAnalogInputPins();
    transmitter->transmitNewLine();
    break;
  case 'C':
  case 'c':
    clearDisplay();
    appState = AWAITING;
    once = false;
    break;
  default:
    break;
  }
}

const char *MENU_HEADER = "View current input levels:";

int const MENU_SIZE = 3;
const char *menuOptions[MENU_SIZE] = {
    "- D: Digital Inputs DI08 - DI13",
    "- A: Analog Inputs A0 - A5",
    "- C: Clear Screen"
};

void displayMenu()
{
  transmitter->transmit(MENU_HEADER);
  transmitter->transmitNewLine();

  for (uint8_t i = 0; i < MENU_SIZE; i++)
  {
    transmitter->transmit(menuOptions[i]);
    transmitter->transmitNewLine();
  }
}

const int DIGITAL_MIN_PIN = 8;
const int DIGITAL_MAX_PIN = 13;

void displayDigitalInputPins()
{
  for (int i = DIGITAL_MIN_PIN; i <= DIGITAL_MAX_PIN; i++)
  {
    displayDigitalInputPin(i);
    transmitter->transmitNewLine();
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
  transmitter->transmit(buffer);
}

const int ANALOG_MIN_PIN = 0;
const int ANALOG_MAX_PIN = 5;

void displayAnalogInputPins()
{
  for (int i = ANALOG_MIN_PIN; i <= ANALOG_MAX_PIN; i++)
  {
    displayAnalogInputPin(i);
    transmitter->transmitNewLine();
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
    transmitter->transmitNewLine();
  }
}

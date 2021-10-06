#include <Arduino.h>

// Could be methods of a UART class
void init()
{
  // This must be done once before doing any tranmissions.

  // UCSR0C Register
  // We should set asynchronous USART UMSEL0[1:0] to 00
  // UPM0n for parity mode where we use none (n)
  // thus UPM0[1:0] 00 (disabled)
  // Then 1 stop bit through the USBS0 bit (0 for 1 bit)
  // We should set the character size (number of data bits in a frame)
  // that the receiver and transmitter use?
  // UCSZ02 in UCSR0B and UCSZ0[1:0] in UCSR0C (UCSZ0[1:0] should be set to 11)
  // Clock Polarity 0 should be set to 0 on our case as we use asynch USART
  UCSR0C = 0b00000110;
  // Set baud rate through UBRR0L and UBRR0H (USART Baud Rate 0 Register Low/High)
  // 12 bits in size. (4 MSB in H) 8 LSB in L.
  UBRR0 = 103; // See datasheet for common used values
  // OR
  // UBRR0H = 0b00000000;
  // UBRR0L = 0b01100111;

  // Enable transmitter through setting the Transmit Enable bit
  UCSR0B = (1 << TXEN0);
}

void transmit(uint8_t data)
{
  // Wait until transmit buffer is empty (UDRE bit is set)
  // Thus meaning that the transmit buffer
  // is ready to receive new data.

  // Thus loop as long as the bit is not set
  // and hence the buffer is not empty.
  while (!(UCSR0A & (1 << UDRE0))) // USART Data Register Empty bit
    ;

  // Write data to buffer (one char at a time of 8 bits)
  // writing to this register will move/copy over data
  // to the TXB (transmit data buffer register).
  // Next the data will be loaded into the transmit shift register
  // which will then be serially transmitted on the TxD0 pin.

  // And reading will return the contents of the
  // receive data buffer register (RXB).
  UDR0 = data;
}

void setup()
{
  init();
}

const char *data = "Hello, World!\n";

void loop()
{
  for (unsigned int i = 0; i < strlen(data); i++)
  {
    transmit(data[i]);
  }

  _delay_ms(2000);
}

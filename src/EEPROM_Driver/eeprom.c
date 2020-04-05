//Copyright 2019 Blacktop Group
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <msp430.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include "eeprom.h"

/* SPI Command Opcodes */
#define WREN	0x06
#define WRDI	0x04
#define RDSR	0x05
#define WRSR	0x01
#define READ	0x03
#define WRITE	0x02

bool init(void)
{
	//initialize MSP SPI
	P1OUT &= ~(BIT5 | BIT6 | BIT7);//set p1.5,1.6,1.7 to 0
	P1DIR |= (BIT5 | BIT6 | BIT7); //set p1.5,1.6,1.7 to outputs
	P1SEL |= BIT5 | BIT6 | BIT7; //configure pins
	P1SEL2 |= BIT5 | BIT6 | BIT7; //configure pins

	UCB0CTL1 |= UCSWRST; //put usci in reset mode to configure
	UCB0CTL1 |= UCSSEL_2; //SMCLK
	UCB0CTL0 = UCCKPH | UCMSB | UCMST | UCSYNC; //3-pin 8 bit spi master, most significant byte first, clock polarity high
	UCB0BR0 |=0x02;	//clock divider
	UCB0BR1 |=0;	//clock divider
	UCB0CTL1 &= ~UCSWRST;

	//CS Line is default high
	P1DIR |= BIT0;
	P1OUT |= BIT0;


	//initialize EEPROM
	//change write protection
	writeStatus(0x00);

	return true;
}
uint8_t read(uint16_t address)
{
	//create message
	uint8_t message[4] = {0};
	message[0] = READ; //read opcode
	message[1] = address >> 8;
	message[2] = address & 0xff;
	message[3] = 0x00; //dummy byte to allow reading

	//send message
	sendMessage(message, 4);

	//Read the data from the slave
	uint8_t value = UCB0RXBUF;

	return value;
}
bool write(uint16_t address, uint8_t data)
{
	//check if address is out of range
	if(address > 0x1fff)
	{
		return false;
	}

	// set write enable latch
	writeEnable();

	// // read the status register
	readStatus();

	//create message
	uint8_t message[4] = {0};
	message[0] = WRITE; //The opcode for a write sequence
	message[1] = address >> 8;
	message[2] = address & 0xff;
	message[3] = data;

	//send message
	sendMessage(message, 4);

	//wait until the status register indicates the write is complete
	uint8_t status = 0x01;
	do{
		status = readStatus();
	}while( (status & 0x01) == 0x01 ); //bit 1 of status register indicates write is in progress


	return true;
}

void writeEnable(void)
{
	uint8_t message[2] = {0};
	message[0] = WREN;
	sendMessage(message, 1);
}

void writeDisable(void)
{
	uint8_t message[2] = {0};
	message[0] = WRDI;
	sendMessage(message, 1);
}

uint8_t readStatus(void)
{
	uint8_t message[2] = {0};
	message[0] = RDSR;
	message[1] = 0x00;
	sendMessage(message, 2);
	uint8_t status = UCB0RXBUF;
	return status;
}

void writeStatus(uint8_t value)
{
	uint8_t message[2] = {0};
	message[0] = WRSR; 
	message[1] = value;
	sendMessage(message, 2);
	__delay_cycles(8);
}

void sendMessage(uint8_t* message, int length)
{
	//set CS line low
	P1OUT &=~ BIT0;

	for(int i = 0; i < length; i++)
	{
		UCB0TXBUF = message[i];
	}
	//reset CS line to high
	P1OUT |= BIT0;

	//delay
	__delay_cycles(8);
}
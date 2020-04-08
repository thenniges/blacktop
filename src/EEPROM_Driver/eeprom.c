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

bool eepromInit(void)
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
	eepromWriteStatus(0x00);

	return true;
}

bool eepromErase(void)
{
	for(uint16_t address = 0x0000; address <=0x1fff; address++)
	{
		eepromWrite(address, 0x00);
	}

	return true;
}

uint8_t eepromRead(uint16_t address)
{
	if(address > 0x1fff)
	{
		return 0x00; //TODO: is this the right value to return? talk to sphinx
	}
	//create message
	uint8_t message[4] = {0};
	message[0] = READ; //read opcode
	message[1] = address >> 8;
	message[2] = address & 0xff;
	message[3] = 0x00; //dummy byte to allow reading

	//send message
	eepromSendMessage(message, 4);

	//Read the data from the slave
	uint8_t value = UCB0RXBUF;

	return value;
}

bool eepromBlockRead(uint16_t address, uint8_t* data, int length)
{
	if(address > 0x1fff)
	{
		return false;
	}

	//create message
	uint8_t message[3] = {0};
	message[0] = READ; //read opcode
	message[1] = address >> 8;
	message[2] = address & 0xff;

	//send message
	//set CS line low
	P1OUT &=~ BIT0;
	for(int i = 0; i < 3; i++)
	{
		UCB0TXBUF = message[i];
	}
	for(int i = 0; i < length; i++)
	{
		UCB0TXBUF = 0x00;
		//Read the data from the slave
		__delay_cycles(8);
		data[i] = UCB0RXBUF;
	}
	//reset CS line to high
	P1OUT |= BIT0;

	return true;
}

bool eepromWrite(uint16_t address, uint8_t data)
{
	//check if address is out of range
	if(address > 0x1fff)
	{
		return false;
	}

	// set write enable latch
	eepromWriteEnable();

	//create message
	uint8_t message[4] = {0};
	message[0] = WRITE; //The opcode for a write sequence
	message[1] = address >> 8;
	message[2] = address & 0xff;
	message[3] = data;

	//send message
	eepromSendMessage(message, 4);

	//wait until the status register indicates the write is complete
	uint8_t status = 0x01;
	do{
		status = eepromReadStatus();
	}while( (status & 0x01) == 0x01 ); //bit 1 of status register indicates write is in progress


	return true;
}

bool eepromPageWrite(uint16_t address, uint8_t* data, int length)
{
	//check if address is out of range or length is too long
	if(address > 0x1fff || length > 32)
	{
		return false;
	}

	// set write enable latch
	eepromWriteEnable();

	//create message
	uint8_t message[35] = {0};
	message[0] = WRITE; //The opcode for a write sequence
	message[1] = address >> 8;
	message[2] = address & 0xff;
	for(int i = 0; i < length; i++)
	{
		message[i + 3] = data[i];
	}

	//send message
	eepromSendMessage(message, length + 3);

	//wait until the status register indicates the write is complete
	uint8_t status = 0x01;
	do{
		status = eepromReadStatus();
	}while( (status & 0x01) == 0x01 ); //bit 1 of status register indicates write is in progress


	return true;
}

void eepromWriteEnable(void)
{
	uint8_t message[2] = {0};
	message[0] = WREN;
	eepromSendMessage(message, 1);
}

void eepromWriteDisable(void)
{
	uint8_t message[2] = {0};
	message[0] = WRDI;
	eepromSendMessage(message, 1);
}

uint8_t eepromReadStatus(void)
{
	uint8_t message[2] = {0};
	message[0] = RDSR;
	message[1] = 0x00;
	eepromSendMessage(message, 2);
	uint8_t status = UCB0RXBUF;
	return status;
}

void eepromWriteStatus(uint8_t value)
{
	uint8_t message[2] = {0};
	message[0] = WRSR; 
	message[1] = value;
	eepromSendMessage(message, 2);
	__delay_cycles(8);
}

void eepromSendMessage(uint8_t* message, int length)
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
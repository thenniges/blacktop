//Copyright 2019 Blacktop Group
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <msp430.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include "eeprom.h"
#include "hardwareDriver.h"

/****************************
* Hardware Driver Functions *
*****************************/

void spiInit(void)
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
	UCB0CTL1 &= ~UCSWRST; //remove the reset and SPI is ready to use
}

void csInit(void)
{
    //CS Line is default high
	P1DIR |= BIT0;
	P1OUT &= ~BIT0;
	__delay_cycles(8);
	P1OUT |= BIT0;
}

uint8_t readSPIBuffer(void)
{
    //Read the data from the slave
	uint8_t value = UCB0RXBUF;
    return value;
}

void repeatedEEPROMRead(uint8_t* message, uint8_t* data, int length)
{
    //set CS line low
	P1OUT &=~ BIT0;
	//send message bytes
	for(int i = 0; i < 3; i++)
	{
		UCB0TXBUF = message[i];
	}
	//Send read bytes
	for(int i = 0; i < length; i++)
	{
		UCB0TXBUF = 0x00; //dummy byte to allow reading
		//Read the data from the slave
		__delay_cycles(8);
		data[i] = UCB0RXBUF;
	}
	//reset CS line to high
	P1OUT |= BIT0;
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

void eepromWaitForWriteCompletion(void)
{
	//wait until the status register indicates the write is complete
	uint8_t status = 0x01;
	do{
		status = eepromReadStatus();
	}while( (status & 0x01) == 0x01 ); //bit 1 of status register indicates write is in progress
}
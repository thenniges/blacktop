//Copyright 2019 Blacktop Group
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <msp430.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include "eeprom.h"

bool init(void)
{

	//TODO: needs the clock to be rising edge enable
	//USCIA0???
	
	P1OUT &= ~(BIT5 | BIT6 | BIT7);//set p1.5,1.6,1.7 to 0
	P1DIR |= (BIT5 | BIT6 | BIT7); //set p1.5,1.6,1.7 to outputs
	P1SEL |= BIT5 | BIT6 | BIT7; //configure pins
	P1SEL2 |= BIT5 | BIT6 | BIT7; //configure pins

	UCB0CTL1 |= UCSWRST; //put usci in reset mode to configure
	UCB0CTL1 |= UCSSEL_2; //SMCLK
	UCB0CTL0 |= UCCKPL | UCMST | UCMODE_0 | UCSYNC | UCMSB; //3-pin spi master, slave mode
	UCB0BR0 |=0x02;	//What does this do? clock divider?
	UCB0BR1 |=0;	//What does this do? clock divider?
	// UCB0MCTL = 0;	//no modulation

	//CS Line is default high
	P1DIR |= BIT0;
	P1OUT |= BIT0;

	UCB0CTL1 &= ~UCSWRST;

	//Send messages to eeprom to initialize it



	return true;
}
uint8_t read(uint16_t address)
{
	return 0;
}
bool write(uint16_t address, uint8_t data)
{
	//check if address is out of range
	if(adress > 0x1fff)
	{
		return false;
	}

	//Set the write enable latch
	P1OUT &=~ BIT0;
	UCB0TXBUF = 0x06;
	P1OUT |= BIT0;

	//create message
	uint8_t message[4] = {0};
	message[0] = 0x02; //The opcode for a write sequence
	message[1] = address >> 8;
	message[2] = address & 0xff;
	message[3] = data;

	//send message
	P1OUT &=~ BIT0;
	for(int i = 0; i < 4; i++)
	{
		UCB0TXBUF = message[i];
	}

	//reset CS line to high
	P1OUT |= BIT0;

	return true;
}
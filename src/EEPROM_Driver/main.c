//Copyright 2019 Blacktop Group
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <msp430.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include <stdint.h>
#include <stdbool.h>

#include "eeprom.h"

int main(void)
{
	WDTCTL  = WDTPW|WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;
	
	P3DIR |= (BIT2 | BIT5 | BIT6);
	P3OUT &= (~BIT6 & ~BIT2 & ~BIT5);

	uint16_t address = 0x0000;
	uint8_t write_value = 0xcb;
	
	eepromInit();
	
	//write a value to all addresses and check

	while(address <= 0x1fff)
	{
		eepromWrite(address, write_value);
		address++;
	}

	address = 0x0000;

	while(address <= 0x1fff)
	{
		uint8_t read_value = eepromRead(address);
		if(read_value != write_value)
		{
			P3OUT |= BIT6;
			break;
		}
		address++;
	}

	P3OUT |= BIT2;

	//erase and check

	eepromErase();

	address = 0x0000;

	while(address <= 0x1fff)
	{
		uint8_t read_value = eepromRead(address);
		if(read_value != 0x00)
		{
			P3OUT |= BIT6;
			break;
		}
		address++;
	}
	
	P3OUT |= BIT5;

	for(;;)
	{
	}
	return 0;
}
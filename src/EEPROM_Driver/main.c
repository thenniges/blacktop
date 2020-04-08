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

void testReadAndWrite();
void testErase();
void testGroupReadAndPageWrite();

int main(void)
{
	WDTCTL  = WDTPW|WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;

	P3DIR |= (BIT2 | BIT3 | BIT5 | BIT6);
	P3OUT &= (~BIT2 & ~BIT3 & ~BIT5 & ~BIT6);
	
	eepromInit();
	
	testReadAndWrite();
	testErase();
	testGroupReadAndPageWrite();

	for(;;)
	{
	}
	return 0;
}

void testReadAndWrite()
{
	uint16_t address = 0x0000;
	uint8_t write_value = 0xcb;
	bool success = true;
	//write a value to all addresses and check, turn rgb green when successful

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
			success = false;
			break;
		}
		address++;
	}
	if(success == true)
	{
		P3OUT |= BIT2;
	}
}

void testErase()
{
	bool success = true;
	//erase and check, turn rgb blue on when successful

	eepromErase();

	uint16_t address = 0x0000;

	while(address <= 0x1fff)
	{
		uint8_t read_value = eepromRead(address);
		if(read_value != 0x00)
		{
			P3OUT |= BIT6;
			success = false;
			break;
		}
		address++;
	}
	if(success == true)
	{
		P3OUT |= BIT5;
	}

}

void testGroupReadAndPageWrite()
{
	uint16_t address = 0x0000;
	bool success = true;
	//Test page write and block read, turn rgb white when successful
	uint8_t write_values[32] = {0x00};
	for(int i =0; i < 32; i++)
	{
		write_values[i] = i + 1;
	}

	eepromPageWrite(address, write_values, 32);

	uint8_t read_values[32] = {0x00};
	eepromBlockRead(address, read_values, 32);

	for(int i = 0; i <32; i++)
	{
		if(read_values[i] != write_values[i])
		{
			P3OUT |= BIT6;
			success = false;
			break;
		}
	}

	if(success == true)
	{
		P3OUT |= BIT3;
	}
}
//Copyright 2019 Blacktop Group
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "eeprom.h"
#include "hardwareDriver.h"

/*****************
* User Functions *
******************/

bool eepromInit(void)
{
	//initialize MSP SPI
	spiInit();

	//initialize the chip select pin
	csInit();


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

bool eepromRead(uint16_t address, uint8_t* value)
{
	if(address > 0x1fff)
	{
		return false;
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
	*value = readSPIBuffer();

	return true;
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
	repeatedEEPROMRead(message, data, length);

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

	//wait until write is complete
	eepromWaitForWriteCompletion();


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

	//wait until write is complete
	eepromWaitForWriteCompletion();

	return true;
}

//Copyright 2019 Blacktop Group
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <stdint.h>
#include <stdbool.h>

/*****************
* User Functions *
******************/

/*
* MUST be done ONCE BEFORE using any other functions in this project
* Setup the serial peripheral interface of the microprocessor for use with the EEPROM
* Change the write protection of the EEPROM to allow writing to the whole address space
*/
bool eepromInit(void);
/*
* Set all the data stored in the EEPROM array (addresses 0x0000 to 0x1fff) to 0x00
*/
bool eepromErase(void);
/*
* Read the data at the given address
* The data will be written to the address passed by the value variable
* Valid addresses are 0x0000 through 0x1fff
*/
bool eepromRead(uint16_t address, uint8_t* value);
/*
* Read the data in a continous block of size length starting at address
* Data will be placed in the data array initialized by the user, make sure you make the array at least size length
*/
bool eepromBlockRead(uint16_t address, uint8_t* data, int length);
/*
* Write the data variable to the given address
* Valid addresses are 0x0000 through 0x1fff
*/
bool eepromWrite(uint16_t address, uint8_t data);
/*
* Write the given data array to the given address in a continuous block of size length
* The length cannot exceed 32
* Valid addresses are 0x0000 through 0x1fff
*/
bool eepromPageWrite(uint16_t address, uint8_t* data, int length);
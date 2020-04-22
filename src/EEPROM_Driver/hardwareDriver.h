#include <stdint.h>
#include <stdbool.h>

/* SPI Command Opcodes */
#define WREN	0x06
#define WRDI	0x04
#define RDSR	0x05
#define WRSR	0x01
#define READ	0x03
#define WRITE	0x02

/****************************
* Hardware Driver Functions *
*****************************/
/*
* Initialize the serial peripheral interface of the MSP
*/
void spiInit(void);
/*
* Setup the chip select line (pin 1.0)
*/
void csInit(void);
/*
* Read the MSP serial peripheral interface receive buffer
*/
uint8_t readSPIBuffer(void);
/*
* Write the specified message, then repeatedly read the EEPROM MISO line length times and put the read data into data array
*/
void repeatedEEPROMRead(uint8_t* mesage, uint8_t* data, int length);
/*
* Write enable the EEPROM, so it is ready to receive a write
*/
void eepromWriteEnable(void);
/*
* Write disable the EEPROM, so it is unable to be written to without another write enable
*/
void eepromWriteDisable(void);
/*
* Read the status register of the EEPROM
* The status register is a 1 byte register with information about the EEPROM
*/
uint8_t eepromReadStatus(void);
/*
* Write to the status register of the EEPROM
* There are 3 bits that are writable, they are described in the data sheet
* This is only useful when changing write protection of the EEPROM
*/
void eepromWriteStatus(uint8_t value);
/*
* Send a SPI message to the EEPROM
* This is used by other messages to send a SPI message of size length
*/
void eepromSendMessage(uint8_t* message, int length);
/*
* Wait until write is complete
* This will continuously sample the status register and wait for a ready status
*/
void eepromWaitForWriteCompletion(void);
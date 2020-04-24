This code uses the Serial Peripheral Interface to allow users to initialize, erase, read, and write the EEPROM memory chip.

eeprom.c and eeprom.h contain the interface for the user to be able to interact with the EEPROM without any knowledge of SPI.
It allows them to be able to easily read and write data to the EEPROM off-chip memory device. The functions are explained briefly in eeprom.h

hardwareDriver.c and hardwareDriver.h allow the eeprom driver to interact with the hardware. None of these functions are intended to be used by the user. They are called appropriately from eeprom.c and eeprom.h 

main.c is used as a system test to ensure that the code is working on your board.

The make file is set up to run the code with main.c as the source file and eeprom.c and hardwareDriver.c as additional files. You can take eeprom.c, eeprom.h, hardwareDriver.c, and hardwareDriver.h and use them with other code. You can also take the makefile and change the source line and add any additional files.

to build the code:
$ make

to remove the binaries:
$ make clean

to flash the code:
$ make flash

to erasse the micro:
$ make erase
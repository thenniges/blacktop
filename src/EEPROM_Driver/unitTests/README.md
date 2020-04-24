This folder contains unit tests for the EEPROM driver. It tests the eeeprom files and mocks the hardwareDriver files. It verifies the messages being created in the EEPRom driver and abstracts out the hardware. This allows the unit tests to run locally on your computer

To run the tests:

$ cmake .
$ make
$ ./eepromTests
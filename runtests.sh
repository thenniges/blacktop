#!/bin/bash

# spacing from last test run
echo -e "\n\n"

#build unit tests
cd src/EEPROM_Driver/unitTests
cmake .
make

#and then run the tests
./eepromTests

# newlines for good measure
echo -e "\n\n"
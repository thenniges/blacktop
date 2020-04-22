 #!/bin/bash

# this script sets up 'when-changed' to watch the test directory and
# rerun all unit tests if there are any changes


when-changed -r "src/EEPROM_Driver/eeprom.c", "src/EEPROM_Driver/eeprom.h",  "src/EEPROM_Driver/unitTests/eepromTests.cpp" -c bash runtests.sh


#Copyright 2019 Blacktop Group
#Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# Project name
SOURCE          = main.c
ADDITIONAL      = eeprom.c hardwareDriver.c
# Get base name so we can create .elf file
NAME            = $(basename $(SOURCE))
# MSP430 MCU to compile for
CPU             = msp430g2553
# Optimisation level
OPTIMIZATION    = -O0
# Extra variables
CFLAGS          = -mmcu=$(CPU) $(OPTIMIZATION) -std=c99 -Wall -g -fomit-frame-pointer
# Libemb library link flags
LIBEMB          = -lserial -lconio

# Build and link executable
$(NAME).elf: $(SOURCE) $(ADDITIONAL)
	msp430-gcc $(CFLAGS) -o $@ $(SOURCE) $(ADDITIONAL) $(LIBEMB)
	msp430-objdump -D $(NAME).elf > $(NAME).hex

# Flash to board with mspdebug
flash: $(NAME).elf
	mspdebug tilib "prog $(NAME).elf"

# Erase board
erase:
	mspdebug tilib erase

# Clean up temporary files
clean:
	rm -f *.elf *.hex

# Remote debug board
debug: $(NAME).elf
	( mspdebug tilib "gdb" 1>/dev/null & ); msp430-gdb $(NAME).elf -ex "target remote :2000"


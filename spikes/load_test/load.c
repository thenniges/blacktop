//Copyright 2019 Blacktop Group
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <msp430.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>

int main(void)
{
	//set p1, p2, and p3 to 0
	P1OUT = 0;
	P2OUT = 0;
	P3OUT = 0;

	//set all to inputs
	P1DIR = 0xff;
	P2DIR = 0xff;
	P3DIR = 0xff;

	//unlock p2.6 and p2.7 and p3
	P2SEL &= ~(0xC0);
	P3SEL = 0x00;

	//turn on all digits of the 7 seg
	P3OUT |= (BIT0 | BIT1 | BIT4 | BIT7);

	//turn on the rgb to be whit and the red led
	P3OUT |= (BIT2 | BIT3 | BIT5 | BIT6);

	//turn on the green led
	P1OUT |= (BIT0); 

	for(;;)
	{
	}
	return 0;
}
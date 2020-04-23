//Copyright 2019 Blacktop Group
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <msp430.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include "dtc.h"

const int DIGS[4] = {0b10010010, 0b10010001, 0b10000011, 0b00010011};
const int NUMS[16] = {0b10111111, 0b10000110, 0b11011011, 0b11001111, 0b11100110, 0b11101101, 0b11111101, 0b10000111, 0b11111111, 0b11100111, 0b11110111, 0b11111100, 0b11011000, 0b11011110, 0b11111001, 0b11110001};

int main(void)
{
	WDTCTL  = WDTPW|WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;

	//set p1, p2, and p3 to 0
	P1OUT = 0;
	P2OUT = 0;
	P3OUT = 0;

	//set all to inputs
	P1DIR = 0xff;
	P2DIR = 0xff;
	P3DIR = 0xff;

	// //unlock p2.6 and p2.7 and p3
	P2SEL &= ~(0xC0);
	P3SEL = 0x00;

	//cycle through all digits on the 7 seg 
	//for pcb
	for(int i = 0; i < 4; i++)
	{
		P3OUT = (DIGS[i]);
		for(int j = 0; j < 16; j++)
		{
			P2OUT = (NUMS[j]);
			__delay_cycles(600000);
		} 
	}
	//for breadboard
	// for(int i = 0; i < 4; i++)
	// {
	// 	P3OUT = ~(DIGS[i]);
	// 	for(int j = 0; j < 16; j++)
	// 	{
	// 		P2OUT = ~(NUMS[j]);
	// 		__delay_cycles(600000);
	// 	} 
	// }
	//Clear p2 and p3 (turn off 7seg)
	P2OUT = 0;
	P3OUT = 0;

	//initialize onboard button (p1.3)
	P1DIR  &= ~BIT3;
	P1OUT |= BIT3;
	P1REN |= BIT3;
	P1IES |= BIT3;
	P1IE |= BIT3;
	P1IFG &= ~BIT3;

	//initialize extra button (p2.7)
	P2DIR  &= ~BIT7;
	P2OUT |= BIT7;
	P2REN |= BIT7;
	P2IES |= BIT7;
	P2IE |= BIT7;
	P2IFG &= ~BIT7;

	//enable global interrupts
	__bis_SR_register(GIE);

	//turn on the rgb to be white and the red led
	P3OUT |= (BIT2 | BIT3 | BIT5 | BIT6);

	//turn on the green led
	P1OUT |= (BIT0); 

	//set up dtc to allow potentiometer to control red led
	//initialize timer for Blue LED(3.5)
	TA0CCR0 = 1024;
	TA0CCR1  = 1024;
	TA0CCTL1 = OUTMOD_7;
	P3DIR |= BIT5;
	P3SEL |= BIT5;
	initialize_dtc(INCH_4, &TA0CCR1);
	TA0CTL   = TASSEL_2|MC_1|ID_0;


	for(;;)
	{
	}
	return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void BUTTON_ISR(void) {

	//debounce
	while(!(BIT3 & P1IN));
	__delay_cycles(32000);
	
	P3OUT ^= BIT6;

	P1IFG &= ~BIT3;
	return;
}

#pragma vector=PORT2_VECTOR
__interrupt void BUTTON2_ISR(void) {

	//debounce
	while(!(BIT7 & P2IN));
	__delay_cycles(32000);
	
	P1OUT ^= BIT0;

	P2IFG &= ~BIT7;
	return;
}
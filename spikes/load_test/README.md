This project uses the pin layout that is the same as the first rev of the PCB

CHIP    BOARD   FUNCTION

2       P1.0	P1.0 = GREEN LED/EEPROM CS

3	    P1.1	P1.1 = UART RECEIVE

4	    P1.2	P1.2 = UART TRANSMIT

5	    P1.3	P1.3 = ONBOARD BUTTON

6		        P1.4 = POTENTIOMETER INPUT

7		        P1.5 = EEPROM CLOCK

22		        P1.6 = EEPROM SOMI

23		        P1.7 = EEPROM SIMO

10		        P2.0 = SEGMENT A

11		        P2.1 = SEGMENT B

12		        P2.2 = SEGMENT C

16		        P2.3 = SEGMENT D

17		        P2.4 = SEGMENT E

18		        P2.5 = SEGMENT F

27		        P2.6 = SEGMENT G (XIN)

26		        P2.7 = EXTRA BUTTON (XOUT)

9		        P3.0 = DIGIT 1

8		        P3.1 = DIGIT 2

13	    P2.3	P3.2 = ONBOARD RGB (GREEN) TA1.1

14	    P2.1	P3.3 = ONBOARD RGB (BLUE) TA1.2

15		        P3.4 = DIGIT 3

19	    P2.5	P3.5 = ONBOARD RGB (RED) TA0.1

20	    P1.6	P3.6 = ONBOARD RED

21		        P3.7 = DIGIT 4

24	    ~RST

25	    TEST

1	    3V3

28	    GND


to build the code:
$ make

to flash the code:
$ make flash
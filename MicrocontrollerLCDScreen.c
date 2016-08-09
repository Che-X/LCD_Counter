//*******************************************************************************
// This program counts up or down between 0 and 999 and displays the number on the 
// LCD.  It will count up if SW1 is pressed and down if SW2 is pressed.
//*******************************************************************************

#include "msp430fg4618.h"
#include "stdio.h"
void Init_LCD(void);
unsigned char *LCDSeg = (unsigned char *) &LCDM3;
int LCD_SIZE=11;

int main(void){
	volatile unsigned int i;
	WDTCTL = WDTPW + WDTHOLD;	
	
	P1DIR &= 0xFC; // Set up SW1 and SW2 to inputs

	Init_LCD();
	// Turn off all of the seven segment displays
	for (i=0;i<LCD_SIZE;i++){
		LCDSeg[i]=0x00;
	}

// Set up array of bytes to specify which segments should turn on for each 
// decimal digit. First index is 0 and last index is 9. Refer to page 4-11 in 
// lab manual for interpretation of each byte.
	unsigned char numbers[] = {0x5F,0x06,0x6B,0x2F,0x36,0x3D,0x7D,0x07,0x7F,0x3F};
	
//3 digit number is in the format mkj (m is most sig, j is least)
int j=9,k=9,m=9; 


	for(;;){
		if (P1IN == 0x01){ //SW2 pressed: count down
			if (j == 0){ //Borrow from k
				if (k == 0){ //Borrow from m
					if (m == 0); //Reached 0, do nothing
					else{
						m--;  //Borrowing from m
						j = 9;
						k = 9;
					}
				}
				else{
					k--; //Borrowing from k
					j = 9;
				}
			}
			else
				j--;  //No borrow
		}

		if (P1IN == 0x02){ //SW1 pressed: count up
			if (j == 9){ //Carry to k
				if (k == 9){ //Carry to m

					if (m == 9); //Reached 999, do nothing

					else{
						m++; //Carrying to m
						j = 0;
						k = 0;
					}
				}
				else{
					k++; //Carrying to k
					j = 0;
				}
			}
			else
				j++; //No carry
		}
		
//Display each digit
		LCDSeg[0]= numbers[j];
		LCDSeg[1]= numbers[k];
		LCDSeg[2]= numbers[m];
		i = 10000; // SW Delay
		do i--;
		while (i != 0);

	}//closes infinite loop
}//closes main


#include <EFM8LB1.h>
#include <stdio.h>

char _c51_external_startup (void)
{
	// Disable Watchdog with key sequence
	SFRPAGE = 0x00;
	WDTCN = 0xDE; //First key
	WDTCN = 0xAD; //Second key

	SFRPAGE = 0x10;
	PFE0CN  = 0x20; // SYSCLK < 75 MHz.
		
	// Transition to 24.5 MHz first
	SFRPAGE = 0x00;
	CLKSEL  = 0x00;
	CLKSEL  = 0x00;
	while ((CLKSEL & 0x80) == 0);
	
	// Now switch to 72 MHz
	CLKSEL = 0x03;
	CLKSEL = 0x03;
	while ((CLKSEL & 0x80) == 0);
	
	P0MDOUT |= 0b_0001_1100; //  bit4=1, bit3=1, bit2=1 => push-pull
                             //  bits 7..5 and 1..0 unchanged

	XBR0 = 0X00;
	XBR1 = 0X00;
	XBR2 = 0x40; // Enable crossbar and weak pull-ups
	
	return 0;
}

void delay (unsigned int x)
{
	unsigned char j;
	while(--x)
	{
		for(j=0; j<100; j++);
	}
}

void main (void)
{
	while(1)
	{
		delay(50000);
		switch(mode)
		{
			case 0:
				P0_2 = 0;
				P0_3 = 0;
				P0_4 = 0;
				break;
			case 1:
				P0_2 = 1;
				P0_3 = 0;
				P0_4 = 0;
				break;
			case 2:
				P0_2 = 0;
				P0_3 = 1;
				P0_4 = 0;
				break;
			case 3:
				P0_2 = 0;
				P0_3 = 0;
				P0_4 = 1;
				break;
			case 4:
				P0_2 = 1;
				P0_3 = 1;
				P0_4 = 0;
				break;
			case 5:
				P0_2 = 0;
				P0_3 = 1;
				P0_4 = 1;
				break;
			default:
				P0_2 = 0;
				P0_3 = 0;
				P0_4 = 0;
				break;
		}

		mode++;
		if(mode > 5) mode =0;

	}
}

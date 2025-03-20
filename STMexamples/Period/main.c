#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../Common/Include/stm32l051xx.h"
#include "lcd.h"

#define F_CPU 32000000L

// Uses SysTick to delay <us> micro-seconds.
void Delay_us(unsigned char us)
{
	// For SysTick info check the STM32L0xxx Cortex-M0 programming manual page 85.
	SysTick->LOAD = (F_CPU/(1000000L/us)) - 1;  
	SysTick->VAL = 0; 
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	while((SysTick->CTRL & BIT16)==0); 
	SysTick->CTRL = 0x00; 
}

void waitms (unsigned int ms)
{
	unsigned int j;
	unsigned char k;
	for(j=0; j<ms; j++)
		for (k=0; k<4; k++) Delay_us(250);
}

void LCD_pulse (void)
{
	LCD_E_1;
	Delay_us(40);
	LCD_E_0;
}

void LCD_byte (unsigned char x)
{
	//Send high nibble
	if(x & 0x80) LCD_D7_1; else LCD_D7_0;
	if(x & 0x40) LCD_D6_1; else LCD_D6_0;
	if(x & 0x20) LCD_D5_1; else LCD_D5_0;
	if(x & 0x10) LCD_D4_1; else LCD_D4_0;
	LCD_pulse();
	Delay_us(40);
	//Send low nibble
	if(x & 0x08) LCD_D7_1; else LCD_D7_0;
	if(x & 0x04) LCD_D6_1; else LCD_D6_0;
	if(x & 0x02) LCD_D5_1; else LCD_D5_0;
	if(x & 0x01) LCD_D4_1; else LCD_D4_0;
	LCD_pulse();
}

void WriteData (unsigned char x)
{
	LCD_RS_1;
	LCD_byte(x);
	waitms(2);
}

void WriteCommand (unsigned char x)
{
	LCD_RS_0;
	LCD_byte(x);
	waitms(5);
}

void LCD_4BIT (void)
{
	LCD_E_0; // Resting state of LCD's enable is zero
	//LCD_RW=0; // We are only writing to the LCD in this program
	waitms(20);
	// First make sure the LCD is in 8-bit mode and then change to 4-bit mode
	WriteCommand(0x33);
	WriteCommand(0x33);
	WriteCommand(0x32); // Change to 4-bit mode

	// Configure the LCD
	WriteCommand(0x28);
	WriteCommand(0x0c);
	WriteCommand(0x01); // Clear screen command (takes some time)
	waitms(20); // Wait for clear screen command to finish.
}

void LCDprint(char * string, unsigned char line, unsigned char clear)
{
	int j;
	WriteCommand(line==2?0xc0:0x80);
	waitms(5);
	for(j=0; string[j]!=0; j++)
	{
		WriteData(string[j]);
	}
	if(clear)
	{
		for(; j<CHARS_PER_LINE; j++) WriteData(' ');
	}
}

// LQFP32 pinout
//             ----------
//       VDD -|1       32|- VSS
//      PC14 -|2       31|- BOOT0
//      PC15 -|3       30|- PB7
//      NRST -|4       29|- PB6
//      VDDA -|5       28|- PB5
//       PA0 -|6       27|- PB4
//       PA1 -|7       26|- PB3
//       PA2 -|8       25|- PA15
//       PA3 -|9       24|- PA14
//       PA4 -|10      23|- PA13
//       PA5 -|11      22|- PA12
//       PA6 -|12      21|- PA11
//       PA7 -|13      20|- PA10 (Reserved for RXD)
//       PB0 -|14      19|- PA9  (Reserved for TXD)
//       PB1 -|15      18|- PA8  (LED+1k)
//       VSS -|16      17|- VDD
//             ----------

void Configure_Pins (void)
{
	RCC->IOPENR |= BIT0; // peripheral clock enable for port A

	// Make pins PA0 to PA5 outputs
	GPIOA->MODER = (GPIOA->MODER & ~(BIT0|BIT1)) | BIT0; // PA0
	GPIOA->OTYPER &= ~BIT0; // Push-pull

	GPIOA->MODER = (GPIOA->MODER & ~(BIT2|BIT3)) | BIT2; // PA1
	GPIOA->OTYPER &= ~BIT1; // Push-pull

	GPIOA->MODER = (GPIOA->MODER & ~(BIT4|BIT5)) | BIT4; // PA2
	GPIOA->OTYPER &= ~BIT2; // Push-pull

	GPIOA->MODER = (GPIOA->MODER & ~(BIT6|BIT7)) | BIT6; // PA3
	GPIOA->OTYPER &= ~BIT3; // Push-pull

	GPIOA->MODER = (GPIOA->MODER & ~(BIT8|BIT9)) | BIT8; // PA4
	GPIOA->OTYPER &= ~BIT4; // Push-pull

	GPIOA->MODER = (GPIOA->MODER & ~(BIT10|BIT11)) | BIT10; // PA5
	GPIOA->OTYPER &= ~BIT5; // Push-pull

	

}

void delay(int dly)
{
	while(dly--);
}

void wait_1ms(void)
{
	SysTick->LOAD = (F_CPU/1000L) - 1;
	SysTick->VAL = 0; 
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	while((SysTick->CTRL & BIT16)==0);
	SysTick->CTRL = 0x00;
}

#define PIN_PERIOD (GPIOA->IDR & BIT8)

// GetPeriod() with added overflow tracking to handle low frequency (large capacitance).
// Minimal changes marked below.
long long int GetPeriod(int n) //  return type changed from 'long int' to 'long long int' to account for overflow
{
	int i;
	long long int overflows = 0; //  tracks systick overflow in a variable
	
	// Wait for square wave to be 0
	SysTick->LOAD = 0xffffff;
	SysTick->VAL  = 0xffffff;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	while(PIN_PERIOD != 0)
	{
		// Instead of returning immediately on overflow, count it:
		if(SysTick->CTRL & BIT16) overflows++; // adds overflow counting when the period isnt 0
		// Optional timeout to avoid infinite loop:
		if(overflows > 100000) // gets adjusted as needed
		{
			SysTick->CTRL = 0;
			return 0;
		}
	}
	SysTick->CTRL = 0x00;

	// Wait for square wave to be 1
	SysTick->LOAD = 0xffffff;
	SysTick->VAL  = 0xffffff;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	overflows = 0; // reseweft overflow counterrr before next wait
	while(PIN_PERIOD == 0)
	{
		if(SysTick->CTRL & BIT16) overflows++;
		if(overflows > 100000)
		{
			SysTick->CTRL = 0;
			return 0;
		}
	}
	SysTick->CTRL = 0x00;
	
	// Measure n cycles
	SysTick->LOAD = 0xffffff;
	SysTick->VAL  = 0xffffff;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	overflows = 0; //  reset before counting nnn cycles
	for(i=0; i<n; i++)
	{
		while (PIN_PERIOD != 0)
		{
			if(SysTick->CTRL & BIT16) overflows++;
			if(overflows > 100000)
			{
				SysTick->CTRL = 0;
				return 0;
			}
		}
		while (PIN_PERIOD == 0)
		{
			if(SysTick->CTRL & BIT16) overflows++;
			if(overflows > 100000)
			{
				SysTick->CTRL = 0;
				return 0;
			}
		}
	}
	SysTick->CTRL = 0x00;

	// Combine leftover SysTick counts with how many times it overflowed.
	// Each overflow is 0xffffff (24-bit) ccounts withi ni t.
	return ((long long int)overflows << 24) + (0xffffff - SysTick->VAL); // adjusts overflows with the system tick for val
}

void main(void)
{
	// Use 64-bit type for 'count' to store larger values
	long long int count; //was 'long int' now not
	float T, f, cap;
	char buff[17];
	unsigned int j = 20;

	Configure_Pins();
	waitms(j);
	LCD_4BIT();
	
	waitms(500);
	printf("4-bit mode LCD Test using the STM32L051.\r\n");
	
	LCDprint("Capacitance:", 1, 1);

	RCC->IOPENR |= 0x00000001; // peripheral clock enable for port A
	
	GPIOA->MODER &= ~(BIT16 | BIT17); // Make pin PA8 input
	GPIOA->PUPDR |= BIT16; 
	GPIOA->PUPDR &= ~(BIT17); 

	waitms(500); 
	printf("Period measurement using the Systick free running counter.\r\n"
	       "Connect signal to PA8 (pin 18).\r\n");
	
	while(1)
	{
		count = GetPeriod(100);
		if(count > 0)
		{
			// Convert to single-cycle period:
			T = count / (F_CPU * 100.0);
			f = 1.0 / T;

			cap = (1.44 * 1000 * 1000) / (1666.666 * 3 * (double)f);

			// Use '%lld' to print 64-bit integer:
			printf("f=%.2fHz, cap=%.5fmicroF, count=%lld            \r", f, cap, count); // <=== CHANGED

			if(cap < 1e-3)
			{
				sprintf(buff, "%.3f pF", cap * 1e6);
			}
			else if(cap < 0.5)
			{
				sprintf(buff, "%.3f nF", cap * 1e3 - 0.4); 
			}
			else
			{
				sprintf(buff, "%.3f microF", cap);
			}
			LCDprint(buff, 2, 1);
		}
		else
		{
			printf("NO SIGNAL                     \r");
			LCDprint("NO SIGNAL", 2, 1);
		}
		fflush(stdout);
		waitms(200);
	}
}
#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>

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

volatile int x;

void delay(int dly)
{
	while( dly--) x++;
}

void main(void)
{
	RCC->IOPENR |= BIT0; // peripheral clock enable for port A
    GPIOA->MODER = (GPIOA->MODER & ~(BIT17|BIT16)) | BIT16; // Make pin PA8 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0))
	
	GPIOA->MODER = (GPIOA->MODER & ~(BIT15|BIT14)) | BIT14; 
    GPIOA->MODER = (GPIOA->MODER & ~(BIT23|BIT22)) | BIT22; 
	
	//GPIOA->OTYPER &= ~(BIT4);
	
	// Ensure PA6 is push-pull
    //GPIOA->OTYPER &= ~(BIT6);  // Set PA6 as push-pull output
	
	// Disable any pull-up/pull-down resistors
    //GPIOA->PUPDR &= ~(BIT13 | BIT12);  // No pull-up, no pull-down
	
	// make sure not assigned to alternate func
	//GPIOA->AFR[0] &= ~(0xF << (6 * 4));  // Clear alternate function bits for PA6
	while(1)
	{
		GPIOA->ODR ^= BIT8; // Toggle PA8
		
		GPIOA->ODR ^= BIT7; // Toggle PA7
		
		GPIOA->ODR ^= BIT11; // Toggle PA11
		delay(5000000);
	}
}
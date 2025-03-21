#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Common/Include/serial.h"

#define F_CPU 32000000L // 32MHz operating frequency 
#define DEF_F 100000L   // 10µs tick for timer

volatile int PWM_Counter = 0;
volatile unsigned char pwm1 = 100, pwm2 = 100;

void wait_1ms(void)
{
	// For SysTick info check the STM32l0xxx Cortex-M0 programming manual.
	SysTick->LOAD = (F_CPU/1000L) - 1;  // set reload value for 1ms, counter rolls over from zero, hence -1
	SysTick->VAL = 0; // reset SysTick counter
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // enable SysTick IRQ and SysTick Timer
	while((SysTick->CTRL & BIT16)==0); // wait for timer rollover - bit 16 is the COUNTFLAG, True when counter rolls over from zero
	SysTick->CTRL = 0x00; // disable Systick counter
}

void delayms(int len)
{
	while(len--) wait_1ms(); // call waitms len times 
}

// Interrupt service routines are the same as normal
// subroutines (or C funtions) in Cortex-M microcontrollers.
// The following should happen at a rate of 1kHz.
// The following function is associated with the TIM2 interrupt 
// via the interrupt vector table defined in startup.c
void TIM2_Handler(void) 
{
	/* 
	Timer Interrupt Handler

	What this function does: 
		Clears the interrupt flag (TIM2->SR &= ~BIT0)
		Controls the PWM duty cycle:
			PA11 (PWM1) and PA12 (PWM2) are set HIGH if PWM_Counter is less than pwm1 or pwm2
			Otherwise, they are set LOW
		Resets PWM_Counter after 20ms to ensure 50 Hz PWM signal 
	*/

	TIM2->SR &= ~BIT0; // clear the update interrupt flag
	PWM_Counter++;     // increment the PWM counter
	
	if(pwm1>PWM_Counter)
	{
		GPIOA->ODR |= BIT11; // set PA11 HIGH 
	}
	else
	{
		GPIOA->ODR &= ~BIT11; // set PA11 LOW
	}
	
	if(pwm2>PWM_Counter)
	{
		GPIOA->ODR |= BIT12; // set PA12 HIGH
	}
	else
	{
		GPIOA->ODR &= ~BIT12; // set PA12 LOW
	}
	
	if (PWM_Counter > 2000) // 20ms PWM cycle (period = 20ms)
	{
		PWM_Counter=0;
		GPIOA->ODR |= (BIT11|BIT12); // reset PWM cycle
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
//       PA5 -|11      22|- PA12 (pwm2)
//       PA6 -|12      21|- PA11 (pwm1)
//       PA7 -|13      20|- PA10 (Reserved for RXD)
//       PB0 -|14      19|- PA9  (Reserved for TXD)
//       PB1 -|15      18|- PA8
//       VSS -|16      17|- VDD
//             ----------

void Hardware_Init(void)
{
	/* 
	Hardware Initialization
	
	What this function does: 
		Enables GPIOA clock.
		Configures PA11 and PA12 as outputs in push-pull mode
		Configures TIM2:
			Enables clock
			Sets reload value to generate 10µs timer ticks
			Enables interrupts
			Starts counting
	*/
	
	RCC->IOPENR |= BIT0; // peripheral clock enable for port A (GPIOA)

	// Set up output pins
    GPIOA->MODER = (GPIOA->MODER & ~(BIT22|BIT23)) | BIT22; // PA11 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT11; // push-pull mode
    GPIOA->MODER = (GPIOA->MODER & ~(BIT24|BIT25)) | BIT24; // PA12 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT12; // push-pull mode

	// Set up timer
	RCC->APB1ENR |= BIT0;      // enable clock for timer2 (UM: page 177)
	TIM2->ARR = F_CPU/DEF_F-1; // set auto-reload register for 10µs tick
	NVIC->ISER[0] |= BIT15;    // enable timer 2 interrupts in the NVIC
	TIM2->CR1 |= BIT4;         // enable downcounting   
	TIM2->CR1 |= BIT7;         // enable ARPE    
	TIM2->DIER |= BIT0;        // enable update event (reload event) interrupt 
	TIM2->CR1 |= BIT0;         // enable counting    
	
	__enable_irq(); // enable global interupts
}

int main(void)
{
	/*
	Calls Hardware_Init() to set up the GPIOs and timer
	Waits 500ms for terminal initialization
	Prints program description
	*/
    char buf[32];
    int npwm;

	Hardware_Init();
	
	delayms(500); // wait for putty to start
	
    printf("Servo signal generatioin using the STM32L051 using TIM2\r\n");
    printf("(outputs are PA11 and PA12, pins 21 and 22).\r\n");
    printf("By Jesus Calvino-Fraga (c) 2018-2023.\r\n\r\n");
	
	while (1)
	{
		/*
		Continuously asks the user for PWM values
		Ensures values are between 60 and 255
		Updates pwm1 and pwm2, which control the PWM signals
		*/

    	printf("PWM1 (60 to 255): ");
    	fflush(stdout);
    	egets_echo(buf, 31); // wait here until data is received from user input
  		printf("\r\n");

	    npwm = atoi(buf); // convert input string to integer

	    if(npwm != 0) // ignore zero input
	    {
		    if(npwm > 255) npwm = 255; // if user input is too large, set to max value
		    if(npwm < 60) npwm = 60;   // if user input is too small, set to min value
		    pwm1 = npwm;               // update PWM1 duty cycle
	    }
	    
    	printf("PWM2 (60 to 255): ");
    	fflush(stdout);
    	egets_echo(buf, 31); // wait here until data is received from user input
 		printf("\r\n");
 
	    npwm = atoi(buf);

	    if(npwm != 0)
	    {
		    if(npwm > 255) npwm = 255; // if user input is too large, set to max value
		    if(npwm < 60) npwm = 60;   // if user input is too small, set to min value 
		    pwm2 = npwm;               // update PWM2 duty cycle
		}
	}
}

/* Updated Servo_PWM to test DC motor 
 * RUN AS MAIN IN Servo_PWM.mk
 * Function: Asks for user input and sends that input as a PWM signal
 *     to a DC motor to control power and speed of two motors.         
 * ELEC 291 Project 2
 * Group B01
 * Created March 21st, 2024 by Madison Howitt
 *
 * A DC motor only needs a PWM signal to control speed. Unlike a servo, it doesn’t need a specific pulse width for positioning.
 * Changes: 
 *  Remove the fixed 20ms period (used for servos).
 *  Use only one output pin (e.g., PA11) for PWM.
 *  Set the PWM frequency to something higher (~1kHz-10kHz) for smooth motor control.
 *  User input should control duty cycle (0-100%) instead of servo position.
 */

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
	
	GPIOA->ODR |= BIT0; // set PA0 LOW
	GPIOA->ODR |= BIT2; // set PA2 LOW
	
	if(pwm1 > PWM_Counter) // pwm1 controls duty cycle (0-100%) instead of position
	{
		GPIOA->ODR |= BIT1; // set PA1 HIGH 
	}
	else
	{
		GPIOA->ODR &= ~BIT1; // set PA1 LOW
	}
	
	if(pwm2 > PWM_Counter) // pwm1 controls duty cycle (0-100%) instead of position
	{
		GPIOA->ODR |= BIT3; // set PA3 HIGH 
	}
	else
	{
		GPIOA->ODR &= ~BIT3; // set PA3 LOW
	}
	

	if (PWM_Counter >= 100) // 1ms PWM cycle (period = 1ms)
	{
		PWM_Counter=0; // reset PWM cycle
	}   
}

// STM32L051 pinout
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
    //GPIOA->MODER = (GPIOA->MODER & ~(BIT22|BIT23)) | BIT22; // PA11 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	//GPIOA->OTYPER &= ~BIT11; // push-pull mode
    //GPIOA->MODER = (GPIOA->MODER & ~(BIT24|BIT25)) | BIT24; // PA12 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	//GPIOA->OTYPER &= ~BIT12; // push-pull mode

	//motor pin configurations
	
	GPIOA->MODER = (GPIOA->MODER & ~(BIT0|BIT1)) | BIT0; // PA0 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT0; // push-pull mode
	
	GPIOA->MODER = (GPIOA->MODER & ~(BIT2|BIT3)) | BIT2; // PA1 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT1; // push-pull mode
	
	GPIOA->MODER = (GPIOA->MODER & ~(BIT4|BIT5)) | BIT4; // PA2 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT2; // push-pull mode
	
	GPIOA->MODER = (GPIOA->MODER & ~(BIT6|BIT7)) | BIT6; // PA3 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT3; // push-pull mode
	
	
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
    int speed1, speed2;

	Hardware_Init();
	
	delayms(500); // wait for putty to start
	
    //printf("Servo signal generation using the STM32L051 using TIM2\r\n");
    //printf("(output is PA11, pin 21).\r\n");
	
	while (1)
	{
		/*
		Continuously asks the user for PWM values
		Ensures values are between 0 and 100
		Updates pwm1, which controls the PWM signal
		*/

    	printf("Enter speed of motor 1 (0 to 100): ");
    	fflush(stdout);
    	egets_echo(buf, 31); // wait here until data is received from user input
  		printf("\r\n");

	    speed1 = atoi(buf); // convert input string to integer
                            // changed npwm to speed
                            
        printf("Enter speed of motor 2 (0 to 100): ");
    	fflush(stdout);
    	egets_echo(buf, 31); // wait here until data is received from user input
  		printf("\r\n");

	    speed2 = atoi(buf); // convert input string to integer
                            // changed npwm to speed
        // removed ignore zero condition
        
        // motor 1
        if(speed1 > 100) speed1 = 100; // if user input is too large, set to max value
        if(speed1 < 0) speed1 = 0;     // if user input is too small, set to min value
        pwm1 = speed1;           // convert percentage to PWM range (0-1000) and update duty cycle
        
        // motor 2
        if(speed2 > 100) speed2 = 100; // if user input is too large, set to max value
        if(speed2 < 0) speed2 = 0;     // if user input is too small, set to min value
        pwm2 = speed2;           // convert percentage to PWM range (0-1000) and update duty cycle
    }
}

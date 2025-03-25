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
#define PWM_MAX 100     // max PWM speed

volatile int PWM_Counter = 0;
volatile unsigned char pwm1 = 0, pwm2 = 0;

void move_backward(int speed) {
    // Set direction pins for backward movement
    GPIOA->ODR &= ~(BIT1 | BIT3); // Set PA1 & PA3 LOW
	GPIOA->ODR |= (BIT0 | BIT2);  // Set PA0 and PA2 HIGH (forward direction)
	
    pwm1 = speed;
    pwm2 = speed;
}

void move_forward(int speed) {
    // Set direction pins for forward movement
    GPIOA->ODR &= ~(BIT0 | BIT2); // Set PA1 & PA3 LOW
    GPIOA->ODR |= (BIT1 | BIT3);  // Set PA1 and PA3 HIGH (backward direction)

    pwm1 = speed;
    pwm2 = speed;
}

void move_left(int speed) {
    GPIOA->ODR &= ~(BIT0 | BIT2); 
    GPIOA->ODR |= (BIT1 | BIT3);
	
    pwm1 = speed;
    pwm2 = 0;
}

void move_right(int speed) {
    GPIOA->ODR &= ~(BIT0 | BIT2);
    GPIOA->ODR |= (BIT1 | BIT3);

	
    pwm1 = 0;
    pwm2 = speed;
}

void move_stop(void) {
    pwm1 = 0;
    pwm2 = 0;
}

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
    
    // Motor 1 PWM (PA0 for forward, PA1 for backward)
	if (PWM_Counter < pwm1) {
	    if ((GPIOA->ODR & BIT1) != 0) { // If PA1 is HIGH, move backward
	        GPIOA->ODR |= BIT1;  // Set PA1 HIGH
	    } else { // Forward
	        GPIOA->ODR |= BIT0;  // Set PA0 HIGH
	    }
	} else {
	    GPIOA->ODR &= ~(BIT0 | BIT1); // Clear both forward and backward pins
	}
	
	// Motor 2 PWM (PA2 for forward, PA3 for backward)
	if (PWM_Counter < pwm2) {
	    if ((GPIOA->ODR & BIT3) != 0) { // If PA3 is HIGH, move backward
	        GPIOA->ODR |= BIT3;  // Set PA3 HIGH
	    } else { // Forward
	        GPIOA->ODR |= BIT2;  // Set PA2 HIGH
	    }
	} else {
	    GPIOA->ODR &= ~(BIT2 | BIT3); // Clear both forward and backward pins
	}
	
    if(++PWM_Counter >= PWM_MAX) PWM_Counter = 0;
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
	RCC->APB1ENR |= BIT0; // Enable TIM2 clock


	// Motor pin configurations
	
    // Configure all motor control pins as outputs
    GPIOA->MODER &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    GPIOA->MODER |= (BIT0 | BIT2 | BIT4 | BIT6);
    
    // Set push-pull mode
    GPIOA->OTYPER &= ~(BIT0 | BIT1 | BIT2 | BIT3);
    
    // Initialize all pins to LOW
    GPIOA->ODR &= ~(BIT0 | BIT1 | BIT2 | BIT3);
    	
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

	Hardware_Init();
	delayms(500); // wait for putty to start

	while (1)
	{
		// Alternate forward and backward motion in a loop

        // Forward for 1 second
		move_forward(100);
		delayms(1000);

        // Backward for 1 second
		move_backward(100);
		delayms(1000); 

        // Turn right for 1 second
		move_right(100);
		delayms(1000);

        // Turn left for 1 second
		move_left(100);
		delayms(1000);

        // Stop + delay
		move_stop();
		delayms(1000);
    }
}

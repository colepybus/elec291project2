/* Auto Functions for DC Motor with Main
 * RUN AS MAIN IN Servo_PWM.mk
 * Function: Moves forward for 1 second and then turns at a random
 * 		angle and resumes forward motion to test auto functions.
 * ELEC 291 Project 2
 * Group B01
 * Created March 26th, 2024 by Madison Howitt
 */

#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Common/Include/serial.h"
#include <time.h>

#define F_CPU 32000000L // 32MHz operating frequency 
#define DEF_F 100000L   // 10µs tick for timer
#define PWM_MAX 100     // maximum PWM for speed 
#define LOWER_ANGLE 90  // minimum turn angle, can be changed based on how much of a turn / range we want 

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

void move_stop(void) {
    pwm1 = 0;
    pwm2 = 0;
}

void turn_CW(int speed) {
	// right wheel forward, left wheel backward (minimum turning radius)
	GPIOA->ODR &= ~(BIT1 | BIT2); // set PA1 & PA2 LOW
    GPIOA->ODR |= (BIT0 | BIT3);  // set PA0 & PA3 HIGH
    
    pwm1 = speed; 
    pwm2 = speed; 
}

void turn_CCW(int speed) { 
	// left wheel forward, right wheel backward (minimum turning radius)
	GPIOA->ODR &= ~(BIT0 | BIT3); // set PA0 & PA3 LOW
    GPIOA->ODR |= (BIT1 | BIT2);  // set PA1 & PA2 HIGH
    
    pwm1 = speed; 
    pwm2 = speed; 
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

void TIM2_Handler(void) 
{
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

void Hardware_Init(void)
{
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

void turn_random() { 
    
    /* Function: Turns on both motors at full speed in different directions and keeps
     *     them on for a length of time determined by a random angle.
     * Testing: Need to know how long it takes for a full 180 degree turn. This will
     *     be the maximum time set for a turning angle of +/- 180 with a linear
     *     relationship between time delay and random angle. Uses the sharpest 
     *     possible turning radius.
     */

    int turn_angle = (rand() % (180 - LOWER_ANGLE + 1)) + LOWER_ANGLE; // generate random angle
    int direction = rand() % 2;                  // generate random turn direction (0 or 1)
    float turn_time = 0.88 * turn_angle / 180.0; // time to turn 180 degrees is 0.88 seconds                      

    if (direction == 1) { 
        // turn right (CW)
        turn_CW(PWM_MAX); 
    } 
    else { 
        // turn left (CCW)
        turn_CCW(PWM_MAX); 
    }
    
    // keep motors on for amount of timer determined by random angle 
    delayms((int)(turn_time * 1000)); // convert turn time to ms

    // transition back to moving forward
    move_forward(PWM_MAX);
}

int main(void)
{
	Hardware_Init();
	delayms(500); // wait for putty to start

	while (1)
	{
		// Move forward and turn at a random angle in a loop
		move_forward(100); 
		delayms(1000); 
		turn_random(); 
    }
}

/* Functions for auto mode motor control, NOT FINISHED  
 * ELEC 291 Project 2
 * Group B01
 * Created March 25th, 2024 by Madison Howitt and Ria Dangi
 */

#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Common/Include/serial.h"
#include <time.h>

#define F_CPU 32000000L // 32MHz operating frequency 
#define DEF_F 100000L   // 10µs tick for timer
#define PWM_MAX 100     // max PWM speed 

volatile int PWM_Counter = 0;
volatile unsigned char pwm1 = 100, pwm2 = 100;

void move_forward(int speed) {
    // Set direction pins for forward movement
    GPIOA->ODR &= ~BIT1; // Set PA1 LOW
    GPIOA->ODR &= ~BIT3; // Set PA3 LOW

    if (pwm1 > PWM_Counter) GPIOA->ODR |= BIT0; // PA0 HIGH
    else GPIOA->ODR &= ~BIT0; // PA0 LOW

    if (pwm2 > PWM_Counter) GPIOA->ODR |= BIT2; // PA2 HIGH
    else GPIOA->ODR &= ~BIT2; // PA2 LOW

    if (++PWM_Counter >= PWM_MAX) PWM_Counter = 0; // Reset PWM cycle
    
    pwm1 = speed;
    pwm2 = speed;
}

void move_backward(int speed) {
    // Set direction pins for backward movement
    GPIOA->ODR &= ~BIT0; // Set PA0 LOW
    GPIOA->ODR &= ~BIT2; // Set PA2 LOW

    if (pwm1 > PWM_Counter) GPIOA->ODR |= BIT1; // PA1 HIGH
    else GPIOA->ODR &= ~BIT1; // PA1 LOW

    if (pwm2 > PWM_Counter) GPIOA->ODR |= BIT3; // PA3 HIGH
    else GPIOA->ODR &= ~BIT3; // PA3 LOW

    if (++PWM_Counter >= PWM_MAX) PWM_Counter = 0; // Reset PWM cycle
    
    pwm1 = speed;
    pwm2 = speed;
}

void update_motor_pwm() {
    if (pwm1 > PWM_Counter) GPIOA->ODR |= BIT1; // PA1 HIGH
    else GPIOA->ODR &= ~BIT1; // PA1 LOW

    if (pwm2 > PWM_Counter) GPIOA->ODR |= BIT3; // PA3 HIGH
    else GPIOA->ODR &= ~BIT3; // PA3 LOW

    if (++PWM_Counter >= PWM_MAX) PWM_Counter = 0; // Reset PWM cycle
}

void turn_right_man ()
{
    pwm1 = 100;
    pwm2 = 70;
}

void turn_left_man()
{
    pwm1 = 70;
    pwm2 = 100;
}

void robot_stop()
{
    pwm1 = 0;
    pwm2 = 0;
}

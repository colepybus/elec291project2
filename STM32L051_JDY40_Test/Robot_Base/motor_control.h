/* Motor Control Functions Header File
 * Declares the manual and automatic functions for DC motor control.
 * ELEC 291 Project 2
 * Group B01
 * Created March 26th, 2024 by Madison Howitt
 */

#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <stdint.h>
#include <stdlib.h> 
#include "../Common/Include/stm32l051xx.h" // hardware specific header

#define BIT0  (1 << 0)
#define BIT1  (1 << 1)
#define BIT2  (1 << 2)
#define BIT3  (1 << 3)
#define BIT4  (1 << 4)
#define BIT5  (1 << 5)
#define BIT6  (1 << 6)
#define BIT7  (1 << 7)

#define F_CPU 16000000L
#define DEF_F 1000
#define PWM_MAX 255
#define LOWER_ANGLE 30

//extern volatile int PWM_Counter;
extern volatile unsigned char pwm1, pwm2;

void move_backward(int speed);
void move_forward(int speed);
void move_left(int speed);
void move_right(int speed);
void move_stop(void);
void wait_1ms(void);
void delayms(int len);
void TIM2_Handler_header(void);
void turn_CW(int speed);
void turn_CCW(int speed);
void turn_random(void);

#endif

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

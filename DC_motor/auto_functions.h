/* Auto Functions for DC Motor
 * Use to make complete header file.
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
#define LOWER_ANGLE 30  // minimum turn angle 

void turn_CW(int speed); 
void turn_CCW(int speed); 
void turn_random();

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

void turn_random() { 
    /* 
     * Function: Turns on both motors at full speed in different directions and keeps
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

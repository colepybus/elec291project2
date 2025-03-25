/* Functions for auto mode motor control, NOT FINISHED  
 * ELEC 291 Project 2
 * Group B01
 * Created March 25th, 2024 by Madison Howitt
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
    GPIOA->ODR &= ~BIT0; // PA0 LOW
    GPIOA->ODR &= ~BIT2; // PA2 LOW

    pwm1 = speed;
    pwm2 = speed;
}

void move_backward(int speed) {
    // Set direction pins for backward movement
    GPIOA->ODR &= ~BIT1; // PA1 LOW
    GPIOA->ODR &= ~BIT3; // PA3 LOW

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

void turn_random_speed() {
    
    /* TURN OPTION 1 (controls speed)
     * Function: Turns on both motors for a set length of time at a speed 
     *     determined by a random angle.
     * Testing: Need to know how the wheel radius and the exact distance between
     *     the wheels. Distance is determined by arc length with a linear 
     *     relationship between speed and random angle. Can use any turning 
     *     radius.
     * THIS FUNCTION HAS NOT BEEN EDITED OR TESTED
     */
    
    int lower = 30; // minimum angle for turn, can be changed based on preference or for debugging
    int turn_angle = (rand() % (180 - lower + 1)) + lower; // generate random angle
    int direction = rand() % 2; // generate random turn direction (0 or 1)
  
    float D = 0.1;   // 10 cm, COMPLETE GUESS, need to measure distance between the wheels
    float R = 0.05;  // minimum turning radius, half of diameter above
    float W = 0.035; // 3.5 cm, COMPLETE GUESS, need to measure radius of wheels
    
    // arc length the robot must travel 
    float L = (turn_angle / 360.0) * (2 * pi * R);
  
    float V_out = (L + 3.14159 * D) / 2;
    float V_in = (L - 3.14159 * D) / 2;
  
    unsigned int max_speed = PWM_MAX; // max PWM value
  
    int pwm_out = (V_out / V_max) * max_speed;
    int pwm_in = (V_in / V_max) * max_speed;
  
    if (direction == 0) { 
        // turn left
        pwm1 = pwm_in;
        pwm2 = pwm_out;
    } 
    else { 
        // turn right
        pwm1 = pwm_out;
        pwm2 = pwm_in;
    }
  
    delayms(2000); // constant delay, calculated based on speed settings

    // stop motors
    pwm1 = 0;
    pwm2 = 0;
}

void turn_random_time() { 
    
    /* TURN OPTION 2 (controls time)
     * Function: Turns on both motors at full speed and keeps them on for a length 
     *     of time determined by a random angle.
     * Testing: Need to know how long it takes for a full 180 degree turn. This will
     *     be the maximum time set for a turning angle of +/- 180 with a linear
     *     relationship between time delay and random angle. Uses the sharpest 
     *     possible turning radius.
     * THIS FUNCTION HAS NOT BEEN EDITED OR TESTED
     */
    
    int lower = 30;                                        // minimum angle for turn, can be changed based on preference or for debugging
    int turn_angle = (rand() % (180 - lower + 1)) + lower; // generate random angle
    int direction = rand() % 2;                            // generate random turn direction (0 or 1)
    int speed = PWM_MAX;                                   // maximum speed for pivoting
    float max_turn_time = 2.0;                             // COMPLETE GUESS, time it takes to turn 180 degrees at maximum turning radius, needs to be measured
    
    int speed = PWM_MAX;   // maximum speed for pivoting

    if (direction == 0) { 
        // turn left
        pwm1 = -speed; // left wheel backward
        pwm2 = speed;  // right wheel forward
    } 
    else { 
        // turn right
        pwm1 = speed;  // left wheel forward
        pwm2 = -speed; // right wheel backward
    }
    int delay = turn_angle * (max_turn_time / 180.0); 
    delayms(2000); // Turn for 2 seconds

    // stop motors
    pwm1 = 0;
    pwm2 = 0;
}

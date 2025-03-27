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
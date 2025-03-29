// Bonus functions 

#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UART2.h"
#include "../Common/Include/serial.h"
#include "adc.h"

// maximum reading above base frequency divded by 7 
#define SCALE_LED 485

void LED_scale(int base_count, int count) {
    // 1 light on
    if (abs(base_count-count) < SCALE_LED) {
        P0_2 = 0;
        P0_3 = 0;
        P0_4 = 1;
    }
    // 2 lights on
    if ((abs(base_count-count) >= SCALE_LED) || (abs(base_count-count) < 2*SCALE_LED)) {
        P0_2 = 0;
        P0_3 = 1;
        P0_4 = 0;
    }
    // 3 lights on 
    if ((abs(base_count-count) >= 2*SCALE_LED) || (abs(base_count-count) < 3*SCALE_LED)) {
        P0_2 = 0;
        P0_3 = 1;
        P0_4 = 1;
    }
    // 4 lights on 
    if ((abs(base_count-count) >= 3*SCALE_LED) || (abs(base_count-count) < 4*SCALE_LED)) {
        P0_2 = 1;
        P0_3 = 0;
        P0_4 = 0;
    }
    // 5 lights on 
    if ((abs(base_count-count) >= 4*SCALE_LED) || (abs(base_count-count) < 5*SCALE_LED)) {
        P0_2 = 1;
        P0_3 = 0;
        P0_4 = 1;
    }
    // 6 lights on 
    if ((abs(base_count-count) >=5*SCALE_LED) || (abs(base_count-count) < 6*SCALE_LED)) {
        P0_2 = 1;
        P0_3 = 1;
        P0_4 = 0;
    }
    // 7 lights on 
    if (abs(base_count-count) >= 6*SCALE_LED) {
        P0_2 = 1;
        P0_3 = 1;
        P0_4 = 1;
    }
}



#define TOONIE_HIGH
#define TOONIE_LOW
#define LOONIE_HIGH
#define LOONIE_LOW
#define QUARTER_HIGH
#define QUARTER_LOW
#define DIME_HIGH
#define DIME_LOW
#define NICKEL_HIGH
#define NICKEL_LOW

volatile double money_count = 0.0;

// Add dollar value to global money_count based on metal detection intensity
// Print it to LCD

// detect     : metal detector intensity 
// init_count : GetPeriod(100), base_count in detectCoin function 
// coin_count : number of coins collected so far 

void money_counter(double detect, int init_count, int coin_count) {
    if (abs(init_count - coin_count) > 100) { 
        if (detect < TOONIE_HIGH && detect > TOONIE_LOW) {
            money_count += 2.00; 
        }
        if (detect < LOONIE_HIGH && detect > LOONIE_LOW) {
            money_count += 1.00; 
        }
        if (detect < QUARTER_HIGH && detect > QUARTER_LOW) {
            money_count += 0.25; 
        }
        if (detect < DIME_HIGH && detect > DIME_LOW) {
            money_count += 0.10; 
        }
        if (detect < NICKEL_HIGH && detect > NICKEL_LOW) {
            money_count += 0.05; 
        }
    }
}
		

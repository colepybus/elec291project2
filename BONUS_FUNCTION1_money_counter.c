#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UART2.h"
#include "../Common/Include/serial.h"
#include "adc.h"

// Ranges for coin detection
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

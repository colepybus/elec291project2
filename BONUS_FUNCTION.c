// Bonus functions 

#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UART2.h"
#include "../Common/Include/serial.h"
#include "adc.h"

// maximum reading above base frequency divded by 7 
#define SCALE_LED 400
#define BASE 183000

#define G5 784
#define A5 880
#define B5 988
#define C6 1047
#define D6 1175
#define E6 1316

void waitms (unsigned int ms)
{
	unsigned int j;
	unsigned char k;
	for(j=0; j<ms; j++)
		for (k=0; k<4; k++) Timer3us(250);
}

void play_song() {
    // Stop current playback
    TR2 = 0;
    
    // Define the melody (sequence of notes)
    unsigned int melody[] =        {G5,  A5,  C6,  A5,  E6,  E6,  D6,  G5,  A5,  C6,  A5,  D6,  D6,  C6,  G5,  A5,  C6,  A5,  C6,  D6,  B5,  A5,  G5,  G5,  D6,  C6};
    unsigned int noteDurations[] = {100, 100, 100, 100, 300, 300, 600, 100, 100, 100, 100, 300, 300, 600, 100, 100, 100, 100, 300, 300, 300, 100, 600, 200, 400, 400};
    
    for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
        setSpeakerFrequency(melody[i]);  // Play note
        delay_ms(noteDurations[i]);      // Hold note
    }
    
    // Stop playback after song
    TR2 = 0;
}

void LED_scale(int base_count, int count) {
    // 1 light on
    if (abs(BASE - count) < SCALE_LED) {
        P0_2 = 0;
        P0_3 = 0;
        P0_4 = 1;
    }
    // 2 lights on
    if ((abs(BASE - count) >= SCALE_LED) || (abs(base_count-count) < 2*SCALE_LED)) {
        P0_2 = 0;
        P0_3 = 1;
        P0_4 = 0;
    }
    // 3 lights on 
    if ((abs(BASE - count) >= 2*SCALE_LED) || (abs(base_count-count) < 3*SCALE_LED)) {
        P0_2 = 0;
        P0_3 = 1;
        P0_4 = 1;
    }
    // 4 lights on 
    if ((abs(BASE - count) >= 3*SCALE_LED) || (abs(base_count-count) < 4*SCALE_LED)) {
        P0_2 = 1;
        P0_3 = 0;
        P0_4 = 0;
    }
    // 5 lights on 
    if ((abs(BASE - count) >= 4*SCALE_LED) || (abs(base_count-count) < 5*SCALE_LED)) {
        P0_2 = 1;
        P0_3 = 0;
        P0_4 = 1;
    }
    // 6 lights on 
    if ((abs(BASE - count) >=5*SCALE_LED) || (abs(base_count-count) < 6*SCALE_LED)) {
        P0_2 = 1;
        P0_3 = 1;
        P0_4 = 0;
    }
    // 7 lights on 
    if (abs(BASE - count) >= 6*SCALE_LED) {
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
		

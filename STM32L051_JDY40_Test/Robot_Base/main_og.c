#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Common/Include/serial.h"
#include "adc.h"
#include <math.h>

#define F_CPU 32000000L
#define DEF_F 100000L // 10us tick

volatile int PWM_Counter = 0;
volatile unsigned char ISR_pwm1=100, ISR_pwm2=100;

void wait_1ms(void)
{
	// For SysTick info check the STM32l0xxx Cortex-M0 programming manual.
	SysTick->LOAD = (F_CPU/1000L) - 1;  // set reload register, counter rolls over from zero, hence -1
	SysTick->VAL = 0; // load the SysTick counter
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Enable SysTick IRQ and SysTick Timer */
	while((SysTick->CTRL & BIT16)==0); // Bit 16 is the COUNTFLAG.  True when counter rolls over from zero.
	SysTick->CTRL = 0x00; // Disable Systick counter
}

void waitms(int len)
{
	while(len--) wait_1ms();
}

// Interrupt service routines are the same as normal
// subroutines (or C funtions) in Cortex-M microcontrollers.
// The following should happen at a rate of 1kHz.
// The following function is associated with the TIM2 interrupt 
// via the interrupt vector table defined in startup.c
void TIM2_Handler(void) 
{
	TIM2->SR &= ~BIT0; // clear update interrupt flag
	PWM_Counter++;
	
	if(ISR_pwm1>PWM_Counter)
	{
		GPIOA->ODR |= BIT11;
	}
	else
	{
		GPIOA->ODR &= ~BIT11;
	}
	
	if(ISR_pwm2>PWM_Counter)
	{
		GPIOA->ODR |= BIT12;
	}
	else
	{
		GPIOA->ODR &= ~BIT12;
	}
	
	if (PWM_Counter > 2000) // THe period is 20ms
	{
		PWM_Counter=0;
		GPIOA->ODR |= (BIT11|BIT12);
	}   
}

// LQFP32 pinout
//                 ----------
//           VDD -|1       32|- VSS
//          PC14 -|2       31|- BOOT0
//          PC15 -|3       30|- PB7 (OUT 5)
//          NRST -|4       29|- PB6 (OUT 4)
//          VDDA -|5       28|- PB5 (OUT 3)
//           PA0 -|6       27|- PB4 (OUT 2)
//           PA1 -|7       26|- PB3 (OUT 1)
//           PA2 -|8       25|- PA15
//           PA3 -|9       24|- PA14 (push button)
//           PA4 -|10      23|- PA13
//           PA5 -|11      22|- PA12 (pwm2) - servo 2 (white robot)
//           PA6 -|12      21|- PA11 (pwm1) - servo 1 (yellow arm)
//           PA7 -|13      20|- PA10 (Reserved for RXD)
// (ADC_IN8) PB0 -|14      19|- PA9  (Reserved for TXD)
// (ADC_IN9) PB1 -|15      18|- PA8  (Measure the period at this pin)
//           VSS -|16      17|- VDD
//                 ----------

void Hardware_Init(void)
{
	RCC->IOPENR  |= (BIT1|BIT0);         // peripheral clock enable for ports A and B

	// Configure the pin used for analog input: PB0 and PB1 (pins 14 and 15)
	GPIOB->MODER |= (BIT0|BIT1);  // Select analog mode for PB0 (pin 14 of LQFP32 package)
	GPIOB->MODER |= (BIT2|BIT3);  // Select analog mode for PB1 (pin 15 of LQFP32 package)

	initADC();
	
	// Configure the pin used to measure period
	GPIOA->MODER &= ~(BIT16 | BIT17); // Make pin PA8 input
	// Activate pull up for pin PA8:
	GPIOA->PUPDR |= BIT16; 
	GPIOA->PUPDR &= ~(BIT17);
	
	// Configure the pin connected to the pushbutton as input
	GPIOA->MODER &= ~(BIT28 | BIT29); // Make pin PA14 input
	// Activate pull up for pin PA8:
	GPIOA->PUPDR |= BIT28; 
	GPIOA->PUPDR &= ~(BIT29);
	
	// Configure some pins as outputs:
	// Make pins PB3 to PB7 outputs (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
    GPIOB->MODER = (GPIOB->MODER & ~(BIT6|BIT7)) | BIT6;    // PB3
	GPIOB->OTYPER &= ~BIT3; // Push-pull
    GPIOB->MODER = (GPIOB->MODER & ~(BIT8|BIT9)) | BIT8;    // PB4
	GPIOB->OTYPER &= ~BIT4; // Push-pull
    GPIOB->MODER = (GPIOB->MODER & ~(BIT10|BIT11)) | BIT10; // PB5
	GPIOB->OTYPER &= ~BIT5; // Push-pull
    GPIOB->MODER = (GPIOB->MODER & ~(BIT12|BIT13)) | BIT12; // PB6
	GPIOB->OTYPER &= ~BIT6; // Push-pull
    GPIOB->MODER = (GPIOB->MODER & ~(BIT14|BIT15)) | BIT14;  // PB7
	GPIOB->OTYPER &= ~BIT7; // Push-pull
	
	// Set up servo PWM output pins
    GPIOA->MODER = (GPIOA->MODER & ~(BIT22|BIT23)) | BIT22; // Make pin PA11 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT11; // Push-pull
    GPIOA->MODER = (GPIOA->MODER & ~(BIT24|BIT25)) | BIT24; // Make pin PA12 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT12; // Push-pull

	// Set up timer
	RCC->APB1ENR |= BIT0;  // turn on clock for timer2 (UM: page 177)
	TIM2->ARR = F_CPU/DEF_F-1;
	NVIC->ISER[0] |= BIT15; // enable timer 2 interrupts in the NVIC
	TIM2->CR1 |= BIT4;      // Downcounting    
	TIM2->CR1 |= BIT7;      // ARPE enable    
	TIM2->DIER |= BIT0;     // enable update event (reload event) interrupt 
	TIM2->CR1 |= BIT0;      // enable counting    
	
	__enable_irq();
}

// A define to easily read PA8 (PA8 must be configured as input first)
#define PA8 (GPIOA->IDR & BIT8)

long long int GetPeriod(int n)
{
    int i;
    long long int overflows = 0;
    __disable_irq(); // Critical section start

    // Wait for signal to be 0
    SysTick->LOAD = 0xffffff;
    SysTick->VAL = 0xffffff;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    while (PA8 != 0)
    {
        if (SysTick->CTRL & BIT16) overflows++;
        if (overflows > 100000)
        {
            SysTick->CTRL = 0;
            __enable_irq();
            return 0;
        }
    }
    SysTick->CTRL = 0x00;

    // Wait for signal to be 1
    SysTick->LOAD = 0xffffff;
    SysTick->VAL = 0xffffff;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    overflows = 0;
    while (PA8 == 0)
    {
        if (SysTick->CTRL & BIT16) overflows++;
        if (overflows > 100000)
        {
            SysTick->CTRL = 0;
            __enable_irq();
            return 0;
        }
    }
    SysTick->CTRL = 0x00;

    // Measure n cycles
    SysTick->LOAD = 0xffffff;
    SysTick->VAL = 0xffffff;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    overflows = 0;
    for (i = 0; i < n; i++)
    {
        while (PA8 != 0)
        {
            if (SysTick->CTRL & BIT16) overflows++;
            if (overflows > 100000)
            {
                SysTick->CTRL = 0;
                __enable_irq();
                return 0;
            }
        }
        while (PA8 == 0)
        {
            if (SysTick->CTRL & BIT16) overflows++;
            if (overflows > 100000)
            {
                SysTick->CTRL = 0;
                __enable_irq();
                return 0;
            }
        }
    }
    SysTick->CTRL = 0x00;
    __enable_irq(); // Critical section end

    return ((long long int)overflows << 24) + (0xffffff - SysTick->VAL);
}


void PrintNumber(long int val, int Base, int digits)
{ 
	char HexDigit[]="0123456789ABCDEF";
	int j;
	#define NBITS 32
	char buff[NBITS+1];
	buff[NBITS]=0;

	j=NBITS-1;
	while ( (val>0) | (digits>0) )
	{
		buff[j--]=HexDigit[val%Base];
		val/=Base;
		if(digits!=0) digits--;
	}
	eputs(&buff[j+1]);
}

// Some 'defines' to turn pins on/off easily (pins must be configured as outputs)
#define PB3_0 (GPIOB->ODR &= ~BIT3)
#define PB3_1 (GPIOB->ODR |=  BIT3)
#define PB4_0 (GPIOB->ODR &= ~BIT4)
#define PB4_1 (GPIOB->ODR |=  BIT4)
#define PB5_0 (GPIOB->ODR &= ~BIT5)
#define PB5_1 (GPIOB->ODR |=  BIT5)
#define PB6_0 (GPIOB->ODR &= ~BIT6)
#define PB6_1 (GPIOB->ODR |=  BIT6)
#define PB7_0 (GPIOB->ODR &= ~BIT7)
#define PB7_1 (GPIOB->ODR |=  BIT7)

// A define to easily read PA14 (PA14 must be configured as input first)
#define PA14 (GPIOA->IDR & BIT14)

// TOGGLE MAGNET
void toggleMagnet(uint8_t state) {
	if (state) {
		PB3_1;
	} else {
		PB3_0;
	}
}

// LIFT ARM
void liftArm() {
	
}

// DROP ARM (opposites of liftArm)
void dropArm() {
	ISR_pwm1=75; ISR_pwm2=75;// starts default (1 - 75) (2 - 240)
	waitms(500);

	// ROTATE OUT
	//ISR_pwm2=82; // move bottom servo - 90 degrees left
	while (ISR_pwm2 < 157) {
		ISR_pwm2++;
		waitms(10);
	}

	waitms(500);
	// MOVE DOWN
	// ISR_pwm1=240; // move top servo - 180 degrees down
	while (ISR_pwm1 < 240) {
		ISR_pwm1++;
		waitms(10);
	}
	
	waitms(500);

	//SWEEP FOR COINS
	//ISR_pwm2=240;// move bottom servo - 90 degrees left
	toggleMagnet(1);
	while (ISR_pwm2 < 240) {
		
		ISR_pwm2++;
		waitms(10);
	}
	waitms(500);
	// MOVE UP
	//ISR_pwm1=75;// move top servo - 170 degrees up
	while (ISR_pwm1 > 75) {
		ISR_pwm1--;
		waitms(10);
	}

	waitms(500);
	// MOVE OVER BOX
	//ISR_pwm2=100;// move bottom servo - 120 degrees right
	
	while (ISR_pwm2 > 100) {
		ISR_pwm2--;
		waitms(10);
	}
	toggleMagnet(0); // turn off magnet
	waitms(500);
}

// DETECT PERIMETER
void detectPerimeter(int v1, int v2, int perimeter_threshold) {
	if ((v1%1000) > 1000 || (v2%1000) > 1000) { // checks if the 4 digits after decimal of v1 and v2 > perimeter threshold (100 = 0.1V)
		eputs("PERIMETER DETECTED!");
		// move backward
		// turn left
		// move forward
	}

	else {
		eputs("NO PERIMETER DETECTED!");
	}
}
// DETECT COIN
#define METAL_THRESHOLD 0.005 // 0.05% change from baseline
#define SAMPLE_SIZE 3 // number of samples for moving average
float freqBuffer[SAMPLE_SIZE] = {0};
uint8_t sampleIndex = 0; 
//uint8_t coinDetected = 0; // boolean as flag
float baseline_f=0;

void detectCoin() {
	long long int count;
	float f, avg_f=0;
	float f_change;
	

	count=GetPeriod(100);
		if(count>0)
		{
			f=(float)(F_CPU*100.0) / (float)count;

			eputs("DEBUG: Current Frequency:");
			PrintNumber(f, 10, 7);
			eputs(" Hz \r\n");
			eputs("count=\r\n");
			PrintNumber(count, 10, 6);
			eputs("          \r");


			// Store in moving average buffer
			freqBuffer[sampleIndex] = f;
			sampleIndex = (sampleIndex + 1) % SAMPLE_SIZE;
	 
			// Compute moving average
			for (int i = 0; i < SAMPLE_SIZE; i++) {
				avg_f += freqBuffer[i];
			}

			avg_f /= SAMPLE_SIZE;

			eputs("DEBUG: Moving Average Calculation: ");
			PrintNumber(avg_f, 10, 7);
			eputs(" Hz \r\n");

			// initialize baseline if needed
			if (baseline_f == 0) {
				baseline_f = f; // if first reading, make baseline the first reading
			}

			// calculate percentage change
			f_change = (avg_f - baseline_f) / baseline_f;
			eputs("DEBUG: Frequency Change:");
			PrintNumber(f_change, 10, 7);
			eputs(" Hz \r\n");

			// check
			if (fabs(f_change) > METAL_THRESHOLD) {
				eputs("COIN DETECTED!");
				waitms(3000);
			}
			else {
				eputs("NO COIN DETECTED!");
			}

			//waitms(50); // Delay to slow down sampling rate

		}

		else
		{
			eputs("NO SIGNAL                     \r");
		}

}



int main(void)
{
    int j, v;
	//long long int count;
	//float f;
	unsigned char LED_toggle=0; // Used to test the outputs

	int p1_v, p2_v; // perimeter sensor values


	Hardware_Init();
	
	waitms(500); // Give putty a chance to start before we send characters with printf()
	eputs("\x1b[2J\x1b[1;1H"); // Clear screen using ANSI escape sequence.
	eputs("\r\nSTM32L051 multi I/O example.\r\n");
	eputs("Measures the voltage from ADC channels 8 and 9 (pins 14 and 15 of LQFP32 package)\r\n");
	eputs("Measures period on PA8 (pin 18 of LQFP32 package)\r\n");
	eputs("Toggles PB3, PB4, PB5, PB6, PB7 (pins 26, 27, 28, 29, 30 of LQFP32 package)\r\n");
	eputs("Generates servo PWMs on PA11, PA12 (pins 21, 22 of LQFP32 package)\r\n");
	eputs("Reads the push-button on pin PA14 (pin 24 of LQFP32 package)\r\n\r\n");

    LED_toggle=0;
	PB3_0;
	PB4_0;
	PB5_0;
	PB6_0;
	PB7_0;
					
	while (1)
	{
		//PB3_1;

		j=readADC(ADC_CHSELR_CHSEL8);
		v=(j*33000)/0xfff;
		eputs("ADC[8]=0x");
		PrintNumber(j, 16, 4);
		eputs(", ");
		PrintNumber(v/10000, 10, 1);
		eputc('.');
		PrintNumber(v%10000, 10, 4);
		eputs("V ");

		j=readADC(ADC_CHSELR_CHSEL9);
		v=(j*33000)/0xfff;
		eputs("ADC[9]=0x");
		PrintNumber(j, 16, 4);
		eputs(", ");
		PrintNumber(v/10000, 10, 1);
		eputc('.');
		PrintNumber(v%10000, 10, 4);
		eputs("V ");
		
		eputs("PA14=");
		if(PA14)
		{
			eputs("1 ");
		}
		else
		{
			eputs("0 ");
		}


		// Now turn on one of outputs per cycle to check
		switch (LED_toggle++)
		{
			// case 0
				// eputs("CASE ZERO: turn magnet on");
				//PB3_1;
				// // toggleMagnet(1);
				// waitms(5000);
				// PB3_0;
				// // toggleMagnet(0);
				// waitms(5000);
				break;
			case 1:
				PB4_1;
				break;
			case 2:
				PB5_1;
				break;
			case 3:
				PB6_1;
				break;
			case 4:
				PB7_1;
				break;
			default:
			    LED_toggle=0;
				PB3_0;
				PB4_0;
				PB5_0;
				PB6_0;
				PB7_0;
				break;
		}

		// find default positions
		//ISR_pwm1=75; ISR_pwm2=75;
		
		detectCoin();
		//dropArm();
		//toggleMagnet(1);
		
		waitms(250);	
	}
}
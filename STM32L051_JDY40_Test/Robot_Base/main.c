#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UART2.h"
#include "../Common/Include/serial.h"
#include "adc.h"
//#include <math.h>
//#include "manual_functions.h"
//#include "auto_functions.h"
//#include "motor_control.h"

//THIS IS THE FULL FUNCTIONING STM32 CODE EVERYTHING MUST BE ADDED TO

// LQFP32 pinout
//                 ----------
//           VDD -|1       32|- VSS
//          PC14 -|2       31|- BOOT0
//          PC15 -|3       30|- PB7 
//          NRST -|4       29|- PB6 
//          VDDA -|5       28|- PB5 
//    out2   PA0 -|6       27|- PB4 
//    out3   PA1 -|7       26|- PB3 (OUT 1)
//    out4   PA2 -|8       25|- PA15
//    out5   PA3 -|9       24|- PA14 
//           PA4 -|10      23|- PA13
//           PA5 -|11      22|- PA12 (pwm2) - servo 2 (BASE: yellow -> green (mC))
//           PA6 -|12      21|- PA11 (pwm1) - servo 1 (ARM: green -> yellow (mC))
//(jdy push) PA7 -|13      20|- PA10 (Reserved for RXD)
// (ADC_IN8) PB0 -|14      19|- PA9  (Reserved for TXD)
// (ADC_IN9) PB1 -|15      18|- PA8  (Measure the period at this pin)
//           VSS -|16      17|- VDD
//                 ----------


//This is our main file for the robot base, it is responsible for the following:
// 1. Receive signal from EFM8 using the JDY40 module
// 2. Pick which mode (manual/automatic) to operate in based off a signal from JDY40
// MANUAL MODE: Take input from joystick, joystick press, turn that into wheel movement, arm/magnet trigger
// AUTOMATIC MODE: Set algorithm for robot to follow, operate wheels and arm based off algorithm and coin detection
// (detect coin --> execute arm thing --> turn 180 --> keep going)

// JDY40 NECESSARY INFORMATION TO BE RECEIEVED: 
// Operating mode (0,1,2), Joystick (float x_norm, float y_norm, bit press), leapmotion eventually 

// STILL NEED: JDY40 stuff, 

// leapmotion possible bonus, arm 3d operation with joysticks, play song and dance after challenge complete, take in path data and optimize with ML

//sets the cpu frequency to 32MHz, and makes DEF_F a 10us tick

//#define F_CPU 32000000L
//#define DEF_F 100000L // 10us tick

// sets a pwm counter and timer pwms to 100 initially

volatile int PWM_Counter_Servo = 0;
volatile unsigned char servo_pwm1=100, servo_pwm2=100;

#define F_CPU 32000000L     // Set CPU frequency to 32MHz
#define DEF_F 100000L       // 10us tick for timer
#define PWM_MAX 100
#define LOWER 90            // lower limit for random angle turn
#define METAL_THRESHOLD 100 // count changes by atleast 100 from baseline count when coin is near

volatile int PWM_Counter_Motor = 0;
volatile unsigned char motor_pwm1 = 0, motor_pwm2 = 0;

// flags
volatile int use_servo = 0; // flag to indicate if servo needs to be used

int done = 0; // flag to indicate if all coins picked up - yo check the type of this variable its supposed to be

// functions that makes it wait 1 ms

void wait_1ms(void)
{
	// For SysTick info check the STM32l0xxx Cortex-M0 programming manual.
	SysTick->LOAD = (F_CPU/1000L) - 1;  // set reload register, counter rolls over from zero, hence -1
	SysTick->VAL = 0; // load the SysTick counter
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Enable SysTick IRQ and SysTick Timer */
	while((SysTick->CTRL & BIT16)==0); // Bit 16 is the COUNTFLAG.  True when counter rolls over from zero.
	SysTick->CTRL = 0x00; // Disable Systick counter
}

// wait len milliseconds
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

	// PWM FOR SERVOS

	if (use_servo) { // only run the servo PWM  if use_servo flag is set
		PWM_Counter_Servo++; // increment servo PWM counter

		if (servo_pwm1 > PWM_Counter_Servo) {GPIOA->ODR |= BIT11;} // set PA11 high
		else {GPIOA->ODR &= ~BIT11;} // set PA11 low

		if (servo_pwm2 > PWM_Counter_Servo) {GPIOA->ODR |= BIT12;} // set PA12 high
		else {GPIOA->ODR &= ~BIT12;} // set PA12 low

		// reset the servo PWM counter after 20ms
		if (PWM_Counter_Servo > 2000) // the period is 20ms
		{
			PWM_Counter_Servo = 0;
			GPIOA->ODR |= (BIT11 | BIT12); // set both PA11 and PA12 high
		}
	}

	// PWM FOR MOTOR

	else // if use_servo flag is not set, run the motor PWM
	{
		PWM_Counter_Motor++; // increment motor PWM counter

	// Motor 1 PWM (PA0 for forward, PA1 for backward)
    	if (PWM_Counter_Motor < motor_pwm1) {
        	if ((GPIOA->ODR & BIT1) != 0) { // If PA1 is HIGH, move backward
            	GPIOA->ODR |= BIT1;  // Set PA1 HIGH
        	} 
			else { // Forward
            	GPIOA->ODR |= BIT0;  // Set PA0 HIGH
        	}
    	} 
		else {
        	GPIOA->ODR &= ~(BIT0 | BIT1); // Clear both forward and backward pins
    	}

        // Motor 2 PWM (PA2 for forward, PA3 for backward)
    	if (PWM_Counter_Motor < motor_pwm2) {
        	if ((GPIOA->ODR & BIT3) != 0) { // If PA3 is HIGH, move backward
            	GPIOA->ODR |= BIT3;  // Set PA3 HIGH
        	} 
			else { // Forward
            	GPIOA->ODR |= BIT2;  // Set PA2 HIGH
        	}
    	} 
		else {
        	GPIOA->ODR &= ~(BIT2 | BIT3); // Clear both forward and backward pins
    	}

    	if (++PWM_Counter_Motor >= PWM_MAX) {PWM_Counter_Motor = 0;}

	}
}

// hardware initialization
void Hardware_Init(void)
{
	RCC->IOPENR  |= (BIT1|BIT0);  // peripheral clock enable for ports A and B

	// Configure the pin used for analog input: PB0 and PB1 (pins 14 and 15)
	GPIOB->MODER |= (BIT0|BIT1);  // Select analog mode for PB0 (pin 14 of LQFP32 package)
	GPIOB->MODER |= (BIT2|BIT3);  // Select analog mode for PB1 (pin 15 of LQFP32 package)

	initADC();
	
	// Configure the pin used to measure period
	GPIOA->MODER &= ~(BIT16 | BIT17); // Make pin PA8 input
	// Activate pull up for pin PA8:
	GPIOA->PUPDR |= BIT16; 
	GPIOA->PUPDR &= ~(BIT17);

	// Activate pull up for pin PA8:
	GPIOA->PUPDR |= BIT1; 
	GPIOA->PUPDR &= ~(BIT2);
	
	// Configure some pins as outputs:
	// Make pins PB3 to PB7 outputs (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
     GPIOB->MODER = (GPIOB->MODER & ~(BIT6|BIT7)) | BIT6;    // PB3
	 GPIOB->OTYPER &= ~BIT3; // Push-pull
    // GPIOB->MODER = (GPIOB->MODER & ~(BIT8|BIT9)) | BIT8;    // PB4
	// GPIOB->OTYPER &= ~BIT4; // Push-pull
    // GPIOB->MODER = (GPIOB->MODER & ~(BIT10|BIT11)) | BIT10; // PB5
	// GPIOB->OTYPER &= ~BIT5; // Push-pull
    // GPIOB->MODER = (GPIOB->MODER & ~(BIT12|BIT13)) | BIT12; // PB6
	// GPIOB->OTYPER &= ~BIT6; // Push-pull
    // GPIOB->MODER = (GPIOB->MODER & ~(BIT14|BIT15)) | BIT14;  // PB7
	// GPIOB->OTYPER &= ~BIT7; // Push-pull

	// MOTOR PIN CONFIGURATIONS
	
	// Configure all motor control pins (PA0 - PA3) as outputs
	GPIOA->MODER &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
	GPIOA->MODER |= (BIT0 | BIT2 | BIT4 | BIT6);
	
	// Set push-pull mode
	GPIOA->OTYPER &= ~(BIT0 | BIT1 | BIT2 | BIT3);
	
	// Initialize all pins to LOW
	GPIOA->ODR &= ~(BIT0 | BIT1 | BIT2 | BIT3);
	
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

// SET UP JDY40 ----------------------------------------------------------------
	GPIOA->MODER = (GPIOA->MODER & ~(BIT27|BIT26)) | BIT26; // Make pin PA13 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0))
	GPIOA->ODR |= BIT13; // 'set' pin to 1 is normal operation mode.

	// change from PA8 -> PA13
	GPIOA->MODER &= ~(BIT26 | BIT27); // Make pin PA8 input
	// Activate pull up for pin PA8:
	GPIOA->PUPDR |= BIT26; 
	GPIOA->PUPDR &= ~(BIT27);
}

// FUNCTIONS FOR JDY40 ----------------------------------------------------------------
void SendATCommand (char * s)
{
	char buff[40];
	printf("Command: %s", s);
	GPIOA->ODR &= ~(BIT13); // 'set' pin to 0 is 'AT' mode.
	waitms(10);
	eputs2(s);
	egets2(buff, sizeof(buff)-1);
	GPIOA->ODR |= BIT13; // 'set' pin to 1 is normal operation mode.
	waitms(10);
	printf("Response: %s", buff);
}

void ReceptionOff (void)
{
	GPIOA->ODR &= ~(BIT13); // 'set' pin to 0 is 'AT' mode.
	waitms(10);
	eputs2("AT+DVID0000\r\n"); // Some unused id, so that we get nothing in RXD1.
	waitms(10);
	GPIOA->ODR |= BIT13; // 'set' pin to 1 is normal operation mode.
	while (ReceivedBytes2()>0) egetc2(); // Clear FIFO
}

// A define to easily read PA8 (PA8 must be configured as input first)
#define PA8 (GPIOA->IDR & BIT8)

// FUNCTIONS FOR MOTOR CONTROL ----------------------------------------------------------------
void move_backward(int speed) {
    // Set direction pins for backward movement
    GPIOA->ODR &= ~(BIT1 | BIT3); // Set PA1 & PA3 LOW
    GPIOA->ODR |= (BIT0 | BIT2);  // Set PA0 and PA2 HIGH (forward direction)

    motor_pwm1 = speed;
    motor_pwm2 = speed;
}

void move_forward(int speed) {
    // Set direction pins for forward movement
    GPIOA->ODR &= ~(BIT0 | BIT2); // Set PA1 & PA3 LOW
    GPIOA->ODR |= (BIT1 | BIT3);  // Set PA1 and PA3 HIGH (backward direction)

    motor_pwm1 = speed;
    motor_pwm2 = speed;
}

void move_left(int speed) {
    GPIOA->ODR &= ~(BIT0 | BIT2); 
    GPIOA->ODR |= (BIT1 | BIT3);

    motor_pwm1 = speed;
    motor_pwm2 = 0;
}

void move_right(int speed) {
    GPIOA->ODR &= ~(BIT0 | BIT2);
    GPIOA->ODR |= (BIT1 | BIT3);


    motor_pwm1 = 0;
    motor_pwm2 = speed;
}

void move_stop(void) {
    motor_pwm1 = 0;
    motor_pwm2 = 0;
}

void turn_CW(int speed) {
    GPIOA->ODR &= ~(BIT1 | BIT2);
    GPIOA->ODR |= (BIT0 | BIT3);
    pwm1 = speed;
    pwm2 = speed;
}

void turn_CCW(int speed) {
    GPIOA->ODR &= ~(BIT0 | BIT3);
    GPIOA->ODR |= (BIT1 | BIT2);
    pwm1 = speed;
    pwm2 = speed;
}

void turn_random() {
    int turn_angle = (rand() % (180 - LOWER_ANGLE + 1)) + LOWER_ANGLE;
    int direction = rand() % 2;
    float turn_time = 0.88 * turn_angle / 180.0;

    if (direction == 1) {
        turn_CW(PWM_MAX);
    } else {
        turn_CCW(PWM_MAX);
    }
    delayms((int)(turn_time * 1000));
    move_forward(PWM_MAX);
}

// GET PERIOD FUNCTION ------------------------------------------------------------------------------------
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

// prints the number given the value, the base and the number of digits

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

// TOGGLE MAGNET -------------------------------------------------------------------------------------------
void toggleMagnet(uint8_t state) {
	if (state) {
		PB3_1;
	} else {
		PB3_0;
	}
}

// PICK COIN MOVEMENT -------------------------------------------------------------------------------------------
void pickCoin() {
	use_servo = 1;

	servo_pwm1=75; servo_pwm2=75;// starts default (1 - 75) (2 - 240)
	waitms(500);

	// ROTATE OUT
	//ISR_pwm2=82; // move bottom servo - 90 degrees left
	while (servo_pwm2 < 157) {
		servo_pwm2++;
		waitms(10);
	}

	waitms(500);
	// MOVE DOWN
	// ISR_pwm1=240; // move top servo - 180 degrees down
	while (servo_pwm1 < 240) {
		servo_pwm1++;
		waitms(10);
	}
	
	waitms(500);

	//SWEEP FOR COINS
	//ISR_pwm2=240;// move bottom servo - 90 degrees left
	toggleMagnet(1);
	while (servo_pwm2 < 240) {
		
		servo_pwm2++;
		waitms(10);
	}
	waitms(500);
	// MOVE UP
	//ISR_pwm1=75;// move top servo - 170 degrees up
	while (servo_pwm1 > 75) {
		servo_pwm1--;
		waitms(10);
	}

	waitms(500);
	// MOVE OVER BOX
	//ISR_pwm2=100;// move bottom servo - 120 degrees right
	
	while (servo_pwm2 > 100) {
		servo_pwm2--;
		waitms(10);
	}
	toggleMagnet(0); // turn off magnet

	// set servo position back to default
	servo_pwm1=75;
	servo_pwm2=75;

	// turn use_servo flag off
	use_servo = 0; 
	waitms(500);
}

// DETECT PERIMETER -------------------------------------------------------------------------------------------
void detectPerimeter(int v1, int v2, int perimeter_threshold) {
	if ((v1%10000) > perimeter_threshold || (v2%10000) > perimeter_threshold) { // checks if the 4 digits after decimal of v1 and v2 > perimeter threshold (100 = 0.1V)
		eputs("PERIMETER DETECTED!");
		turn_random();
		/*
		move_backward(50); // move backward
        	// STOP MOVING BACKWARDS
		move_left(50); // turn left by 90 degrees
		move_forward(50); // continue moving forward */
	}

	else {
		eputs("NO PERIMETER DETECTED!");
	}
}

// METAL DETECTOR TO DETECT COIN  -------------------------------------------------------------------------------------------
void detectCoin() {
	long long int count;
	float f;
    static int first_time=1;
    static int base_count=0;
	static int coin_count=0; 
	
    if (first_time) // first reading, calibrate metal detector
    {
        base_count=GetPeriod(100); // set base_count 
        first_time=0;
    }

	count=GetPeriod(100); // count reading (instead of freq reading)
    eputs("count=");
    PrintNumber(count, 10, 6);
    eputs("\r\n");


    if(abs(base_count-count) > 100)
    {
        eputs("coin detected!\r\n");

        move_backward(100);
		waitms(500);
		move_stop();

        pickCoin();
		eputs("got coin!");
		eputs("coin count:");
		coin_count++;
		PrintNumber(coin_count, 10, 1);
		eputs("\r\n");

        base_count=GetPeriod(100); // recalibrate base_count after coin is picked

        move_forward(100);
    }

	// change to 20
	if (coin_count == 3) {
		eputs("3 COINS PICKED UP!!!! HOORAY!!! WE ARE DONE!!!");
		done = 1;
		move_stop();
		// play song and dance

	}
}

int main(void)
{
    int j, v;

	// unsigned char LED_toggle=0; // Used to test the outputs

	int p1_v, p2_v; // perimeter sensor ADC values read by MC

	// jdy variables
	char buff[80];
	int cnt=0;
	char c;
    int timeout_cnt=0;
    int cont1=0, cont2=100;

    // to know whether in automatic or manual mode

    static int mode = 0; // 0 = manual mode. 1 = automatic mode


	Hardware_Init();
	initUART2(9600);
	
	waitms(500); // Give putty a chance to start before we send characters with printf()
	eputs("\x1b[2J\x1b[1;1H"); // Clear screen using ANSI escape sequence.
	eputs("\r\nSTM32L051 multi I/O example.\r\n");
	eputs("Measures the voltage from ADC channels 8 and 9 (pins 14 and 15 of LQFP32 package)\r\n");
	eputs("Measures period on PA8 (pin 18 of LQFP32 package)\r\n");
	eputs("Generates servo PWMs on PA11, PA12 (pins 21, 22 of LQFP32 package)\r\n");

	ReceptionOff(); // for JDY

	// To check configuration of JDY
	SendATCommand("AT+VER\r\n");
	SendATCommand("AT+BAUD\r\n");
	SendATCommand("AT+RFID\r\n");
	SendATCommand("AT+DVID\r\n");
	SendATCommand("AT+RFC\r\n");
	SendATCommand("AT+POWE\r\n");
	SendATCommand("AT+CLSS\r\n");
	
	// We should select an unique device ID.  The device ID can be a hex
	// number from 0x0000 to 0xFFFF.  In this case is set to 0xABBA
	SendATCommand("AT+DVIDFDFD\r\n"); 
	SendATCommand("AT+RFC113\r\n"); 

	cnt=0;

    // LED_toggle=0;
	// PB3_0;
	// PB4_0;
	// PB5_0;
	// PB6_0;
	// PB7_0;

	// JDY40 
					
	while (1)
	{

		//move_forward(100);

		//PB3_1;

		 j=readADC(ADC_CHSELR_CHSEL8);
		 p1_v=(j*33000)/0xfff;
		// // eputs("ADC[8]=0x");
		// // PrintNumber(j, 16, 4);
		// // eputs(", ");
		// // PrintNumber(p1_v/10000, 10, 1);
		// // eputc('.');
		// // PrintNumber(p1_v%10000, 10, 4);
		// // eputs("V ");

		 j=readADC(ADC_CHSELR_CHSEL9);
		 p2_v=(j*33000)/0xfff;
		 //eputs("ADC[9]=0x");
		// // PrintNumber(j, 16, 4);
		// eputs(", ");
		// PrintNumber(p2_v/10000, 10, 1);
		// eputc('.');
		// PrintNumber(p2_v%10000, 10, 4);
		// eputs("V ");
		
		// eputs("PA14=");
		// if(PA14)
		// {
		// 	eputs("1 ");
		// }
		// else
		// {
		// 	eputs("0 ");
		// }

        // reset arm to default position
        //ISR_pwm1=75; ISR_pwm2=75;

		//stm recieving of data

		// if(ReceivedBytes2()>0) // Something has arrived
		// {
		// 	//eputs("GETTING IN THE LOOP\r\n");
		// 	//waitms(1000);
		// 	c=egetc2();
			
			if(c=='!') // Master is sending message
			{
				egets2(buff, sizeof(buff)-1);

				if(strlen(buff)==8)
				{
					// printf("Master says: %s\r", buff);

					// //move_forward(100);
					// printf(buff);
					// //move_stop();
					// eputs("mode: ");
					// PrintNumber(mode, 10, 1);
					// eputs("\r\n");

					// if (strstr(buff, "5")) {

					// 	eputs("MODE SWITCHED\r\n");

					// 	if (mode == 0) {
					// 		eputs("automatic mode activated\r\n");
					// 		mode = 1; // set to automatic mode
					// 	}
					// 	else {
					// 		eputs("manual mode activated\r\n");
					// 		mode = 0; // set to manual mode
					// 	}

					// 	PrintNumber(mode, 10, 1);
					// 	waitms(3000);

					// }

					if (strstr(buff, "5")) {
						eputs("AUTOMATIC MODE ACTIVATED\r\n");
						mode = 1; // set to automatic mode
						waitms(1000);
					}

					else if (strstr(buff, "6")) {
						eputs("MANUAL MODE ACTIVATED\r\n");
						mode = 0; // set to manual mode
						waitms(1000);
					}

				

				
					else if (mode == 0 && strstr(buff, "1")) {
						printf("moving forward (1)\r\n");
						move_forward(100);
						waitms(10);

						// manual mode
						// pickCoin();
						// toggleMagnet(1);
						// detectCoin();
					}

					else if (mode == 0 && strstr(buff, "2")) {
						printf("moving backward (2)\r\n");
						move_backward(100);
						waitms(10);
					}

					else if (mode == 0 && strstr(buff, "3")) {
						printf("turning right (3)\r\n");
						move_right(100);
						waitms(10);
					}

					else if (mode == 0 && strstr(buff, "4")) {
						printf("turning left (4)\r\n");
						move_left(100);
						waitms(10);
					}
		// 				// manual mode
		// 				// pickCoin();
		// 				// toggleMagnet(1);
		// 				// detectCoin();
		// 			}

					else
					{
						//move_forward(0);
						move_stop();
						printf("stopping robot\r\n");

						//waitms(150);
						
		// 			}
					
		// 		}
		// 		else
		// 		{
		// 			printf("*** BAD MESSAGE ***: %s\r", buff);
		// 		}				
		// 	}
		// 	else if(c=='@') // Master wants slave data
		// 	{
		// 		sprintf(buff, "%05u\n", cnt);
		// 		cnt++;
		// 		waitms(5); // The radio seems to need this delay...
		// 		eputs2(buff);
		// 	}
		// }

		// find default positions for servo
		// ISR_pwm1=75; ISR_pwm2=75;


        // AUTOMATIC MODE
        if (mode == 1) {

			if (done == 0) {
				//pickCoin();
				//detectCoin();
				//waitms(1000);
				move_forward(100);
				// detectPerimeter(p1_v, p2_v, 3000);
				detectCoin();
				waitms(1000);
			}
		
            //detectPerimeter(p1_v, p2_v, 3000);
            //detectCoin();
			// move_forward(100);
			// eputs("moving forward");
			// waitms(1000);
			// eputs("stopping now");
			
			//move_stop();
			//move_forward(0);

			//waitms(10000);

			// Alternate forward and backward motion in a loop

        	// Forward for 1 second
        	// move_forward(100);
        	// waitms(1000);

        	// // Backward for 1 second
        	// move_backward(100);
        	// waitms(1000); 

        	// // Turn right for 1 second
        	// move_right(100);
        	// waitms(1000);

        	// // Turn left for 1 second
        	// move_left(100);
        	// waitms(1000);

        	// // Stop + delay
        	// move_stop();
        	// waitms(1000);
        }
	
      
		//waitms(500);	

        		// // Now turn on one of outputs per cycle to check
		// switch (LED_toggle++)
		// {
		// 	// case 0
		// 		// eputs("CASE ZERO: turn magnet on");
		// 		//PB3_1;
		// 		// // toggleMagnet(1);
		// 		// waitms(5000);
		// 		// PB3_0;
		// 		// // toggleMagnet(0);
		// 		// waitms(5000);
		// 		break;
		// 	case 1:
		// 		PB4_1;
		// 		break;
		// 	case 2:
		// 		PB5_1;
		// 		break;
		// 	case 3:
		// 		PB6_1;
		// 		break;
		// 	case 4:
		// 		PB7_1;
		// 		break;
		// 	default:
		// 	    LED_toggle=0;
		// 		PB3_0;
		// 		PB4_0;
		// 		PB5_0;
		// 		PB6_0;
		// 		PB7_0;
		// 		break;y
		// }
	}

}

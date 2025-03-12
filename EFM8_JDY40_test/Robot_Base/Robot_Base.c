#include <EFM8LB1.h>
#include <stdlib.h>
#include <stdio.h>

volatile unsigned int servo_counter=0;
volatile unsigned char servo1=150, servo2=150;

#define SERVO1     P1_6
#define SERVO2     P1_7
#define PERIOD_PIN P1_5
#define OUTPIN1    P1_0
#define OUTPIN2    P1_1
#define OUTPIN3    P1_2
#define OUTPIN4    P1_3
#define OUTPIN5    P1_4
#define BOOT       P3_7

#define SYSCLK 72000000L // SYSCLK frequency in Hz
#define BAUDRATE 115200L
#define SARCLK 18000000L
#define RELOAD_10us (0x10000L-(SYSCLK/(12L*100000L))) // 10us rate

char _c51_external_startup (void)
{
	// Disable Watchdog with key sequence
	SFRPAGE = 0x00;
	WDTCN = 0xDE; //First key
	WDTCN = 0xAD; //Second key
  
	VDM0CN |= 0x80;
	RSTSRC = 0x02;

	#if (SYSCLK == 48000000L)	
		SFRPAGE = 0x10;
		PFE0CN  = 0x10; // SYSCLK < 50 MHz.
		SFRPAGE = 0x00;
	#elif (SYSCLK == 72000000L)
		SFRPAGE = 0x10;
		PFE0CN  = 0x20; // SYSCLK < 75 MHz.
		SFRPAGE = 0x00;
	#endif
	
	#if (SYSCLK == 12250000L)
		CLKSEL = 0x10;
		CLKSEL = 0x10;
		while ((CLKSEL & 0x80) == 0);
	#elif (SYSCLK == 24500000L)
		CLKSEL = 0x00;
		CLKSEL = 0x00;
		while ((CLKSEL & 0x80) == 0);
	#elif (SYSCLK == 48000000L)	
		// Before setting clock to 48 MHz, must transition to 24.5 MHz first
		CLKSEL = 0x00;
		CLKSEL = 0x00;
		while ((CLKSEL & 0x80) == 0);
		CLKSEL = 0x07;
		CLKSEL = 0x07;
		while ((CLKSEL & 0x80) == 0);
	#elif (SYSCLK == 72000000L)
		// Before setting clock to 72 MHz, must transition to 24.5 MHz first
		CLKSEL = 0x00;
		CLKSEL = 0x00;
		while ((CLKSEL & 0x80) == 0);
		CLKSEL = 0x03;
		CLKSEL = 0x03;
		while ((CLKSEL & 0x80) == 0);
	#else
		#error SYSCLK must be either 12250000L, 24500000L, 48000000L, or 72000000L
	#endif
	
	// Configure the pins used as outputs
	P1MDOUT |= 0b_1101_1111; // SERVO2, SERVO1, OUPTUT1 to OUTPUT5
	P0MDOUT |= 0b_0001_0000; // Configure UART0 TX (P0.4) as push-pull output
	XBR0     = 0x01; // Enable UART0 on P0.4(TX) and P0.5(RX)                     
	XBR1     = 0X00; // 
	XBR2     = 0x40; // Enable crossbar and weak pull-ups

	#if (((SYSCLK/BAUDRATE)/(2L*12L))>0xFFL)
		#error Timer 0 reload value is incorrect because (SYSCLK/BAUDRATE)/(2L*12L) > 0xFF
	#endif
	// Configure Uart 0
	SCON0 = 0x10;
	CKCON0 |= 0b_0000_0000 ; // Timer 1 uses the system clock divided by 12.
	TH1 = 0x100-((SYSCLK/BAUDRATE)/(2L*12L));
	TL1 = TH1;      // Init Timer1
	TMOD &= ~0xf0;  // TMOD: timer 1 in 8-bit auto-reload
	TMOD |=  0x20;                       
	TR1 = 1; // START Timer1
	TI = 1;  // Indicate TX0 ready

	// Initialize timer 5 for periodic interrupts
	SFRPAGE=0x10;
	TMR5CN0=0x00;
	TMR5=0xffff;   // Set to reload immediately
	EIE2|=0b_0000_1000; // Enable Timer5 interrupts
	TR5=1;         // Start Timer5 (TMR5CN0 is bit addressable)
	
	EA=1;
	
	SFRPAGE=0x00;
	
	return 0;
}

void InitADC (void)
{
	SFRPAGE = 0x00;
	ADEN=0; // Disable ADC
	
	ADC0CN1=
		(0x2 << 6) | // 0x0: 10-bit, 0x1: 12-bit, 0x2: 14-bit
        (0x0 << 3) | // 0x0: No shift. 0x1: Shift right 1 bit. 0x2: Shift right 2 bits. 0x3: Shift right 3 bits.		
		(0x0 << 0) ; // Accumulate n conversions: 0x0: 1, 0x1:4, 0x2:8, 0x3:16, 0x4:32
	
	ADC0CF0=
	    ((SYSCLK/SARCLK) << 3) | // SAR Clock Divider. Max is 18MHz. Fsarclk = (Fadcclk) / (ADSC + 1)
		(0x0 << 2); // 0:SYSCLK ADCCLK = SYSCLK. 1:HFOSC0 ADCCLK = HFOSC0.
	
	ADC0CF1=
		(0 << 7)   | // 0: Disable low power mode. 1: Enable low power mode.
		(0x1E << 0); // Conversion Tracking Time. Tadtk = ADTK / (Fsarclk)
	
	ADC0CN0 =
		(0x0 << 7) | // ADEN. 0: Disable ADC0. 1: Enable ADC0.
		(0x0 << 6) | // IPOEN. 0: Keep ADC powered on when ADEN is 1. 1: Power down when ADC is idle.
		(0x0 << 5) | // ADINT. Set by hardware upon completion of a data conversion. Must be cleared by firmware.
		(0x0 << 4) | // ADBUSY. Writing 1 to this bit initiates an ADC conversion when ADCM = 000. This bit should not be polled to indicate when a conversion is complete. Instead, the ADINT bit should be used when polling for conversion completion.
		(0x0 << 3) | // ADWINT. Set by hardware when the contents of ADC0H:ADC0L fall within the window specified by ADC0GTH:ADC0GTL and ADC0LTH:ADC0LTL. Can trigger an interrupt. Must be cleared by firmware.
		(0x0 << 2) | // ADGN (Gain Control). 0x0: PGA gain=1. 0x1: PGA gain=0.75. 0x2: PGA gain=0.5. 0x3: PGA gain=0.25.
		(0x0 << 0) ; // TEMPE. 0: Disable the Temperature Sensor. 1: Enable the Temperature Sensor.

	ADC0CF2= 
		(0x0 << 7) | // GNDSL. 0: reference is the GND pin. 1: reference is the AGND pin.
		(0x1 << 5) | // REFSL. 0x0: VREF pin (external or on-chip). 0x1: VDD pin. 0x2: 1.8V. 0x3: internal voltage reference.
		(0x1F << 0); // ADPWR. Power Up Delay Time. Tpwrtime = ((4 * (ADPWR + 1)) + 2) / (Fadcclk)
	
	ADC0CN2 =
		(0x0 << 7) | // PACEN. 0x0: The ADC accumulator is over-written.  0x1: The ADC accumulator adds to results.
		(0x0 << 0) ; // ADCM. 0x0: ADBUSY, 0x1: TIMER0, 0x2: TIMER2, 0x3: TIMER3, 0x4: CNVSTR, 0x5: CEX5, 0x6: TIMER4, 0x7: TIMER5, 0x8: CLU0, 0x9: CLU1, 0xA: CLU2, 0xB: CLU3

	ADEN=1; // Enable ADC
}

void InitPinADC (unsigned char portno, unsigned char pin_num)
{
	unsigned char mask;
	
	mask=1<<pin_num;

	SFRPAGE = 0x20;
	switch (portno)
	{
		case 0:
			P0MDIN &= (~mask); // Set pin as analog input
			P0SKIP |= mask; // Skip Crossbar decoding for this pin
		break;
		case 1:
			P1MDIN &= (~mask); // Set pin as analog input
			P1SKIP |= mask; // Skip Crossbar decoding for this pin
		break;
		case 2:
			P2MDIN &= (~mask); // Set pin as analog input
			P2SKIP |= mask; // Skip Crossbar decoding for this pin
		break;
		default:
		break;
	}
	SFRPAGE = 0x00;
}

unsigned int ADC_at_Pin(unsigned char pin)
{
	ADC0MX = pin;   // Select input from pin
	ADINT = 0;
	ADBUSY = 1;     // Convert voltage at the pin
	while (!ADINT); // Wait for conversion to complete
	return (ADC0);
}

void Timer5_ISR (void) interrupt INTERRUPT_TIMER5
{
	SFRPAGE=0x10;
	TF5H = 0; // Clear Timer5 interrupt flag
	TMR5RL=RELOAD_10us;
	servo_counter++;
	if(servo_counter==2000)
	{
		servo_counter=0;
	}
	if(servo1>=servo_counter)
	{
		SERVO1=1;
	}
	else
	{
		SERVO1=0;
	}
	if(servo2>=servo_counter)
	{
		SERVO2=1;
	}
	else
	{
		SERVO2=0;
	}
}

// Uses Timer3 to delay <us> micro-seconds. 
void Timer3us(unsigned char us)
{
	unsigned char i;               // usec counter
	
	// The input for Timer 3 is selected as SYSCLK by setting T3ML (bit 6) of CKCON0:
	CKCON0|=0b_0100_0000;
	
	TMR3RL = (-(SYSCLK)/1000000L); // Set Timer3 to overflow in 1us.
	TMR3 = TMR3RL;                 // Initialize Timer3 for first overflow
	
	TMR3CN0 = 0x04;                 // Sart Timer3 and clear overflow flag
	for (i = 0; i < us; i++)       // Count <us> overflows
	{
		while (!(TMR3CN0 & 0x80));  // Wait for overflow
		TMR3CN0 &= ~(0x80);         // Clear overflow indicator
	}
	TMR3CN0 = 0 ;                   // Stop Timer3 and clear overflow flag
}

void waitms (unsigned int ms)
{
	unsigned int j;
	for(j=ms; j!=0; j--)
	{
		Timer3us(249);
		Timer3us(249);
		Timer3us(249);
		Timer3us(250);
	}
}

// Measure the period of a square signal at PERIOD_PIN
unsigned long GetPeriod (int n)
{
	unsigned int overflow_count;
	unsigned char i;
	
	TR0=0; // Stop Timer/Counter 0
	TMOD&=0b_1111_0000; // Set the bits of Timer/Counter 0 to zero
	TMOD|=0b_0000_0001; // Timer/Counter 0 used as a 16-bit timer

	// Reset the counter
	TR0=0;
	TL0=0; TH0=0; TF0=0; overflow_count=0;
	TR0=1;
	while(PERIOD_PIN!=0) // Wait for the signal to be zero
	{
		if(TF0==1) // Did the 16-bit timer overflow?
		{
			TF0=0;
			overflow_count++;
			if(overflow_count==10) // If it overflows too many times assume no signal is present
			{
				TR0=0;
				return 0; // No signal
			}
		}
	}
	
	// Reset the counter
	TR0=0;
	TL0=0; TH0=0; TF0=0; overflow_count=0;
	TR0=1;
	while(PERIOD_PIN!=1) // Wait for the signal to be one
	{
		if(TF0==1) // Did the 16-bit timer overflow?
		{
			TF0=0;
			overflow_count++;
			if(overflow_count==10) // If it overflows too many times assume no signal is present
			{
				TR0=0;
				return 0; // No signal
			}
		}
	}
	
	// Reset the counter
	TR0=0;
	TL0=0; TH0=0; TF0=0; overflow_count=0;
	TR0=1; // Start the timer
	for(i=0; i<n; i++) // Measure the time of 'n' periods
	{
		while(PERIOD_PIN!=0) // Wait for the signal to be zero
		{
			if(TF0==1) // Did the 16-bit timer overflow?
			{
				TF0=0;
				overflow_count++;
			}
		}
		while(PERIOD_PIN!=1) // Wait for the signal to be one
		{
			if(TF0==1) // Did the 16-bit timer overflow?
			{
				TF0=0;
				overflow_count++;
			}
		}
	}
	TR0=0; // Stop timer 0, the 24-bit number [overflow_count-TH0-TL0] has the period in clock cycles!
	
	return (overflow_count*65536+TH0*256+TL0);
}

void eputs(char *String)
{	
	while(*String)
	{
		putchar(*String);
		String++;
	}
}

void PrintNumber(long int val, int Base, int digits)
{ 
	code const char HexDigit[]="0123456789ABCDEF";
	int j;
	#define NBITS 32
	xdata char buff[NBITS+1];
	buff[NBITS]=0;
	
	if(val<0)
	{
		putchar('-');
		val*=-1;
	}

	j=NBITS-1;
	while ( (val>0) | (digits>0) )
	{
		buff[j--]=HexDigit[val%Base];
		val/=Base;
		if(digits!=0) digits--;
	}
	eputs(&buff[j+1]);
}

// WARNING: do not use printf().  It makes the program big and slow!

void main (void)
{
    long int j, v;
	long int count, f;
	unsigned char LED_toggle=0; // Used to test the outputs
	
	InitPinADC(2, 2); // Configure P2.2 as analog input
	InitPinADC(2, 3); // Configure P2.3 as analog input
	InitADC();
    
    waitms(1000); // Wait a second to give PuTTy a chance to start
    
	eputs("\x1b[2J\x1b[1;1H"); // Clear screen using ANSI escape sequence.

	eputs("\r\nEFM8LB12 multi I/O example.\r\n");
	eputs("Measures the voltage from pins P2.2 and P2.3\r\n");
	eputs("Measures period on P1.5\r\n");
	eputs("Toggles pins P1.0, P1.1, P1.2, P1.3, P1.4\r\n");
	eputs("Generates servo PWMs on P1.6 and P1.7\r\n");
	eputs("Reads the BOOT push-button on pin P3.7\r\n\r\n");
		
	OUTPIN1=0;
	OUTPIN2=0;
	OUTPIN3=0;
	OUTPIN4=0;
	OUTPIN5=0;
	
	while (1)
	{
		j=ADC_at_Pin(QFP32_MUX_P2_2);
		v=(j*33000)/0x3fff;
		eputs("ADC[P2.2]=0x");
		PrintNumber(j, 16, 4);
		eputs(", ");
		PrintNumber(v/10000, 10, 1);
		putchar('.');
		PrintNumber(v%10000, 10, 4);
		eputs("V ");;

		j=ADC_at_Pin(QFP32_MUX_P2_3);
		v=(j*33000)/0x3fff;
		eputs("ADC[P2.3]=0x");
		PrintNumber(j, 16, 4);
		eputs(", ");
		PrintNumber(v/10000, 10, 1);
		putchar('.');
		PrintNumber(v%10000, 10, 4);
		eputs("V ");
		
		eputs("BOOT(P3.7)=");
		if(BOOT)
		{
			eputs("1 ");
		}
		else
		{
			eputs("0 ");
		}

		// Not very good for high frequencies because of all the interrupts in the background
		// but decent for low frequencies around 10kHz.
		count=GetPeriod(30);
		if(count>0)
		{
			f=(SYSCLK*30.0)/(count*12);
			eputs("f=");
			PrintNumber(f, 10, 7);
			eputs("Hz, count=");
			PrintNumber(count, 10, 8);
			eputs("          \r");
		}
		else
		{
			eputs("NO SIGNAL                     \r");
		}

		// Now turn on one of outputs per cycle to check
		switch (LED_toggle++)
		{
			case 0:
				OUTPIN1=1;
				break;
			case 1:
				OUTPIN2=1;
				break;
			case 2:
				OUTPIN3=1;
				break;
			case 3:
				OUTPIN4=1;
				break;
			case 4:
				OUTPIN5=1;
				break;
			default:
				OUTPIN1=0;
				OUTPIN2=0;
				OUTPIN3=0;
				OUTPIN4=0;
				OUTPIN5=0;
				LED_toggle=0;
				break;
		}
		
		// Change the servo PWM signals
		if (servo1<200)
		{
			servo1++;
		}
		else
		{
			servo1=100;	
		}

		if (servo2>100)
		{
			servo2--;
		}
		else
		{
			servo2=200;	
		}
		
		waitms(200);	
	}
}

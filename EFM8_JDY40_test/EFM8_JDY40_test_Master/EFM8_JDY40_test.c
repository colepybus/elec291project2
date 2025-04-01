#include <EFM8LB1.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// comment!

#define SYSCLK 72000000
#define BAUDRATE 115200L
#define SARCLK 18000000L
#define SCALE_LED 450
#define BASE 183000
#define DEFAULT_F 15500L

#define JDY40_SET_PIN P1_4

#define LCD_RS P1_7
// #define LCD_RW Px_x // Not used in this code.  Connect to GND
#define LCD_E  P2_0
#define LCD_D4 P1_3
#define LCD_D5 P1_2
#define LCD_D6 P1_1
#define LCD_D7 P1_0
#define CHARS_PER_LINE 16	

idata char buff[20];
int actual_mode = 0;

char _c51_external_startup (void)
{
	// Disable Watchdog with key sequence
	SFRPAGE = 0x00;
	WDTCN = 0xDE; //First key
	WDTCN = 0xAD; //Second key
  
	VDM0CN=0x80;       // enable VDD monitor
	RSTSRC=0x02|0x04;  // Enable reset on missing clock detector and VDD

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
	
	P0MDOUT |= 0x1D; // Enable UART0 TX (P0.4) and UART1 TX (P0.0) as push-pull outputs changed from P0MDOUT |= 0x1D
	// P2MDOUT |= 0x01; // P2.0 in push-pull mode
	P1MDOUT |= 0x10;  // Set P1.4 as push-pull output
	P2MDOUT |= 0b_00000010; //P2.1 as push-pull output
    P1 |= 0x10;       // Start with JDY40 in normal mode (SET high)
	XBR0     = 0x01; // Enable UART0 on P0.4(TX) and P0.5(RX)                     
	XBR1     = 0X00;
	XBR2     = 0x41; // Enable crossbar and uart 1

	// Configure Uart 0
	#if (((SYSCLK/BAUDRATE)/(2L*12L))>0xFFL)
		#error Timer 0 reload value is incorrect because (SYSCLK/BAUDRATE)/(2L*12L) > 0xFF
	#endif
	SCON0 = 0x10;
	TH1 = 0x100-((SYSCLK/BAUDRATE)/(2L*12L));
	TL1 = TH1;      // Init Timer1
	TMOD &= ~0xf0;  // TMOD: timer 1 in 8-bit auto-reload
	TMOD |=  0x20;                       
	TR1 = 1; // START Timer1
	TI = 1;  // Indicate TX0 ready
  	
	TMR2CN0=0x00;   // Stop Timer2; Clear TF2;
	CKCON0|=0b_0001_0000; // Timer 2 uses the system clock
	//TMR2RL=(0x10000L-(SYSCLK/(2*TIMER_2_FREQ))); // Initialize reload value
	//TMR2=0xffff;   // Set to reload immediately
	//sketchy
	TMR2RL=(-(SYSCLK/(2*DEFAULT_F))); // Initialize reload value
	TMR2=0xffff;   // Set to reload immediately
	ET2=1;         // Enable Timer2 interrupts
	TR2=1;         // Start Timer2 (TMR2CN is bit addressable)

	//sketchy
	EA = 1;

	return 0;
}

void Timer2_ISR (void) interrupt INTERRUPT_TIMER2
{
    TF2H = 0;           // Clear interrupt flag
    P2_1 = !P2_1;       // Toggle the speaker pin
}

void setSpeakerFrequency(unsigned int input_val)
{
    unsigned long int f;
    int freq_out;

	if (input_val == 0) {
		TR2 = 0;
		return;
	}

	freq_out = (input_val / 2) - 90000;
	printf("freq_out: %d", freq_out);
	if (freq_out < 1) freq_out = 1;

    // 3. Stop Timer2, load new reload, and restart
    TR2 = 0;  // Stop Timer2
    TMR2RL = 0x10000L - (SYSCLK / (2L * freq_out));
    TR2 = 1;        // Start Timer2
	f=SYSCLK/(2L*(0x10000L-TMR2RL));
	printf("\nActual frequency: %lu\n", f);
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
	unsigned char k;
	for(j=0; j<ms; j++)
		for (k=0; k<4; k++) Timer3us(250);
}

void LCD_pulse (void)
{
	LCD_E=1;
	Timer3us(40);
	LCD_E=0;
}

void LCD_byte (unsigned char x)
{
	// The accumulator in the C8051Fxxx is bit addressable!
	ACC=x; //Send high nible
	LCD_D7=ACC_7;
	LCD_D6=ACC_6;
	LCD_D5=ACC_5;
	LCD_D4=ACC_4;
	LCD_pulse();
	Timer3us(40);
	ACC=x; //Send low nible
	LCD_D7=ACC_3;
	LCD_D6=ACC_2;
	LCD_D5=ACC_1;
	LCD_D4=ACC_0;
	LCD_pulse();
}

void WriteData (unsigned char x)
{
	LCD_RS=1;
	LCD_byte(x);
	waitms(2);
}

void WriteCommand (unsigned char x)
{
	LCD_RS=0;
	LCD_byte(x);
	waitms(5);
}

void LCD_4BIT (void)
{
	LCD_E=0; // Resting state of LCD's enable is zero
	// LCD_RW=0; // We are only writing to the LCD in this program
	waitms(20);
	// First make sure the LCD is in 8-bit mode and then change to 4-bit mode
	WriteCommand(0x33);
	WriteCommand(0x33);
	WriteCommand(0x32); // Change to 4-bit mode

	// Configure the LCD
	WriteCommand(0x28);
	WriteCommand(0x0c);
	WriteCommand(0x01); // Clear screen command (takes some time)
	waitms(20); // Wait for clear screen command to finsih.
}

void LCDprint(char * string, unsigned char line, bit clear)
{
	int j;

	WriteCommand(line==2?0xc0:0x80);
	waitms(5);
	for(j=0; string[j]!=0; j++)	WriteData(string[j]);// Write the message
	if(clear) for(; j<CHARS_PER_LINE; j++) WriteData(' '); // Clear the rest of the line
}

void UART1_Init (unsigned long baudrate)
{
    SFRPAGE = 0x20;
	SMOD1 = 0x0C; // no parity, 8 data bits, 1 stop bit
	SCON1 = 0x10;
	SBCON1 =0x00;   // disable baud rate generator
	SBRL1 = 0x10000L-((SYSCLK/baudrate)/(12L*2L));
	TI1 = 1; // indicate ready for TX
	SBCON1 |= 0x40;   // enable baud rate generator
	SFRPAGE = 0x00;
}

void putchar1 (char c) 
{
    SFRPAGE = 0x20;
	while (!TI1);
	TI1=0;
	SBUF1 = c;
	SFRPAGE = 0x00;
}

void sendstr1 (char * s)
{
	while(*s)
	{
		putchar1(*s);
		s++;	
	}
}

char getchar1 (void)
{
	char c;
    SFRPAGE = 0x20;
	while (!RI1);
	RI1=0;
	// Clear Overrun and Parity error flags 
	SCON1&=0b_0011_1111;
	c = SBUF1;
	SFRPAGE = 0x00;
	return (c);
}

char getchar1_with_timeout (void)
{
	char c;
	unsigned int timeout;
    SFRPAGE = 0x20;
    timeout=0;
	while (!RI1)
	{
		SFRPAGE = 0x00;
		Timer3us(20);
		SFRPAGE = 0x20;
		timeout++;
		if(timeout==25000)
		{
			SFRPAGE = 0x00;
			return ('\n'); // Timeout after half second
		}
	}
	RI1=0;
	// Clear Overrun and Parity error flags 
	SCON1&=0b_0011_1111;
	c = SBUF1;
	SFRPAGE = 0x00;
	return (c);
}

void getstr1 (char * s, unsigned char n)
{
	char c;
	unsigned char cnt;
	
	cnt=0;
	while(1)
	{
		c=getchar1_with_timeout();
		if(c=='\n')
		{
			*s=0;
			return;
		}
		
		if (cnt<n)
		{
			cnt++;
			*s=c;
			s++;
		}
		else
		{
			*s=0;
			return;
		}
	}
}

// RXU1 returns '1' if there is a byte available in the receive buffer of UART1
bit RXU1 (void)
{
	bit mybit;
    SFRPAGE = 0x20;
	mybit=RI1;
	SFRPAGE = 0x00;
	return mybit;
}

void waitms_or_RI1 (unsigned int ms)
{
	unsigned int j;
	unsigned char k;
	for(j=0; j<ms; j++)
	{
		for (k=0; k<4; k++)
		{
			if(RXU1()) return;
			Timer3us(250);
		}
	}
}

void SendATCommand (char * s)
{
	printf("Command: %s", s);
	JDY40_SET_PIN = 0;
	//P2_0=0; // 'set' pin to 0 is 'AT' mode.
	waitms(5);
	sendstr1(s);
	getstr1(buff, sizeof(buff)-1);
	waitms(10);
	JDY40_SET_PIN = 1;
	//P2_0=1; // 'set' pin to 1 is normal operation mode.
	printf("Response: %s\r\n", buff);
}

void ReceptionOff (void)
{
	//P2_0=0; // 'set' pin to 0 is 'AT' mode.
	JDY40_SET_PIN = 0;
	waitms(10);
	sendstr1("AT+DVID0000\r\n"); // Some unused id, so that we get nothing in RXD1.
	waitms(10);
	// Clear Overrun and Parity error flags 
	SCON1&=0b_0011_1111;
	JDY40_SET_PIN = 0;
	//P2_0=1; // 'set' pin to 1 is normal operation mode.
}

//joystick functions
#define VDD 3.300 // The measured value of VDD in volts
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

float Volts_at_Pin(unsigned char pin)
{
	 return ((ADC_at_Pin(pin)*VDD)/16383.0);
}

void InitPushButton(void)
{
    SFRPAGE = 0x20;  // Switch to Port Configuration Page
    P3MDOUT &= ~(1 << 2); // Set P3.2 as open-drain (input mode)
    P3 |= (1 << 2);  // Enable internal pull-up resistor => p3.2
	P3 |= (1 << 0); // P3.0
	P3 |= (1 << 1); // P3.1
    SFRPAGE = 0x00;  // Restore SFRPAGE
}

// LED BONUS FUNCTION -----------------------------------------------------------------------------------------------------------------------------------------------

// Mesaures how much higher the metal detector is reading vs. it's base reading. 
// This reading is then converted into a range which determines how many LEDs are
// turned on.

// Input parameters: 
// base_count - base level deteciong intensity 
// count - present detection intensity 

void LED_scale(int count) {
    // 1 light on
    if (abs(BASE - count) < SCALE_LED) {
        P0_2 = 0;
        P0_3 = 0;
        P0_4 = 1;
    }
    // 2 lights on
    else if ((abs(BASE - count) >= SCALE_LED) && (abs(BASE - count) < 2*SCALE_LED)) {
        P0_2 = 0;
        P0_3 = 1;
        P0_4 = 0;
    }
    // 3 lights on 
    else if ((abs(BASE - count) >= 2*SCALE_LED) && (abs(BASE - count) < 3*SCALE_LED)) {
        P0_2 = 0;
        P0_3 = 1;
        P0_4 = 1;
    }
    // 4 lights on 
    else if ((abs(BASE - count) >= 3*SCALE_LED) && (abs(BASE - count) < 4*SCALE_LED)) {
        P0_2 = 1;
        P0_3 = 0;
        P0_4 = 0;
    }
    // 5 lights on 
    else if ((abs(BASE - count) >= 4*SCALE_LED) && (abs(BASE - count) < 5*SCALE_LED)) {
        P0_2 = 1;
        P0_3 = 0;
        P0_4 = 1;
    }
    // 6 lights on 
    else if ((abs(BASE - count) >=5*SCALE_LED) && (abs(BASE - count) < 6*SCALE_LED)) {
        P0_2 = 1;
        P0_3 = 1;
        P0_4 = 0;
    }
    // 7 lights on 
    else if (abs(BASE - count) >= 6*SCALE_LED) {
        P0_2 = 1;
        P0_3 = 1;
        P0_4 = 1;
    }
}

/*
void LED_scale(int count) {
    // 1 light on
    if (abs(BASE - count) < SCALE_LED) {
        P0_2 = 0;
        P0_3 = 0;
        P0_4 = 1;
    }
    // 2 lights on
    else if (abs(BASE - count) < 2*SCALE_LED) {
        P0_2 = 0;
        P0_3 = 1;
        P0_4 = 0;
    }
    // 3 lights on 
    else if (abs(BASE - count) < 3*SCALE_LED) {
        P0_2 = 0;
        P0_3 = 1;
        P0_4 = 1;
    }
    // 4 lights on 
    else if (abs(BASE - count) < 4*SCALE_LED) {
        P0_2 = 1;
        P0_3 = 0;
        P0_4 = 0;
    }
    // 5 lights on 
    else if (abs(BASE - count) < 5*SCALE_LED) {
        P0_2 = 1;
        P0_3 = 0;
        P0_4 = 1;
    }
    // 6 lights on 
    else if (abs(BASE - count) < 6*SCALE_LED) {
        P0_2 = 1;
        P0_3 = 1;
        P0_4 = 0;
    }
    // 7 lights on 
    else {
        P0_2 = 1;
        P0_3 = 1;
        P0_4 = 1;
    }
} */

#define G5 784
#define A5 880
#define B5 988
#define C6 1047
#define D6 1175
#define E6 1316

void songSpeakerFrequency(unsigned int input_val)
{
    unsigned long int f;

    // 3. Stop Timer2, load new reload, and restart
    TR2 = 0;  // Stop Timer2
    TMR2RL = 0x10000L - (SYSCLK / (2L * input_val));
    TR2 = 1;        // Start Timer2
	f=SYSCLK/(2L*(0x10000L-TMR2RL));
	printf("\nActual frequency: %lu\n", f);
}

void play_song() {
    // Stop current playback
    TR2 = 0; 
    
    songSpeakerFrequency(G5);
    waitms(100); 
    songSpeakerFrequency(A5);
    waitms(100); 
    songSpeakerFrequency(C6);
    waitms(100); 
    songSpeakerFrequency(A5);
    waitms(100); 
    songSpeakerFrequency(E6);
    waitms(300); 
    songSpeakerFrequency(E6);
    waitms(300);
    songSpeakerFrequency(D6);
    waitms(600); 
    
    songSpeakerFrequency(G5);
    waitms(100); 
    songSpeakerFrequency(A5);
    waitms(100); 
    songSpeakerFrequency(C6);
    waitms(100); 
    songSpeakerFrequency(A5);
    waitms(100);
    songSpeakerFrequency(D6);
    waitms(300); 
    songSpeakerFrequency(D6);
    waitms(300); 
    songSpeakerFrequency(C6);
    waitms(600); 
    
    songSpeakerFrequency(G5);
    waitms(100); 
    songSpeakerFrequency(A5);
    waitms(100); 
    songSpeakerFrequency(C6);
    waitms(100); 
    songSpeakerFrequency(A5);
    waitms(100);
    songSpeakerFrequency(C6);
    waitms(300); 
    songSpeakerFrequency(D6);
    waitms(300); 
    songSpeakerFrequency(B5);
    waitms(300); 
    songSpeakerFrequency(A5);
    waitms(100); 
    songSpeakerFrequency(G5);
    waitms(500);
    
    
    songSpeakerFrequency(A5);
    waitms(200); 
    songSpeakerFrequency(D6);
    waitms(400); 
    songSpeakerFrequency(C6);
    waitms(400); 
   
    // Stop playback after song
    TR2 = 0;
}

void main (void)
{
    int timeout_cnt=0;
    int cont1=0, cont2=100;
	
	float v[4];
	float norm_x;
	float norm_y;
	int mode = 0;
	int freq = 0;
	int freq_int; 

	bit button_state;
	bit button_1_state;
	bit button_2_state;
	
	LCD_4BIT();
	
	waitms(500);
	printf("\x1b[2J"); // Clear screen using ANSI escape sequence.
	//printf("\r\nEFM8LB12 JDY-40 Master Test.\r\n");
	UART1_Init(9600);

	// To configure the device (shown here using default values).
	// For some changes to take effect, the JDY-40 needs to be power cycled.
	// Communication can only happen between devices with the
	// same RFID and DVID in the same channel.
	
	//SendATCommand("AT+BAUD4\r\n");
	//SendATCommand("AT+RFID8899\r\n");
	//SendATCommand("AT+DVID1122\r\n"); // Default device ID.
	//SendATCommand("AT+RFC001\r\n");
	//SendATCommand("AT+POWE9\r\n");
	//SendATCommand("AT+CLSSA0\r\n");
	
	ReceptionOff();

	// To check configuration
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

	InitPinADC(2, 2); // Configure P2.2 as analog input
	InitPinADC(2, 3); // Configure P2.3 as analog input
	InitPinADC(2, 4); // Configure P2.4 as analog input
	InitPinADC(2, 5); // Configure P2.5 as analog input
    InitADC();
	InitPushButton();

	
	
	//LCDprint("Frequency", 1,8);

	
	while(1)
	{
		
		//sprintf(buff, "%03d,%03d\n", cont1, cont2); // Construct a test message
		putchar1('!'); // Send a message to the slave. First send the 'attention' character which is '!'
		// Wait a bit so the slave has a chance to get ready
		waitms(5); // This may need adjustment depending on how busy is the slave
		//sendstr1(buff); // Send the test message
		
		//sending stuff to slave regardless of whether smth has come back
		   // Read 14-bit value from the pins configured as analog inputs
		   v[0] = Volts_at_Pin(QFP32_MUX_P2_2);
		   v[1] = Volts_at_Pin(QFP32_MUX_P2_3);
		   v[2] = Volts_at_Pin(QFP32_MUX_P2_4);
		   v[3] = Volts_at_Pin(QFP32_MUX_P2_5);

		   norm_x = (v[1] / 3.294) * 2.0 - 1.0;  // Horizontal (P2.3)
		   norm_y = (v[0] / 3.294) * 2.0 - 1.0;  // Vertical   (P2.2)

		   button_state = (P3 & (1 << 2)) ? 0 : 1;
		   
		   button_1_state = (P3 & (1 << 0)) ? 0 : 1; // If HIGH, button not pressed; If LOW, button pressed
		   button_2_state = (P3 & (1 << 1)) ? 0 : 1; 

		   if (button_1_state == 1) {
			   //printf("button 1 pressed. switch to automatic mode");
			   mode = 5;
			   actual_mode = 1; // automatic
			   TR2 = 0;  // turn off speaker 
			   LCDprint("Automatic", 2, 1);

		   }

		   else if (button_2_state == 1) {
			   //printf("button 2 pressed. switch to manual mode");
			   mode = 6;
			   play_song(); 
		   }
		   
		   else if(button_state == 1){
		   
		   waitms(500);
		   
			   if(button_state ==1){
			   
			   	printf("joystick button is pressed. pick coin");
			   	mode = 7;
			   	LCDprint("Picking coin", 2, 1);
			   }
		   }

		   // test with diagonal joystick control 
		   else if (norm_x <= 1.5 && norm_x > 0.5) //right
		   {
			   mode = 4;
			   LCDprint("Moving Right", 2, 1);
		   }

		   else if (norm_x <-0.5 && norm_x>= -1.5) //left
		   {
			   mode = 3;
			   LCDprint("Moving Left", 2, 1);
		   }
		   
		   else if (norm_y <= 1.5 && norm_y > 0.5) // forward
		   {
			   mode = 1;
			   LCDprint("Moving Front", 2, 1);
		   }

		   else if (norm_y <-0.5 && norm_y>= -1.5) //backwards
		   {
			   mode = 2;
			   LCDprint("Moving Back", 2, 1);
		   }
		   else
		   {
			   mode = 0;
		   } 

		   sprintf(buff, "test= %01d\n", mode);
		   sendstr1(buff);

		waitms(5); // This may need adjustment depending on how busy is the slave
		
		putchar1('@'); // Request a message from the slave
		
		timeout_cnt=0;
		

		while(1)
		{
			if(RXU1()) break; // Something has arrived
			if(++timeout_cnt>250) break; // Wait up to 25ms for the repply
			Timer3us(100); // 100us*250=25ms
			//printf("thing");
		}
		
		if(RXU1()) // Something has arrived from the slave
		{
			getstr1(buff, sizeof(buff)-1);
			//freq = buff;

			//LCDprint(freq,2,1);
			
			if(strlen(buff)==6) // Check for valid message size (6 characters)
			{
				if (strstr(buff, "DONE")) { 
					play_song();
				}
				else if (actual_mode == 0) {
					//LCDprint("Freq", 1,1);
					char displayF[20];
					sprintf(displayF, "Freq=%s", buff);
					LCDprint(displayF, 1, 1);
					
				    printf("Slave says: %s\r\n", buff);
				    //LCDprint(buff,1,10);
				    freq_int = atoi(buff); 
				    LED_scale(freq_int); 
				    
				   	setSpeakerFrequency(freq_int);
				}
				else {
				 	P2_1 = 0; 
				}  	
			}
			else
			{
				printf("*** BAD MESSAGE ***: %s\r\n", buff);
			}
			
		}
		else // Timed out waiting for reply
		{
			printf("NO RESPONSE\r\n", buff);
		}
	}
}

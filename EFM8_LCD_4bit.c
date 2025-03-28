#include <EFM8LB1.h>
#include <stdio.h>
#include <string.h>    // <-- needed for strtok, atof

//THIS FILE IS WHAT ALLOWS THE EFM8  TO RECIEVE THE LEAPMOTION METRICS VIA SERIAL

#define SYSCLK    72000000L 
#define BAUDRATE    115200L 

#define LCD_RS P1_7
#define LCD_E  P2_0
#define LCD_D4 P1_3
#define LCD_D5 P1_2
#define LCD_D6 P1_1
#define LCD_D7 P1_0
#define CHARS_PER_LINE 16

// ... (All your startup, clock, Timer3us, waitms, etc. code unchanged) ...

void LCD_4BIT (void)
{
	LCD_E=0; 
	waitms(20);
	WriteCommand(0x33);
	WriteCommand(0x33);
	WriteCommand(0x32); // 4-bit mode
	WriteCommand(0x28);
	WriteCommand(0x0c);
	WriteCommand(0x01); 
	waitms(20);
}

void LCDprint(char * string, unsigned char line, bit clear)
{
	int j;
	WriteCommand(line==2 ? 0xc0 : 0x80);
	waitms(5);
	for(j=0; string[j]!=0; j++) WriteData(string[j]);
	if(clear) for(; j<CHARS_PER_LINE; j++) WriteData(' ');
}

int getsn (char * buff, int len)
{
	int j;
	char c;
	for(j=0; j<(len-1); j++)
	{
		c=getchar();
		if ((c=='\n') || (c=='\r'))
		{
			buff[j]=0;
			return j;
		}
		else
		{
			buff[j]=c;
		}
	}
	buff[j]=0;
	return len;
}

void main (void)
{
	char buff[128];
	char pinchbuf[16];
	char displaybuf[CHARS_PER_LINE + 1];

	LCD_4BIT();
	LCDprint("Leap -> EFM8", 1, 1);
	LCDprint("Waiting data...", 2, 1);

	while(1)
	{
		// // Block until we get a full line from UART:
		// getsn(buff, sizeof(buff));

		// //------------------------------------------
		// // 1) Parse out the pinch value (5th field)
		// //------------------------------------------
		// // Incoming format example:
		// //  !61957,1,85.40,Left,0.79,0.00
		// //   ^   1   2     3     4     5     6 fields

		// // Skip leading '!' if present:
		// char *line = buff;
		// if(line[0] == '!') line++;

		// float pinchVal = 0.0f;
		// char *token = strtok(line, ",");
		// int fieldCount = 1;

		// while(token != NULL)
		// {
		// 	if(fieldCount == 5) // The pinch field
		// 	{
		// 		pinchVal = atof(token);
		// 		break;
		// 	}
		// 	token = strtok(NULL, ",");
		// 	fieldCount++;
		// }

		// // Format “Pinch=xx.xx” for line 1
		// sprintf(pinchbuf, "Pinch=%.2f", pinchVal);

		// //------------------------------------------
		// // 2) Display pinch on line 1
		// //------------------------------------------
		// LCDprint("                ", 1, 1);      // Clear line 1
		// LCDprint(pinchbuf, 1, 1);

		// //------------------------------------------
		// // 3) Display the entire incoming string on line 2
		// //------------------------------------------
		// LCDprint("                ", 2, 1);      // Clear line 2
		// LCDprint(buff, 2, 1);

		// Block until we get a full line from UART
        getsn(buff, sizeof(buff));

        // Example output to parse:
        // "Right Hand Grab: 0.500000 | Left Hand Pinch: 0.600000 | Right Hand Turn: 0.200000 | Right Hand Move: 0.300000"
        
        // Parse Left Hand Pinch
        char* leftPinchPos = strstr(buff, "Left Hand Pinch:");
        float leftPinch = 0.0f;
        if (leftPinchPos)
        {
            leftPinch = atof(leftPinchPos + strlen("Left Hand Pinch:"));
        }

        // Parse Right Hand Grab
        char* rightGrabPos = strstr(buff, "Right Hand Grab:");
        float rightGrab = 0.0f;
        if (rightGrabPos)
        {
            rightGrab = atof(rightGrabPos + strlen("Right Hand Grab:"));
        }

        // First line: Left Hand Pinch
        sprintf(displaybuf, "L Pinch: %.2f", leftPinch);
        LCDprint(displaybuf, 1, 1);

        // Second line: Right Hand Grab
        sprintf(displaybuf, "R Grab: %.2f", rightGrab);
        LCDprint(displaybuf, 2, 1);

	}
}
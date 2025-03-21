#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For usleep()

#include <termios.h>
#include <string.h>

#include "ExampleConnection.h"  // Use functions from ExampleConnection.c

int serial_fd = -1; //file descriptor for efm8 usb serial connection

//WORKFLOW for cole: paste in the following command to compile the code -->
//              gcc -o example leapc_main.c ExampleConnection.c -I../include -L../lib -lLeapC -arch arm64
//then run the executable with ./example

void setupEMF8Serial(const char* portName)
{
    serial_fd = open(portName, O_WRONLY | O_NOCTTY);
    if(serial_fd < 0) {
        perror("ERROR opening EFM8 port");
        exit(1);
    }

    struct termios tty;
    tcgetattr(serial_fd, &tty);

    // Set baud rate to match your EFM8 code (9600 in your example)
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // 8-N-1 serial settings
    tty.c_cflag &= ~PARENB;    // No parity
    tty.c_cflag &= ~CSTOPB;    // One stop bit
    tty.c_cflag &= ~CSIZE;     
    tty.c_cflag |= CS8;        // 8 bits
    tty.c_cflag |= CREAD | CLOCAL;

    tcsetattr(serial_fd, TCSANOW, &tty);
    printf("EFM8 USB serial port configured.\n");
}


void sendToEFM8(long long frameID, unsigned int hands, float framerate, const char* handType, float pinch, float grab)
{
    if(serial_fd < 0) return; // Not open
    
    // Example format: !FRAMEID,HANDS,FRAMERATE,HANDTYPE,PINCH,GRAB\n
    // '!' is a start marker for easy parsing on the EFM8
    char buffer[128];
    snprintf(buffer, sizeof(buffer), 
             "!%lld,%u,%.2f,%s,%.2f,%.2f\n", 
             frameID, hands, framerate, handType, pinch, grab);
    
    write(serial_fd, buffer, strlen(buffer));
}


int main() {
    printf("Starting Leap Motion connection...\n");

    // Open Leap Motion Connection
    OpenConnection();

    setupEMF8Serial("/dev/tty.usbserial-D3098FBT");

    while (1) {
        LEAP_TRACKING_EVENT* frame = GetFrame();

        if (frame) {  
            printf("\rFrame ID: %lld | Hands: %u | Framerate: %f", 
                   frame->tracking_frame_id, frame->nHands, frame->framerate);

            // Check if hands are detected
            if (frame->nHands > 0 && frame->pHands) {
                LEAP_HAND* hand = frame->pHands;  // Pointer to the first detected hand
                
                // Get chirality (left or right)
                const char* handType = (hand->type == eLeapHandType_Left) ? "Left" : "Right";

                printf(" | Hand: %s | Pinch Strength: %f | Grab Strength: %f", handType, hand->pinch_strength, hand->grab_strength);

                sendToEFM8(frame->tracking_frame_id, frame->nHands, frame->framerate, handType, pinch, grab);

            }
        } else {
            printf("\rNo hands detected...                     ");
        }

        fflush(stdout);
        usleep(50000);  // Sleep for 50 milliseconds
    }

    CloseConnection();
    return 0;
}

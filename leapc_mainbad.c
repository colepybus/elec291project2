#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // For usleep()
#include <fcntl.h>
#include <termios.h>
#include <string.h>

#include "ExampleConnection.h"  // Use functions from ExampleConnection.c

int serial_fd = -1; //file descriptor for efm8 usb serial connection

// WORKFLOW for cole: compile with:
//   gcc -o example leapc_main.c ExampleConnection.c -I../include -L../lib -lLeapC -arch arm64
// then run with ./example

void setupEMF8Serial(const char* portName)
{
    printf("port function 1\n");
    serial_fd = open(portName, O_WRONLY | O_NOCTTY | O_NONBLOCK);

    printf("port function 0.5\n");
    if (serial_fd < 0) {
        perror("ERROR opening EFM8 port");
        exit(1);
    }

    printf("port function 2\n");
    struct termios tty;
    tcgetattr(serial_fd, &tty); // get current settings

    printf("port function 3\n");
    // Set baud rate to match your EFM8 code 
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    printf("port function 4\n");
    // 8-N-1 serial settings
    tty.c_cflag &= ~PARENB;      // No parity
    tty.c_cflag &= ~CSTOPB;      // One stop bit
    tty.c_cflag &= ~CSIZE;       
    tty.c_cflag |= CS8;          // 8 bits
    tty.c_cflag |= CREAD | CLOCAL;

    printf("port function 5\n");
    tcsetattr(serial_fd, TCSANOW, &tty);
    printf("EFM8 USB serial port configured.\n");
}

void sendToEFM8(float pinch, float grab, float pitch, float roll)
{
    if (serial_fd < 0) return; // Not open

    char buffer[32];
    // Format: "!pinch,grab,pitch,roll\n"
    snprintf(buffer, sizeof(buffer), "!%.2f,%.2f,%.2f,%.2f\n", 
             pinch, grab, pitch, roll);

    printf("[DEBUG] Outgoing: %s", buffer);

    int n = write(serial_fd, buffer, strlen(buffer));
    if (n > 0) {
        printf("[DEBUG] Sent to EFM8: %s\n", buffer);
    } else {
        perror("ERROR writing to serial");
    }
}

int main() {
    printf("Starting Leap Motion connection...\n");

    // 1) Open Leap Motion Connection
    //OpenConnection();
    printf("opened connection\n");

    // 2) Setup EFM8 Serial
    //setupEMF8Serial("/dev/tty.usbserial-D3098FBT");
    printf("tried setting up efm8serial with port\n");

    while (1) {
        LEAP_TRACKING_EVENT* frame = GetFrame();
        if (frame) {  
            // Show basic frame info on a new line:
            printf("\nFrame ID: %lld | Hands: %u | Framerate: %.2f\n",
                   frame->tracking_frame_id, 
                   frame->nHands, 
                   frame->framerate);

            // We ONLY care about sending data if both hands exist:
            if (frame->nHands >= 2) {
                LEAP_HAND* leftHand  = NULL;
                LEAP_HAND* rightHand = NULL;

                // Identify each hand
                for (uint32_t i = 0; i < frame->nHands; i++) {
                    if (frame->pHands[i].type == eLeapHandType_Left) {
                        leftHand = &frame->pHands[i];
                    } else if (frame->pHands[i].type == eLeapHandType_Right) {
                        rightHand = &frame->pHands[i];
                    }
                }

                // If we have both:
                if (leftHand && rightHand) {
                    // Grab = rightHand->grab_strength
                    float grab  = rightHand->grab_strength;
                    // Pinch = leftHand->pinch_strength
                    float pinch = leftHand->pinch_strength;
                    // Pitch & roll from the right hand
                    float pitch = rightHand->palm.normal.x;
                    float roll  = rightHand->palm.normal.z;

                    // Print it out
                    printf("RightHand Grab=%.2f | LeftHand Pinch=%.2f | RightHand Pitch=%.2f | RightHand Roll=%.2f\n",
                           grab, pinch, pitch, roll);

                    // Now send these to EFM8
                    sendToEFM8(pinch, grab, pitch, roll);
                }
                else {
                    // If we do NOT actually have both 
                    printf("Did not find both left and right hands.\n");
                }
            }
            else {
                // If fewer than 2 hands:
                printf("Not enough hands (need >=2)\n");
            }
        } else {
            // No frame => no hands at all
            printf("No hands detected...\n");
        }

        fflush(stdout);
        usleep(50000); // Sleep for 50 ms
    }

    CloseConnection();
    return 0;
}

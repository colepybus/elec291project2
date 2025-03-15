#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For usleep()
#include "ExampleConnection.h"  // Use functions from ExampleConnection.c

//WORKFLOW for cole: paste in the following command to compile the code -->
//              gcc -o example leapc_main.c ExampleConnection.c -I../include -L../lib -lLeapC -arch arm64
//then run the executable with ./example

int main() {
    printf("Starting Leap Motion connection...\n");

    // Open Leap Motion Connection
    OpenConnection();

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

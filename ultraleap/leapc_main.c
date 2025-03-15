#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For usleep()
#include "ExampleConnection.h"  // Use functions from ExampleConnection.c

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

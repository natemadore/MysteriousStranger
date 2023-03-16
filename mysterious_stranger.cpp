#include <stdio.h>
#include <stdlib.h>
#include <Events.h>
#include <Movies.h>
#include <QuickTimeComponents.h>

int main(void)
{
    // Open the .mov file
    FILE* fp = fopen("mysterious-stranger.mov", "rb");
    if (fp == NULL) {
        printf("Error: Unable to open the .mov file.\n");
        return 1;
    }

    // Open the movie in QuickTime
    Movie movie;
    short resRefNum;
    OSErr err = OpenMovieFile("mysterious-stranger.mov", &resRefNum, fsRdPerm);
    if (err != noErr) {
        printf("Error: Unable to find the .mov file.\n");
        return 1;
    }
    err = NewMovieFromFile(&movie, resRefNum, NULL, NULL, 0, NULL);
    if (err != noErr) {
        printf("Error: Unable to open the .mov file in QuickTime.\n");
        return 1;
    }

    // Set the movie to play in full-screen mode
    Rect screenBounds;
    GDHandle mainDevice;
    GetMainDevice(&mainDevice);
    screenBounds = (*mainDevice)->gdRect;
    Rect movieRect = screenBounds;
    SetMovieBox(movie, &movieRect);

    // Set the movie volume to 100%
    SetMovieVolume(movie, 0x0100);

    // Remove the player controls from full-screen mode
    long controlStyle = kQTMovieControlStyleNone;
    SetMovieProperty(movie, kQTMovieControlStyleAttribute, &controlStyle, sizeof(controlStyle));

    // Play the movie on loop until shift + ESC is pressed
    EventRecord event;
    int ch = 0;
    while (ch != 27) { // ASCII code for ESC key
        // Play the movie
        StartMovie(movie);

        // Check if the user pressed shift + ESC
        if (EventAvail(keyDownMask, &event)) {
            char key;
            GetNextEvent(keyDownMask, &event);
            key = (char)event.message & charCodeMask;
            if (event.modifiers & shiftKey && key == 27) {
                break; // Exit the loop
            }
        }
    }

    // Stop the movie and close it
    StopMovie(movie);
    DisposeMovie(movie);

    // Close the .mov file
    fclose(fp);

    return 0;
}

#ifndef LEDMODES_H
#define LEDMODES_H

#include <Arduino.h>
#include <FastLED.h>

class LEDModes {
    public:
        LEDModes(int pin, int numLEDs);  // Constructor

        void begin();   // Initialize the LED strip
        void updateRainbow();  // Updates LEDs with the next color
        void updateIntensity(); // Adjusts LED brightness dynamically
        void updatePulse();
        void setDelay(String ppl_L_R); // Set delay based on input
        int getColorDelay();
        int getHighlightDelay();
        CRGB* leds;

    private:
        int NUM_LEDS;
        int currentIndex;
        int currentHighlight_1;
        int currentHighlight_2;
        int colorDelayTime;    // Delay for color changes
        int highlightDelayTime; // Delay for highlight movement
        int ppl_T;
        int ppl_D;
        unsigned long lastColorUpdate;    // Last update time for colors
        unsigned long lastHighlightUpdate; // Last update time for highlights

        // Add new variables for smooth transitions
        int targetColorDelay;
        int targetHighlightDelay;
        unsigned long transitionStartTime;
        static const int TRANSITION_DURATION = 2000; // Increased to 2 seconds

        // Add variables to store starting delays
        int startColorDelay;
        int startHighlightDelay;

        static const int colorCycle[][3];  // Static array of RGB colors
        static const int numColors;

        unsigned long warningStartTime;    // When the warning animation started
        unsigned long lastInputTime;       // When we last accepted input
        static const int WARNING_DURATION = 3000;  // 3 seconds cooldown
        int warningPos;                    // Position for warning animation
        bool isInWarningState;             // Track if we're in warning state
};

#endif

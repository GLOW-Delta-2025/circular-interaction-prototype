#ifndef LEDMODES_H
#define LEDMODES_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LEDModes {
    public:
        LEDModes(int pin, int numLEDs);  // Constructor

        void begin();   // Initialize the LED strip
        void updateRainbow();  // Updates LEDs with the next color
        void updateIntensity(); // Adjusts LED brightness dynamically
        void setDelay(String ppl_L_R); // Set delay based on input
        int getColorDelay();
        int getHighlightDelay();
        Adafruit_NeoPixel strip;

    private:
        int currentIndex;
        int currentHighlight_1;
        int currentHighlight_2;
        int colorDelayTime;    // Delay for color changes
        int highlightDelayTime; // Delay for highlight movement
        int ppl_T;
        int ppl_D;
        unsigned long lastColorUpdate;    // Last update time for colors
        unsigned long lastHighlightUpdate; // Last update time for highlights

        static const int colorCycle[][3];  // Static array of RGB colors
        static const int numColors;
};

#endif

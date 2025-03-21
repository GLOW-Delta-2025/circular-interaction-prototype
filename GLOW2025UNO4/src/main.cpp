#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "LEDModes.h"

const int LED_PIN = 3;
const int NUM_LEDS = 124;

LEDModes ledCycle(LED_PIN, NUM_LEDS);

String getSerialInput() {
    String input = "";
    unsigned long startTime = millis();
    const unsigned long TIMEOUT = 100; // 100ms timeout    

    char data = Serial.read();
    if (data == '$') {
        // Start collecting data until '#' or timeout
        while (millis() - startTime < TIMEOUT) {
            if (Serial.available()) {
                char c = Serial.read();
                if (c == '#') {
                    Serial.println("Echo: " + input);
                    return input;  // Stop reading at '#'
                }
                input += c;
            }
        }
    }
/*---------------------------------------- BLUE ERROR FEEDBACK ----------------------------------------------------------- */
/**/    for (int i = 0; i < ledCycle.strip.numPixels(); i++) {                                                           /**/
/**/        ledCycle.strip.setPixelColor(i, ledCycle.strip.Color(0, 0, 128));                                            /**/
/**/    }                                                                                                                /**/   
/**/    ledCycle.strip.show();                                                                                           /**/ 
/**/    delay(500);                                                                                                      /**/               
/*-------------------------------------------------------------------------------------------------------------------------*/
    return "";
}

void setup() {
    Serial.begin(115200);
    ledCycle.begin();
    ledCycle.setDelay("3:2"); // Set transition delay (milliseconds)
}

void loop() {
//    ledCycle.updateIntensity();

    if (Serial.available() > 0) {
        String input = getSerialInput();
        if (input.length() > 0) {
            ledCycle.setDelay(input);
        }
    }    
}
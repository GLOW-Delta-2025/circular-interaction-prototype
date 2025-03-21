#include <Arduino.h>
#include <FastLED.h>
#include "LEDModes.h"

const int LED_PIN = 3;
const int NUM_LEDS = 124;

LEDModes ledCycle(LED_PIN, NUM_LEDS);

String getSerialInput() {
    String input = "";
    unsigned long startTime = millis();
    const unsigned long TIMEOUT = 100;

    char data = Serial.read();
    if (data == '$') {
        while (millis() - startTime < TIMEOUT) {
            if (Serial.available()) {
                char c = Serial.read();
                if (c == '#') {
                    Serial.println("Echo: " + input);
                    return input;
                }
                input += c;
            }
        }
    }
    
    return "";
}

void setup() {
    Serial.begin(115200);
    ledCycle.begin();
    ledCycle.setDelay("3:2");
}

void loop() {
    ledCycle.updateIntensity();

    if (Serial.available() > 0) {
        String input = getSerialInput();
        if (input.length() > 0) {
            ledCycle.setDelay(input);
        }
    }    
}
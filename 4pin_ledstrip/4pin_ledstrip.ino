/*
18-03-2025  10:52
Authored by Bas Bakens
*/

/// This code is for a 4 pin LED strip with the following pins:
/// first pin (-->) 12V+
/// second pin  Green LED color
/// third pin   Red   LED color
/// fourth pin  Blue  LED color

// ARDUINO CONSTANT & VARIABLE DEFINITIONS
// -------------------------------------
//   Arduino output pin receiving brightness levels for a specific color
#define RED_PIN 5
#define GREEN_PIN 6
#define BLUE_PIN 3

// =====================================
// ARDUINO SETUP ROUTINE
// -------------------------------------
void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  setLEDS(0, 0, 0);
}

// =====================================
// ARDUINO MAIN LOOP ROUTINE
// -------------------------------------
void loop() {
  setLEDS(255, 255, 255);
}

//    send the LED levels to the Arduino pins
void setLEDS(int ipR, int ipG, int ipB) {
  analogWrite(RED_PIN, ipR);
  analogWrite(GREEN_PIN, ipG);
  analogWrite(BLUE_PIN, ipB);
}

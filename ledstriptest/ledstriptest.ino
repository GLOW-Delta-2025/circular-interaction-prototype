#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN        6
#define NUMPIXELS  65
int colour = 0;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  Serial.begin(9600); 

  pixels.begin();
  pixels.clear();
}

void loop() {
  Serial.println(colour);
  if (Serial.available() > 0)
  {
    colour = Serial.parseInt();

    //Serial.println(255);
    for (int i = 0; i <= NUMPIXELS; i++) {
      pixels.setPixelColor(i, colour, colour, 0);
    }
    pixels.show();
  }
}

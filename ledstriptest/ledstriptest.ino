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

  Serial.begin(115200); 

  pixels.begin();
  pixels.clear();
}

void loop() {
  if (Serial.available() > 0)
  {
    String input = Serial.readStringUntil('\n'); 
    int colour = input.toInt();

    uint16_t hue = map(colour, 0, 255, 0, 65535); // Color shift
     uint8_t brightness = map(colour, 0, 255, 0, 255);

    //Serial.println(255);
    for (int i = 0; i <= NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.ColorHSV(hue, 255, brightness));
    }
    pixels.show();
  }
}

// inclure la bibliothèque nécessaire
#include <Adafruit_NeoPixel.h>

// initialiser l'anneau de leds avec 16 leds connectés sur la pin 6
#define NUMPIXELS 16

Adafruit_NeoPixel ring1 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
// notre anneau est maintenant représenté par l'alias "ring1"

void setup() {
  // intialiser l'anneau
  ring1.begin();
}

void loop() {
  // on répéte une ligne de code pour chaque led
  // c'est à dire de la led à l'index 0 jusqu'à la led 15.
  for (int i = 0 ; i < NUMPIXELS ; i ++) {
    int sat = map(i, 0, NUMPIXELS, 0, 255);
    //uint32_t rgbcolor = ring1.ColorHSV(0, sat,255); // sans correction
    uint32_t rgbcolor = ring1.gamma32(ring1.ColorHSV(0, sat, 255));// avec correction
    ring1.setPixelColor(i, rgbcolor);
  }
  ring1.show();
}

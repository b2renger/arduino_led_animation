
#include <Tween.h>
#include <Adafruit_NeoPixel.h>

// initialisation des leds
#define PIN        6
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// initialisation des leds
#define PIN2        9
#define NUMPIXELS  16
Adafruit_NeoPixel ring2(NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800);


// initialisation de la timeline
Tween::Timeline timeline;

float red = .0f;
float green = 0.0f;
float blue = 0.0f;

float red2 = .0f;
float green2 = 0.0f;
float blue2 = 0.0f;

/*
   bleu : rgb(0, 200,255 )
   orange : rgb(255, 105, 0)
*/

void setup() {
  // anneau1
  ring1.begin();
  ring2.begin();

  timeline.mode(Tween::Mode::REPEAT_TL);

  timeline.add(red)
  .init(0)
  .then(255, 2000)
  .hold(2000)
  .then(0, 2000)
  .hold(2000);

  timeline.add(green)
  .init(200)
  .then(105, 2000)
  .hold(2000)
  .then(200, 2000)
  .hold(2000);

  timeline.add(blue)
  .init(255)
  .then(0, 2000)
  .hold(2000)
  .then(255, 2000)
  .hold(2000);

  // anneau 2
  
     timeline.add(red2)
    .init(105)
    .then(200, 2000)
    .hold(2000)
    .then(105, 2000)
    .hold(2000);
/*
    timeline.add(green2)
    .init(105)
    .then(200, 2000)
    .hold(2000)
    .then(105, 2000)
    .hold(2000);

    timeline.add(blue2)
    .init(0)
    .then(255, 2000)
    .hold(2000)
    .then(0, 2000)
    .hold(2000);*/

  timeline.start(); // on démarre la timeline



}

void loop() {

  timeline.update(); // on fait les calculs de la timeline

  // on parcourt les leds comme d'habitude
  for (int i = 0; i < NUMPIXELS ; i++) {
    ring1.setPixelColor(i, ring1.Color(red, green, blue));
    ring2.setPixelColor(i, ring2.Color(red, green, blue));
  }
  ring1.show();
  ring2.show();
}

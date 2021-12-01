

#include <Tween.h>
#include <Adafruit_NeoPixel.h>

// initialisation des leds
#define PIN        6
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// initialisation de la timeline
Tween::Timeline timeline;
// initialisation de variables qui vont être modifiées
float red = .0f;
float green = 0.0f;
float blue = 0.0f;


/*
   bleu : rgb(0, 200,255 )
   orange : rgb(255, 105, 0)
*/
void setup() {

  ring1.begin();

  timeline.mode(Tween::Mode::REPEAT_TL); // la timeline doit se répéter à l'infini

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

  timeline.start(); // on démarre la timeline
}

void loop() {

  timeline.update(); // on fait les calculs de la timeline

  // on parcourt les leds comme d'habitude
  for (int i = 0; i < NUMPIXELS ; i++) {
    ring1.setPixelColor(i, ring1.Color(red, green, blue));
  }


  if (timeline.sec() > 2. && timeline.sec() < 4.) {

    for (int i = 0; i < NUMPIXELS ; i+=1) {
      if (i % 4 == 0) {
        ring1.setPixelColor(i, ring1.Color(red, green, blue));
      }
      else {
        ring1.setPixelColor(i, ring1.Color(0, 0, 0));
      }
    }
  }

 

  ring1.show(); // on n'oublie pas d'afficher les couleurs sur l'anneau

}

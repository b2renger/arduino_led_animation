#include <Tween.h>
#include <Adafruit_NeoPixel.h>

// initialisation des leds
#define PIN        6
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

Tween::Timeline timeline;
// on créé une variable qui pemettra de décaler les couleurs
int offset = 0;

void setup() {
  Serial.begin(9600);
  ring1.begin();

  // on créé un timeline qui va agir sur la variable offset
  // pour la faire changer au cours du temps
  timeline.mode(Tween::Mode::REPEAT_TL);
  timeline.add(offset)
  .init(0)
  .then(NUMPIXELS, 1000);
  /* on démarre à 0 on prévoit de faire un tour complet
     d'anneau en 1 seconde/
     il est possible d'utiliser d'autres types d'interpolation !
  */
  timeline.start();
}

void loop() {

  timeline.update();
  // on parcourt les leds comme d'habitude
  for (int i = 0; i < NUMPIXELS ; i++) {
    // on calcul notre index grace au modulo
    int index = (i+offset)%NUMPIXELS
    // on calcule une variable h1 qui transforme notre index en une valeur
    // utilisable comme une teinte
    int h1 = map(index, 0, NUMPIXELS, 0, 65535);
    // on applique notre teinte à chaque pixel de notre anneau
    // en gardant la saturation et la luminosité au maxium
    ring1.setPixelColor(i, ring1.gamma32(ring1.ColorHSV (h1, 255, 255)));
  }
  ring1.show();
}

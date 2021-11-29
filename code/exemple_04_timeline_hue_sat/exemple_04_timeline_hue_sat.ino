#include <Tween.h>
#include <Adafruit_NeoPixel.h>

// initialisation des leds
#define PIN        6
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// initialisation de la timeline
Tween::Timeline timeline;
// initialisation d'une variable qui va être modifiée
float teinte = .0f;
float saturation = 0.0f;

void setup() {

  ring1.begin();

  timeline.mode(Tween::Mode::REPEAT_TL); // la timeline doit se répéter à l'infini

  timeline.add(teinte) // elle va agir sur notre variable "teinte"
  .init(173) // au début on veut une teinte de 173 - light blue
  .then(20, 2000) // on va aller jusqu'à une teinte orangée de 20 en 2 secondes
  //.then<Ease::ElasticInOut>(29, 2000)
  .hold(2000) // on attend 2 secondes
  .then(173, 2000) // on revient à 173 en 2 secondes encore
  //.then<Ease::Sine>(173, 2000)
  .hold(2000); // on attend de nouveau 2 secondes avant de boucler.

  timeline.add(saturation) // elle va agir sur notre variable "saturation"
  .init(255) // au début on veut une saturation maximum
  .then<Ease::SineIn>(0, 1000) //on réduit la saturation à 0 en 1s
  .then<Ease::SineOut>(255, 1000) // on la remonte au max en 1s (la transition vers le orange durant 2s)
  .hold(2000) // on attend pour garder un orange plein
  .then<Ease::SineIn>(0, 1000) // on recommence
  .then<Ease::SineOut>(255, 1000) 
  .hold(2000); // on attend de nouveau 2 secondes avant de boucler.


  timeline.start(); // on démarre la timeline
}

void loop() {

  timeline.update(); // on fait les calculs de la timeline

  // on parcourt les leds comme d'habitude
  for (int i = 0; i < NUMPIXELS ; i++) {
    // on calcule une variable h1 qui transforme notre valeur teinte
    //(qui est drivée par la timeline), grace à la fonction map
    // on passe d'une valeur entre 0 et 360 à une valeur entre 0 et 65535
    int h1 = map(teinte, 0, 360, 0, 65535);
    // on applique notre teinte à chaque pixel de notre anneau
    // en gardant la saturation et la luminosité au maxium
    ring1.setPixelColor(i, ring1.ColorHSV (h1, saturation, 255));
  }

  ring1.show(); // on n'oublie pas d'afficher les couleurs sur l'anneau

}

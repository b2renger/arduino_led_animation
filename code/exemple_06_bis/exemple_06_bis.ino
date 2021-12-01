/*
   Structure de donnée pour stocker des couleurs
*/
struct Vec3 {
  float r;
  float g;
  float b;
  Vec3()
    : r(0), g(0), b(0) {}
  Vec3(const float r, const float g, const float b)
    : r(r), g(g), b(b) {}
  Vec3 operator+(const Vec3& rhs) const {
    return Vec3(r + rhs.r, g + rhs.g, b + rhs.b);
  }
  Vec3 operator-(const Vec3& rhs) const {
    return Vec3(r - rhs.r, g - rhs.g, b - rhs.b);
  }
  Vec3 operator*(const float f) const {
    return Vec3(r * f, g * f, b * f);
  }
};

// biblithèques habituelles
#include <Tween.h>
#include <Adafruit_NeoPixel.h>

// initialisation des leds anneau 1
#define PIN        6
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// initialisation des leds anneau 2
#define PIN2        9 // la broche de controle est différente
#define NUMPIXELS  16
Adafruit_NeoPixel ring2(NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800);


// initialisation de la timeline
Tween::Timeline timeline;

/*
    définition des couleurs de notre séquence
    ce sont des couleurs qui vont être des points de passage
    bleu : rgb(0, 200,255 )
    orange : rgb(255, 105, 0)
    ici nous en avons 2 mais nous pouvons en avoir plus
*/
Vec3 bleu = Vec3(0, 200, 255);
Vec3 orange = Vec3(255, 105, 0);

/*
   définition des couleurs courante pour chaque anneau
   v1 sera pour l'anneau 1
   v2 sera pour l'anneau 2
*/
Vec3 v1;
Vec3 v2;

void setup() {
  // initialisation de l'anneau 1
  ring1.begin();
  ring2.begin();

  // initialisation de la timeline
  timeline.mode(Tween::Mode::REPEAT_TL);

  // première séquence
  // on manipule la valeur v1
  // qui sera la couleur de l'anneau 1
  timeline.add(v1)
  .init(orange)
  .then(bleu, 2000)
  .hold(2000)
  .then(orange, 2000)
  .hold(2000);

  // seconde séquence
  // on manipule la valeur v2
  // qui sera la couleur de l'anneau 2
  timeline.add(v2)
  .init(bleu)
  .then(orange, 2000)
  .hold(2000)
  .then(bleu, 2000)
  .hold(2000);

  timeline.start(); // on démarre la timeline
}

void loop() {

  timeline.update(); // on fait les calculs de la timeline

  // on parcourt les leds comme d'habitude
  for (int i = 0; i < NUMPIXELS ; i++) {
    // on assigne l'anneau 1
    ring1.setPixelColor(i, ring1.Color(v1.r, v1.g, v1.b)); // on utilise v1 pour accéder à ses composantes r g et b
    // on assigne l'anneau 2
    ring2.setPixelColor(i, ring2.Color(v2.r, v2.g, v2.b)); // on utilise v2 pour accéder à ses composantes r g et b
  }
  // on actualise les anneaux
  ring1.show();
  ring2.show();
}

// la bibliothèque pour les leds
#include <Adafruit_NeoPixel.h>

// la structure de données pour stocker les couleurs
struct Vec3 {
  float c1;
  float c2;
  float c3;
  Vec3()
    : c1(0), c2(0), c3(0) {}
  Vec3(const float c1, const float c2, const float c3)
    : c1(c1), c2(c2), c3(c3) {}
};

// quelques définitions de couleurs
Vec3 bleu = Vec3(0, 200, 255);
Vec3 orange = Vec3(255, 105, 0);
Vec3 noir = Vec3(0, 0, 0);
Vec3 rose = Vec3(220, 0, 120);

// initialisation des leds
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800); // nombre de leds, broche arduino, type de leds
Adafruit_NeoPixel ring2(NUMPIXELS, 9, NEO_GRB + NEO_KHZ800);



void setup() {
  
  ring1.begin();// démarrage de l'anneau
  ring2.begin();

}

void loop() {
  // gestion du temps
  long totalTime = 5000; // temps total de l'animation
  long dur = millis() % totalTime; // timing

  // créer des transitions

  // transitions linéaires
  transition_lin(&ring1, NUMPIXELS, dur, 0, 2500, noir, bleu);
  transition_lin(&ring1, NUMPIXELS, dur, 2500, 5000, bleu, noir);
  
  // flash de couleurs fixes
  fixed_color(&ring2, NUMPIXELS, dur, 0, 1000, noir);
  fixed_color(&ring2, NUMPIXELS, dur, 1000, 2000, bleu);
  fixed_color(&ring2, NUMPIXELS, dur, 2000, 3000, noir);
  fixed_color(&ring2, NUMPIXELS, dur, 3000, 4000, rose);
  fixed_color(&ring2, NUMPIXELS, dur, 4000, 5000, noir);




  ring1.show();
  ring2.show();

}

// pas de transition : une couleur fixe pendant un durée
void fixed_color(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 c) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(c.c1, c.c2, c.c3));
    }
  }
}

// transition linéaire d'une couleur à une autre pendant une durée donnée
void transition_lin(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    float r = map(currentTime, 0, 1000, startC.c1, endC.c1);
    float g = map(currentTime, 0, 1000, startC.c2, endC.c2);
    float b = map(currentTime, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}

// transitions : https://easings.net/
// transition sine
void transition_sine_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = -(cos(PI * currentTime) - 1) / 2.;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_sine_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = 1 - cos((currentTime * PI) / 2.);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_sine_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = sin((currentTime * PI) / 2);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
// Transition circ
void transition_circ_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = currentTime < 0.5 ? (1 - sqrt(1 - pow(2 * currentTime, 2))) / 2 : (sqrt(1 - pow(-2 * currentTime + 2, 2)) + 1) / 2;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_circ_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = 1 - sqrt(1 - pow(currentTime, 2));
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_circ_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  sqrt(1 - pow(currentTime - 1, 2));
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
// Transition exponentielle
void transition_expo_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  currentTime == 0
                      ? 0
                      : currentTime == 1
                               ? 1
                               : currentTime < 0.5 ? pow(2, 20 * currentTime - 10) / 2
                               : (2 - pow(2, -20 * currentTime + 10)) / 2;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_expo_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  currentTime == 1 ? 1 : 1 - pow(2, -10 * currentTime);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_expo_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =   currentTime == 0 ? 0 : pow(2, 10 * currentTime - 10);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}

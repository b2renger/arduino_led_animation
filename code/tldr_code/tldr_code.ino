/////////////////////////////////////////////////////////////////////
// Les parties à n pas modifier
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
// les types de dynamiques disponibles pour vos transitions
// transitions : https://easings.net/
int LIN = 0;
int SIN_IN_OUT = 1;
int SIN_IN = 2;
int SIN_OUT = 3;
int CIRC_IN_OUT = 4;
int CIRC_IN = 5;
int CIRC_OUT = 6;
int EXPO_IN_OUT = 7;
int EXPO_IN = 8;
int EXPO_OUT = 9;


////////////////////////////////////////////////////////////////////////
// Les parties que vous devez modifier
// quelques définitions de couleurs
// vous pouvez en ajouter d'autres au format RGB
Vec3 bleu = Vec3(0, 200, 255);
Vec3 orange = Vec3(255, 105, 0);
Vec3 noir = Vec3(0, 0, 0);
Vec3 rose = Vec3(220, 0, 120);
// initialisation des leds
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);  // nombre de leds, broche arduino, type de leds
//Adafruit_NeoPixel ring2(NUMPIXELS, 9, NEO_GRB + NEO_KHZ800);  // nombre de leds, broche arduino, type de leds

void setup() {
  ring1.begin();  // démarrage de l'anneau
  //ring2.begin();  // démarrage de l'anneau
}

void loop() {
  // gestion du temps
  long totalTime = 5000;            // temps total de l'animation
  long dur = millis() % totalTime;  // timing

  // créer des transitions à vous de jouer !
  transition(LIN, &ring1, NUMPIXELS, dur, 0, 2500, noir, bleu); // du noir au bleu entre la seconde 0 et 2,5 secondes
  transition(LIN, &ring1, NUMPIXELS, dur, 2500, 5000, bleu, noir); // du bleu au noit entre la seconde 2,5 et 5 secondes

  // ou avec une dynamique un peu différentes
  //transition(SIN_IN_OUT, &ring2, NUMPIXELS, dur, 0, 2500, noir, bleu);
  //transition(SIN_IN_OUT, &ring2, NUMPIXELS, dur, 2500, 5000, bleu, noir);
  
  // ou
  //animation_radiale(LIN, &ring1, NUMPIXELS, dur, 0, 5000);
  //animation_radiale(SIN_OUT, &ring2, NUMPIXELS, dur, 0, 5000);


  ring1.show();
  //ring2.show();
}



// toutes les transitions disponibles ci-dessous
// vous pouvez les utiliser dans le loop, ne les modifiez que si vous comprenez

// pas de transition : une couleur fixe pendant un durée
void fixed_color(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 c) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    for (int i = 0; i < n; i++) {
      strip->setPixelColor(i, strip->Color(c.c1, c.c2, c.c3));
    }
  }
}

// transition d'une couleur à une autre pendant une durée donnée avec une dynamique donnée
void transition(int easeType, Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = easing(easeType,t,startT,endT);
    float r = map(currentTime * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(currentTime * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(currentTime * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n; i++) {
      strip->setPixelColor(i, strip->gamma32(strip->Color(r, g, b)));
    }
  }
}

// animation circulaire 
void animation_radiale(int easeType, Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, float startH, float endH) {
  if (t > startT && t < endT) {
    int currentTime = easing(easeType,t,startT,endT)*1000;
    int ledsOffset = map(currentTime, 0, 1000, 0, n * 8);  // 8 représenter le nombre de tours pendant la durée de l'animation
    for (int i = 0; i < n; i++) {
      float teinteHSB = map(i, 0, n, startH, endH);                                                           // on veut une teinte entre 100 et 235 pour notre dégradé
      float teinteLeds = map(teinteHSB, 0, 360, 0, 65535);                                                // on transforme dans le référentiel led
      strip->setPixelColor((i + ledsOffset) % n, strip->gamma32(strip->ColorHSV(teinteLeds, 255, 255)));  // on applique la couleur
    }
  }
}


// fonction pour gérer les dynamiques
float easing(int easeType, int tt, int startT, int endT) {
  float t = 0;
  float currentTime = map(tt, startT, endT, 0, 1000) / 1000.;
  if (easeType == 0) {  // lin
    t = currentTime;
  } else if (easeType == 1) {  // sinInOut
    t = -(cos(PI * currentTime) - 1) / 2.;
  } else if (easeType == 2) {  // sinIn
    t = 1 - cos((currentTime * PI) / 2.);
  } else if (easeType == 3) {  // sinOut
    t = sin((currentTime * PI) / 2);
  } else if (easeType == 4) {  // circInOut
    t = currentTime < 0.5 ? (1 - sqrt(1 - pow(2 * currentTime, 2))) / 2 : (sqrt(1 - pow(-2 * currentTime + 2, 2)) + 1) / 2;
  } else if (easeType == 5) {  // circIn
    t = 1 - sqrt(1 - pow(currentTime, 2));
  } else if (easeType == 6) {  // circOut
    t = sqrt(1 - pow(currentTime - 1, 2));
  } else if (easeType == 7) {  // expo
    t = currentTime == 0
          ? 0
        : currentTime == 1
          ? 1
        : currentTime < 0.5 ? pow(2, 20 * currentTime - 10) / 2
                            : (2 - pow(2, -20 * currentTime + 10)) / 2;
  } else if (easeType == 8) {  // expoIn
    t = currentTime == 1 ? 1 : 1 - pow(2, -10 * currentTime);
  } else if (easeType == 9) {  // expoOut
    t = currentTime == 0 ? 0 : pow(2, 10 * currentTime - 10);
  }
  return t;
}
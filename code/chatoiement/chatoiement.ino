
#include <Adafruit_NeoPixel.h>
#include <SimplexNoise.h>

#define PIN 6
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// initialiser la bibliothèque pour le noise
SimplexNoise sn;

double nx;
double x = 0.0;

double ny;
double y = 0.0;

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


void setup() {

  ring1.begin();

  // obtenir des valeurs aléatoires au lancement du programme
  x = analogRead(0) / 10.;
  y = analogRead(2) / 10.;
}

void loop() {
  // gestion du temps
  long totalTime = 15000;           // temps total de l'animation
  long dur = millis() % totalTime;  // timing

  chatoiement_valeur(&ring1, NUMPIXELS, dur, 0, 5000, 100, 180, 0, 255);
  chatoiement(&ring1, NUMPIXELS, dur, 5000, 8000, 100, 180);
  chatoiement_color(&ring1, NUMPIXELS, dur, 8000, 11000, 100, 180, 0, 10);
  chatoiement_valeur(&ring1, NUMPIXELS, dur, 11000, 15000, 0, 10, 255, 0);

  ring1.show();
}

void chatoiement(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, float h1, float h2) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    x += 0.005;
    y += 0.001;
    for (int i = 0; i < n; i++) {
      nx = sn.noise(x, i);
      int valeur = map(nx * 100, -100, 100, 0, 255);  //la valeur des leds chatoie entre 0 et 255
      ny = sn.noise(y, i * 2);
      int teinte = map(ny * 100, -100, 100, h1, h2);                                   //la teinte est entre les valeurs h1 et h2 dans les arguments de la fonction
      int teinte1 = map(teinte, 0, 360, 0, 65535);                                     // on transforme dans le référentiel led
      strip->setPixelColor(i, strip->gamma32(strip->ColorHSV(teinte1, 255, valeur)));  // on applique la couleur
    }
  }
}

void chatoiement_valeur(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, float h1, float h2, float valeur1, float valeur2) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    x += 0.005;
    y += 0.001;
    for (int i = 0; i < n; i++) {
      nx = sn.noise(x, i);
      float valeur = map(currentTime, 0, 1000, valeur1, valeur2);     //la valeur passe de valeur1 à valeur2 pendant la durée de la transition
      float noiseValeur = map(nx * 100, -100, 100, 0, 1000) / 1000.;  //on crée un noise entre 0 et 1 pour la valeur finale
      ny = sn.noise(y, i * 2);
      int teinte = map(ny * 100, -100, 100, h1, h2);  //la teinte est entre les valeurs h1 et h2 dans les arguments de la fonction
      int teinte1 = map(teinte, 0, 360, 0, 65535);
      strip->setPixelColor(i, strip->gamma32(strip->ColorHSV(teinte1, 255, noiseValeur * valeur)));  // on applique la couleur avec la valeur qui chatoie et qui passe de valeur1 à valeur2
    }
  }
}

void chatoiement_color(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, float h_debut_1, float h_debut_2, float h_fin_1, float h_fin_2) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);

    x += 0.005;
    y += 0.001;
    for (int i = 0; i < n; i++) {
      nx = sn.noise(x, i);
      int valeur = map(nx * 100, -100, 100, 0, 255);  //la valeur des leds chatoie entre 0 et 255
      ny = sn.noise(y, i * 2);
      int h1 = map(currentTime, 0, 1000, h_debut_1, h_fin_1);
      int h2 = map(currentTime, 0, 1000, h_debut_2, h_fin_2);
      int teinte = map(ny * 100, -100, 100, h1, h2);  //
      int teinteFinale = map(teinte, 0, 360, 0, 65535);
      strip->setPixelColor(i, strip->gamma32(strip->ColorHSV(teinteFinale, 255, valeur)));  // on applique la couleur avec la valeur qui chatoie et qui passe de valeur1 à valeur2
    }
  }
}
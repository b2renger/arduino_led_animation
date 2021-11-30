
#include <Adafruit_NeoPixel.h>
#include <SimplexNoise.h>

#define PIN        6
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// initialiser la bibliothèque pour le noise
SimplexNoise sn;

double n;
double x = 0.0;

double ny;
double y = 0.0;


void setup() {

  ring1.begin(); 

  // obtenir des valeurs aléatoires au lancement du programme
  x= analogRead(0)/10.;
  y= analogRead(2)/ 10.;
}

void loop() {

  // on augmente les variables x et y lentement
  x += 0.005;
  y += 0.001;

  // on parcourt tous les pixels
  for (int i = 0; i < NUMPIXELS; i++) {

    // on calcule une valeur de noise (comprise entre 0 et 1)
    n = sn.noise(x, i); 
    // on retransforme cette valeur en une valeur comprise entre 0 et 255
    int br = map(n * 100, -100, 100, 0, 255); 

    // on fait de même avec notre seconde valeur.
    n = sn.noise(y, i*2);
    // on retransforme pour avoir des valeurs entre 160 et 270
    // référentiel de la roue chromatique
    int h = map(n*100, -100, 100, 160, 270);
    // on retransforme enfin pour avoir des valeurs entre 0 et 65535
    // dans le référentiel des anneaux de leds.
    int h1 = map(h, 0, 360, 0, 65535);
    
    ring1.setPixelColor(i, ring1.gamma32(ring1.ColorHSV (h1, 255, br)));
  }
  ring1.show();   


}

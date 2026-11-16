// LedAnimation : des animations de leds neopixel basées sur une timeline,
// écrites en enchaînant des fonctions.
//
//   ring1.transition(noir, bleu).from(0).to(2500).ease(SIN_IN_OUT);
//
// Tout ce qui n'est pas précisé prend une valeur par défaut.
// Le tutoriel complet : https://github.com/b2renger/arduino_led_animation
//
// Ce fichier est organisé en 7 parties :
//   1. les couleurs
//   2. le temps
//   3. les dynamiques (easing)
//   4. le noise
//   5. l'anneau de leds (LedRing) et l'animation (Anim)
//   6. les animations disponibles  <- des modèles pour écrire les vôtres
//   7. le lien entre l'anneau et les animations

#ifndef LED_ANIMATION_H
#define LED_ANIMATION_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

////////////////////////////////////////////////////////////////////////
// 1. Les couleurs

// la structure de données pour stocker les couleurs (en RGB, de 0 à 255)
struct Vec3 {
  float c1;
  float c2;
  float c3;
  Vec3()
    : c1(0), c2(0), c3(0) {}
  Vec3(const float c1, const float c2, const float c3)
    : c1(c1), c2(c2), c3(c3) {}
};

// une couleur à partir d'un Vec3
inline uint32_t rgb(Vec3 c) {
  return Adafruit_NeoPixel::Color(constrain(c.c1, 0, 255), constrain(c.c2, 0, 255), constrain(c.c3, 0, 255));
}

// une couleur en HSB : teinte entre 0 et 360, saturation et luminosité entre 0 et 255
inline uint32_t hsv(float teinte, float saturation = 255, float valeur = 255) {
  teinte = fmod(teinte, 360);
  if (teinte < 0) teinte += 360;
  return Adafruit_NeoPixel::ColorHSV((uint16_t)(teinte / 360. * 65535.), constrain(saturation, 0, 255), constrain(valeur, 0, 255));
}

// un mélange de deux couleurs : k = 0 donne "debut", k = 1 donne "fin"
inline Vec3 mixColors(Vec3 debut, Vec3 fin, float k) {
  return Vec3(debut.c1 + (fin.c1 - debut.c1) * k, debut.c2 + (fin.c2 - debut.c2) * k, debut.c3 + (fin.c3 - debut.c3) * k);
}

////////////////////////////////////////////////////////////////////////
// 2. Le temps

struct LedClock {
  unsigned long total = 5000;  // durée de la boucle
  unsigned long now = 0;       // temps actuel dans la boucle
};
inline LedClock &ledClock() {
  static LedClock c;
  return c;
}

// à appeler au début du loop() : l'animation boucle toutes les "totalTime" millisecondes
inline void timeline(unsigned long totalTime) {
  if (totalTime > 0) ledClock().total = totalTime;
  ledClock().now = millis() % ledClock().total;
}

////////////////////////////////////////////////////////////////////////
// 3. Les dynamiques : https://easings.net/

enum Ease {
  LIN,
  SIN_IN_OUT,
  SIN_IN,
  SIN_OUT,
  CIRC_IN_OUT,
  CIRC_IN,
  CIRC_OUT,
  EXPO_IN_OUT,
  EXPO_IN,
  EXPO_OUT
};

// x est l'avancement entre 0 et 1, le résultat est aussi entre 0 et 1
inline float easing(Ease easeType, float x) {
  switch (easeType) {
    case SIN_IN_OUT: return -(cos(PI * x) - 1) / 2.;
    case SIN_IN: return 1 - cos((x * PI) / 2.);
    case SIN_OUT: return sin((x * PI) / 2.);
    case CIRC_IN_OUT: return x < 0.5 ? (1 - sqrt(1 - pow(2 * x, 2))) / 2 : (sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2;
    case CIRC_IN: return 1 - sqrt(1 - pow(x, 2));
    case CIRC_OUT: return sqrt(1 - pow(x - 1, 2));
    case EXPO_IN_OUT: return x <= 0 ? 0 : x >= 1 ? 1 : x < 0.5 ? pow(2, 20 * x - 10) / 2 : (2 - pow(2, -20 * x + 10)) / 2;
    case EXPO_IN: return x <= 0 ? 0 : pow(2, 10 * x - 10);
    case EXPO_OUT: return x >= 1 ? 1 : 1 - pow(2, -10 * x);
    default: return x;
  }
}

////////////////////////////////////////////////////////////////////////
// 4. Noise : des nombres aléatoires cohérents entre eux, entre -1 et 1
// (le motif se répète quand x augmente de 256)

inline float ledHash(long x, long y) {
  uint32_t h = (uint32_t)(x & 255) * 374761393UL + (uint32_t)y * 668265263UL;
  h = (h ^ (h >> 13)) * 1274126177UL;
  h ^= h >> 16;
  return (h & 0xFFFF) / 32767.5 - 1.;
}

inline float ledNoise(float x, float y) {
  long xi = floor(x);
  long yi = floor(y);
  float fx = x - xi;
  float fy = y - yi;
  fx = fx * fx * (3 - 2 * fx);
  fy = fy * fy * (3 - 2 * fy);
  float haut = ledHash(xi, yi) + (ledHash(xi + 1, yi) - ledHash(xi, yi)) * fx;
  float bas = ledHash(xi, yi + 1) + (ledHash(xi + 1, yi + 1) - ledHash(xi, yi + 1)) * fx;
  return haut + (bas - haut) * fy;
}

////////////////////////////////////////////////////////////////////////
// 5. L'anneau de leds et l'animation

class Anim;
// une animation est une fonction qui colore les leds, cf. la partie 6
typedef void (*Effect)(Anim &a);

// les sélections de leds toutes prêtes pour .leds()
enum LedSelection {
  FULL,         // toutes les leds
  ODDS,         // les leds impaires : 1, 3, 5 ...
  EVEN,         // les leds paires : 0, 2, 4 ...
  FIRST_HALF,   // la première moitié
  SECOND_HALF   // la seconde moitié
};

// Un anneau ou ruban de leds : c'est un Adafruit_NeoPixel (setPixelColor,
// ColorHSV ... fonctionnent toujours) auquel on ajoute les animations.
class LedRing : public Adafruit_NeoPixel {
public:
  // nombre de leds, broche arduino, type de leds
  LedRing(uint16_t n, int16_t pin, neoPixelType type = NEO_GRB + NEO_KHZ800)
    : Adafruit_NeoPixel(n, pin, type) {}

  // les animations (elles sont écrites dans les parties 6 et 7)
  Anim fixed_color(Vec3 couleur);
  Anim transition(Vec3 debut, Vec3 fin);
  Anim sunrise(Vec3 debut, Vec3 fin);
  Anim animation_radiale();
  Anim chatoiement();
  // jouer n'importe quelle animation, y compris les vôtres
  Anim play(Effect fx);

  // envoyer les couleurs aux leds
  void show() {
    if (!begun) begin();  // au cas où begin() a été oublié dans le setup()
    Adafruit_NeoPixel::show();
    timeline(0);  // on met le temps à jour pour le prochain tour de loop()
  }

  // le temps du noise de cet anneau : il n'avance que quand on l'utilise,
  // un chatoiement reprend donc là où il s'était arrêté
  float noiseTime(float speed) {
    unsigned long now = millis();
    unsigned long dt = min(now - noiseLast, 100UL);
    noiseLast = now;
    noiseX += (long)(dt * 600. * speed);
    if (noiseX >= 1024000000L) noiseX -= 1024000000L;
    if (noiseX < 0) noiseX += 1024000000L;
    return noiseX / 1000000.;  // entre 0 et 1024
  }

private:
  long noiseX = 0;
  unsigned long noiseLast = 0;
};

// Une animation : c'est l'objet sur lequel on enchaîne les fonctions.
// Elle se joue toute seule à la fin de la ligne (au point-virgule).
class Anim {
public:
  // --- les valeurs utilisables quand on écrit sa propre animation ---
  LedRing &strip;                    // les leds
  int n;                             // le nombre de leds
  float t = 0;                       // l'avancement de l'animation, entre 0 et 1 (dynamique appliquée)
  unsigned long ms = 0;              // le temps écoulé depuis le début de l'animation, en millisecondes
  Vec3 color1, color2;               // .colors()
  float hue1 = 0, hue2 = 360;        // .hue()
  float hueTo1 = 0, hueTo2 = 360;    // .hueTo()
  float value1 = 255, value2 = 255;  // .value()
  float nTurns = 8;                  // .turns()
  float speedK = 1;                  // .speed()

  // une valeur qui va de "debut" à "fin" pendant l'animation
  float mix(float debut, float fin) {
    return debut + (fin - debut) * t;
  }
  // une couleur qui va de "debut" à "fin" pendant l'animation
  Vec3 mix(Vec3 debut, Vec3 fin) {
    return mixColors(debut, fin, t);
  }
  // donner une couleur à une led : la sélection .leds(), la luminosité .value()
  // et la correction gamma sont appliquées ici
  void pixel(int i, uint32_t color) {
    if (!selected(i)) return;
    if (dim < 1) {
      color = Adafruit_NeoPixel::Color(((color >> 16) & 255) * dim, ((color >> 8) & 255) * dim, (color & 255) * dim);
    }
    strip.setPixelColor(i, Adafruit_NeoPixel::gamma32(color));
  }
  void pixel(int i, Vec3 color) {
    pixel(i, rgb(color));
  }
  // donner la même couleur à toutes les leds
  void fill(uint32_t color) {
    for (int i = 0; i < n; i++) pixel(i, color);
  }
  void fill(Vec3 color) {
    fill(rgb(color));
  }
  // est-ce que la led i fait partie de la sélection .leds() ?
  bool selected(int i) {
    if (i < 0 || i >= n || i < first || i > last) return false;
    if (preset == ODDS && i % 2 == 0) return false;
    if (preset == EVEN && i % 2 == 1) return false;
    if (preset == FIRST_HALF && i >= n / 2) return false;
    if (preset == SECOND_HALF && i < n / 2) return false;
    if (hasList) return i < 256 && ((list[i >> 3] >> (i & 7)) & 1);
    return true;
  }

  // --- les fonctions à enchaîner ---
  // début et fin en millisecondes (par défaut : toute la durée de la timeline)
  Anim &from(unsigned long debut) {
    tStart = debut;
    return *this;
  }
  Anim &to(unsigned long fin) {
    tEnd = fin;
    hasEnd = true;
    return *this;
  }
  // la dynamique (par défaut : LIN)
  Anim &ease(Ease e) {
    easeType = e;
    return *this;
  }
  // n'utiliser qu'une sélection toute prête : FULL, ODDS, EVEN, FIRST_HALF, SECOND_HALF
  Anim &leds(LedSelection selection) {
    preset = selection;
    return *this;
  }
  // n'utiliser que les leds de "premiere" à "derniere"
  Anim &leds(int premiere, int derniere) {
    first = premiere;
    last = derniere;
    return *this;
  }
  // n'utiliser que les leds dont on donne les numéros : .leds({ 0, 4, 5, 9 })
  // (numéros entre 0 et 255)
  template<size_t N>
  Anim &leds(const int (&numeros)[N]) {
    hasList = true;
    for (size_t k = 0; k < sizeof(list); k++) list[k] = 0;
    for (size_t k = 0; k < N; k++) {
      if (numeros[k] >= 0 && numeros[k] < 256) list[numeros[k] >> 3] |= 1 << (numeros[k] & 7);
    }
    return *this;
  }
  // couleur de début et de fin
  Anim &colors(Vec3 debut, Vec3 fin) {
    color1 = debut;
    color2 = fin;
    return *this;
  }
  // les teintes utilisées, entre 0 et 360
  Anim &hue(float h1, float h2) {
    hue1 = h1;
    hue2 = h2;
    return *this;
  }
  // les teintes à la fin de l'animation (par défaut : les mêmes qu'au début)
  Anim &hueTo(float h1, float h2) {
    hueTo1 = h1;
    hueTo2 = h2;
    hasHueTo = true;
    return *this;
  }
  // la luminosité au début et à la fin, entre 0 et 255 (par défaut : 255)
  Anim &value(float debut, float fin) {
    value1 = debut;
    value2 = fin;
    return *this;
  }
  // le nombre de tours pendant l'animation (par défaut : 8, négatif = dans l'autre sens)
  Anim &turns(float k) {
    nTurns = k;
    return *this;
  }
  // la vitesse : 2 = deux fois plus vite, 0.5 = deux fois plus lent
  Anim &speed(float k) {
    speedK = k;
    return *this;
  }

  // --- la mécanique ---
  Anim(LedRing &strip, Effect fx)
    : strip(strip), n(strip.numPixels()), last(strip.numPixels() - 1), fx(fx) {}
  Anim(Anim &&o)
    : Anim((const Anim &)o) {
    o.fx = 0;
  }
  // la fin de la ligne : si c'est le moment, on joue l'animation
  ~Anim() {
    if (!fx) return;
    unsigned long now = ledClock().now;
    unsigned long end = hasEnd ? tEnd : ledClock().total;
    if (now < tStart || now >= end) return;
    ms = now - tStart;
    t = easing(easeType, (float)ms / (end - tStart));
    if (!hasHueTo) {
      hueTo1 = hue1;
      hueTo2 = hue2;
    }
    dim = constrain(mix(value1, value2) / 255., 0, 1);
    fx(*this);
  }

private:
  Anim(const Anim &) = default;
  unsigned long tStart = 0, tEnd = 0;
  bool hasEnd = false, hasHueTo = false, hasList = false;
  Ease easeType = LIN;
  LedSelection preset = FULL;
  int first = 0, last;
  uint8_t list[32];
  float dim = 1;
  Effect fx;
};

////////////////////////////////////////////////////////////////////////
// 6. Les animations disponibles.
// Vous pouvez écrire les vôtres sur le même modèle dans votre programme
// et les jouer avec ring1.play(nom_de_votre_fonction)

// une couleur fixe
inline void fx_fixed_color(Anim &a) {
  a.fill(a.color1);
}

// transition d'une couleur à une autre
inline void fx_transition(Anim &a) {
  a.fill(a.mix(a.color1, a.color2));
}

// transition d'une couleur à une autre led par led, en partant de la led 0
// et en faisant le tour de l'anneau des deux côtés à la fois
inline void fx_sunrise(Anim &a) {
  int paires = (a.n + 1) / 2;
  float position = a.t * paires;  // le nombre de paires de leds déjà "levées"
  for (int i = 0; i < paires; i++) {
    float k = constrain(position - i, 0, 1);  // l'avancement de cette paire, entre 0 et 1
    Vec3 couleur = mixColors(a.color1, a.color2, k);
    a.pixel(i, couleur);
    a.pixel(a.n - 1 - i, couleur);
  }
}

// dégradé de teintes qui tourne
inline void fx_animation_radiale(Anim &a) {
  long ledsOffset = a.t * a.n * a.nTurns;
  for (int i = 0; i < a.n; i++) {
    float teinte = a.hue1 + (a.hue2 - a.hue1) * i / a.n;
    int led = ((i + ledsOffset) % a.n + a.n) % a.n;  // toujours entre 0 et n, même si on tourne à l'envers
    a.pixel(led, hsv(teinte));
  }
}

// chatoiement : la luminosité et la teinte de chaque led ondulent grâce au noise
inline void fx_chatoiement(Anim &a) {
  float x = a.strip.noiseTime(a.speedK);
  int anneau = a.strip.getPin() * 37;  // chaque anneau a son propre chatoiement
  float h1 = a.mix(a.hue1, a.hueTo1);
  float h2 = a.mix(a.hue2, a.hueTo2);
  for (int i = 0; i < a.n; i++) {
    float noiseValeur = (ledNoise(x, i + anneau) + 1) / 2.;                // entre 0 et 1
    float noiseTeinte = (ledNoise(x / 4., i * 2 + 1000 + anneau) + 1) / 2.;  // entre 0 et 1, plus lent
    a.pixel(i, hsv(h1 + (h2 - h1) * noiseTeinte, 255, noiseValeur * 255));
  }
}

////////////////////////////////////////////////////////////////////////
// 7. Le lien entre l'anneau et les animations, avec les valeurs par défaut

inline Anim LedRing::play(Effect fx) {
  return Anim(*this, fx);
}
inline Anim LedRing::fixed_color(Vec3 couleur) {
  Anim a(*this, fx_fixed_color);
  a.colors(couleur, couleur);
  return a;
}
inline Anim LedRing::transition(Vec3 debut, Vec3 fin) {
  Anim a(*this, fx_transition);
  a.colors(debut, fin);
  return a;
}
inline Anim LedRing::sunrise(Vec3 debut, Vec3 fin) {
  Anim a(*this, fx_sunrise);
  a.colors(debut, fin);
  return a;
}
inline Anim LedRing::animation_radiale() {
  Anim a(*this, fx_animation_radiale);
  a.hue(100, 235);
  return a;
}
inline Anim LedRing::chatoiement() {
  Anim a(*this, fx_chatoiement);
  a.hue(160, 270);
  return a;
}

#endif

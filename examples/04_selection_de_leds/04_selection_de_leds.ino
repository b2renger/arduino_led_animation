// 04 - Choisir les leds utilisées par une animation
//
// Par défaut une animation utilise toutes les leds de l'anneau.
// Avec .leds() on peut n'en utiliser qu'une partie, de trois manières :
//
//   .leds(EVEN)             une sélection toute prête
//   .leds(4, 11)            toutes les leds de la numéro 4 à la numéro 11
//   .leds({ 0, 4, 8, 12 })  une liste de numéros, entre accolades
//
// Les leds sont numérotées à partir de 0 : un anneau de 16 leds va de 0 à 15.
// Les leds qui ne sont pas sélectionnées ne sont pas modifiées par l'animation.
//
// Branchement : un anneau de 16 leds, "data in" sur la broche 6.

#include <LedAnimation.h>

Vec3 noir = Vec3(0, 0, 0);
Vec3 bleu = Vec3(0, 200, 255);
Vec3 rose = Vec3(220, 0, 120);
Vec3 orange = Vec3(255, 105, 0);

LedRing ring1(16, 6);

// Une liste de numéros peut aussi être rangée dans une variable, pour lui donner
// un nom et la réutiliser dans plusieurs animations.
int croix[] = { 0, 4, 8, 12 };

void setup() {
  ring1.begin();
}

void loop() {
  timeline(12000);

  // --- de 0 à 3 secondes : les sélections toutes prêtes ---
  // Les leds paires (0, 2, 4 ...) en bleu, les impaires (1, 3, 5 ...) en rose.
  // Les deux animations se jouent en même temps, chacune sur ses propres leds.
  ring1.transition(noir, bleu).to(1500).leds(EVEN);
  ring1.transition(noir, rose).to(1500).leds(ODDS);
  // Puis on éteint d'un coup la première moitié de l'anneau, et ensuite la seconde.
  // Pendant que la première moitié s'éteint, la seconde garde ses couleurs :
  // aucune animation ne la modifie.
  ring1.fixed_color(noir).from(1500).to(2250).leds(FIRST_HALF);
  ring1.fixed_color(noir).from(2250).to(3000).leds(SECOND_HALF);

  // --- de 3 à 6 secondes : de telle led à telle led ---
  // Tout l'anneau est noir, sauf les leds 4 à 11 qui s'allument en orange.
  ring1.fixed_color(noir).from(3000).to(6000);
  ring1.transition(noir, orange).from(3000).to(4500).leds(4, 11);
  ring1.transition(orange, noir).from(4500).to(6000).leds(4, 11);

  // --- de 6 à 9 secondes : une liste de numéros ---
  // Les numéros s'écrivent entre accolades, séparés par des virgules,
  // dans l'ordre que vous voulez.
  ring1.fixed_color(noir).from(6000).to(9000);
  ring1.transition(noir, bleu).from(6000).to(7500).leds({ 0, 1, 2, 8, 9, 10 });
  ring1.transition(bleu, noir).from(7500).to(9000).leds({ 0, 1, 2, 8, 9, 10 });

  // --- de 9 à 12 secondes : une liste rangée dans une variable ---
  // "croix" est définie en haut du programme.
  ring1.fixed_color(noir).from(9000);
  ring1.transition(noir, rose).from(9000).to(10500).leds(croix);
  ring1.transition(rose, noir).from(10500).leds(croix);

  ring1.show();
}

// Les sélections toutes prêtes :
//   FULL          toutes les leds (c'est la valeur par défaut)
//   EVEN          les leds paires : 0, 2, 4 ...
//   ODDS          les leds impaires : 1, 3, 5 ...
//   FIRST_HALF    la première moitié de l'anneau
//   SECOND_HALF   la seconde moitié de l'anneau
//
// .leds() fonctionne avec toutes les animations : chatoiement, animation_radiale ...

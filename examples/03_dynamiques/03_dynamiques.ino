// 03 - Les dynamiques (easing)
//
// Une transition peut être régulière, ou bien accélérer et ralentir : c'est sa
// dynamique, que l'on choisit avec .ease().
// Pour bien voir la différence, les deux moitiés de l'anneau font la même
// transition en même temps mais avec deux dynamiques différentes.
//
// Les courbes sont dessinées sur cette page : https://easings.net/
//
// Branchement : un anneau de 16 leds, "data in" sur la broche 6.

#include <LedAnimation.h>

Vec3 noir = Vec3(0, 0, 0);
Vec3 orange = Vec3(255, 105, 0);

LedRing ring1(16, 6);

void setup() {
  ring1.begin();
}

void loop() {
  timeline(6000);

  // --- première moitié de l'anneau : dynamique linéaire ---
  // LIN est la dynamique par défaut : écrire .ease(LIN) ou ne rien écrire revient au même.
  // La luminosité change à vitesse constante.
  ring1.transition(noir, orange).from(0).to(3000).ease(LIN).leds(FIRST_HALF);
  ring1.transition(orange, noir).from(3000).to(6000).ease(LIN).leds(FIRST_HALF);

  // --- seconde moitié de l'anneau : dynamique exponentielle ---
  // EXPO_IN : démarre très lentement puis accélère fortement à la fin.
  // EXPO_OUT : démarre très vite puis ralentit à la fin.
  ring1.transition(noir, orange).from(0).to(3000).ease(EXPO_IN).leds(SECOND_HALF);
  ring1.transition(orange, noir).from(3000).to(6000).ease(EXPO_OUT).leds(SECOND_HALF);

  ring1.show();
}

// Les dynamiques disponibles :
//
//   LIN           vitesse constante
//
//   SIN_IN        démarre doucement              (le plus doux)
//   SIN_OUT       finit doucement
//   SIN_IN_OUT    démarre et finit doucement
//
//   CIRC_IN       démarre doucement              (plus marqué)
//   CIRC_OUT      finit doucement
//   CIRC_IN_OUT   démarre et finit doucement
//
//   EXPO_IN       démarre doucement              (le plus marqué)
//   EXPO_OUT      finit doucement
//   EXPO_IN_OUT   démarre et finit doucement
//
// "IN" agit sur le début de l'animation, "OUT" sur la fin.
//
// A essayer : remplacer EXPO_IN et EXPO_OUT par d'autres dynamiques de la liste.

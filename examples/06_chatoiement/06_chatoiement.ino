// 06 - Chatoiement
//
// Un rendu organique : la luminosité et la teinte de chaque led ondulent
// indépendamment, comme des reflets sur l'eau ou des braises.
// Le mouvement est produit par un algorithme de "noise" inclus dans la bibliothèque.
//
// Les teintes s'écrivent entre 0 et 360, comme sur une roue chromatique :
//     0 rouge     60 jaune    120 vert
//   180 cyan     240 bleu     300 magenta     360 rouge à nouveau
//
// Branchement : un anneau de 16 leds, "data in" sur la broche 6.

#include <LedAnimation.h>

LedRing ring1(16, 6);

void setup() {
  ring1.begin();
}

void loop() {
  timeline(20000);

  // --- de 0 à 4 secondes : le chatoiement s'allume ---
  //   .hue(160, 270)   les teintes ondulent entre 160 (cyan) et 270 (violet).
  //                    Ce sont les teintes par défaut : on pourrait ne pas l'écrire.
  //   .value(0, 255)   la luminosité générale monte de 0 à 255
  //   .ease(SIN_OUT)   la montée ralentit à la fin
  ring1.chatoiement().to(4000).hue(160, 270).value(0, 255).ease(SIN_OUT);

  // --- de 4 à 8 secondes : le chatoiement seul ---
  // Sans aucun réglage : teintes 160 à 270, pleine luminosité.
  ring1.chatoiement().from(4000).to(8000);

  // --- de 8 à 12 secondes : changement de couleur ---
  //   .hueTo(0, 40)   à la fin de l'animation les teintes ondulent entre 0 (rouge)
  //                   et 40 (orange). On passe donc progressivement de l'eau au feu.
  ring1.chatoiement().from(8000).to(12000).hueTo(0, 40);

  // --- de 12 à 16 secondes : plus nerveux ---
  // On repart des teintes d'arrivée de l'animation précédente : .hue(0, 40)
  //   .speed(4)   quatre fois plus rapide. Avec 0.25 ce serait quatre fois plus lent.
  ring1.chatoiement().from(12000).to(16000).hue(0, 40).speed(4);

  // --- de 16 à 20 secondes : le chatoiement s'éteint ---
  //   .value(255, 0)   la luminosité générale descend de 255 à 0
  ring1.chatoiement().from(16000).hue(0, 40).value(255, 0).ease(SIN_IN);

  ring1.show();
}

// A savoir :
// - des teintes proches (.hue(200, 220)) donnent une couleur presque unie dont
//   seule la luminosité ondule ; des teintes éloignées (.hue(0, 360)) donnent un
//   rendu multicolore.
// - on peut faire chatoyer seulement une partie de l'anneau avec .leds(),
//   par exemple deux couleurs différentes sur chaque moitié :
//     ring1.chatoiement().hue(0, 40).leds(FIRST_HALF);
//     ring1.chatoiement().hue(160, 270).leds(SECOND_HALF);

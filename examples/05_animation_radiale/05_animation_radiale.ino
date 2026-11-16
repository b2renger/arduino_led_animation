// 05 - Animation radiale
//
// Un dégradé de teintes réparti sur tout l'anneau, qui tourne.
// Aucune couleur à définir ici : on travaille avec des teintes, entre 0 et 360,
// comme sur une roue chromatique :
//
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
  timeline(15000);

  // --- de 0 à 5 secondes : tout par défaut ---
  // Sans aucun réglage : un dégradé du vert au bleu (teintes 100 à 235)
  // qui fait 8 tours pendant la durée de l'animation.
  ring1.animation_radiale().to(5000);

  // --- de 5 à 10 secondes : on choisit les teintes et le nombre de tours ---
  //   .hue(0, 60)   un dégradé du rouge (0) au jaune (60)
  //   .turns(2)     2 tours seulement : la rotation est donc plus lente
  ring1.animation_radiale().from(5000).to(10000).hue(0, 60).turns(2);

  // --- de 10 à 15 secondes : dans l'autre sens, avec une dynamique ---
  //   .hue(0, 360)       toute la roue chromatique : un arc-en-ciel
  //   .turns(-3)         un nombre négatif fait tourner dans l'autre sens
  //   .ease(SIN_IN_OUT)  la rotation démarre et s'arrête en douceur
  //   .value(255, 0)     la luminosité passe de 255 (maximum) à 0 : l'anneau s'éteint
  ring1.animation_radiale().from(10000).hue(0, 360).turns(-3).ease(SIN_IN_OUT).value(255, 0);

  ring1.show();
}

// A savoir :
// - le nombre de tours est le nombre de tours pendant toute la durée de l'animation :
//   pour une rotation plus rapide, augmentez .turns() ou raccourcissez l'animation.
// - .value(debut, fin) règle la luminosité au début et à la fin de l'animation.
//   Cela fonctionne avec toutes les animations, pour les allumer ou les éteindre
//   progressivement.

// 08 - Deux anneaux
//
// Chaque anneau a son nom et sa broche. Ils partagent la même timeline, on peut
// donc les synchroniser : faire la même chose en même temps, l'un après l'autre,
// ou deux choses complètement différentes.
//
// Branchement : deux anneaux de 16 leds, "data in" sur les broches 6 et 9.
// Au-delà de deux anneaux il faut une alimentation externe, cf. le tutoriel.

#include <LedAnimation.h>

Vec3 noir = Vec3(0, 0, 0);
Vec3 bleu = Vec3(0, 200, 255);
Vec3 orange = Vec3(255, 105, 0);

// Un anneau par ligne : son nom, son nombre de leds, sa broche.
LedRing ring1(16, 6);
LedRing ring2(16, 9);

void setup() {
  // chaque anneau doit être démarré
  ring1.begin();
  ring2.begin();
}

void loop() {
  // une seule timeline pour les deux anneaux
  timeline(12000);

  // --- de 0 à 4 secondes : la même animation, en même temps ---
  ring1.transition(noir, bleu).to(2000);
  ring2.transition(noir, bleu).to(2000);
  ring1.transition(bleu, noir).from(2000).to(4000);
  ring2.transition(bleu, noir).from(2000).to(4000);

  // --- de 4 à 8 secondes : la même animation, l'un après l'autre ---
  // L'anneau 2 fait la même chose que l'anneau 1, avec une seconde de retard.
  ring1.transition(noir, orange).from(4000).to(5000);
  ring1.transition(orange, noir).from(5000).to(6000);
  ring2.transition(noir, orange).from(5000).to(6000);
  ring2.transition(orange, noir).from(6000).to(7000);
  // Pour les animations qui s'enchaînent sur un anneau pendant que l'autre attend,
  // on s'assure que celui qui attend est bien noir.
  ring2.fixed_color(noir).from(4000).to(5000);
  ring1.fixed_color(noir).from(6000).to(8000);
  ring2.fixed_color(noir).from(7000).to(8000);

  // --- de 8 à 12 secondes : deux animations différentes ---
  // .value(0, 255) puis .value(255, 0) : chaque animation s'allume puis s'éteint.
  ring1.chatoiement().from(8000).to(10000).value(0, 255);
  ring1.chatoiement().from(10000).value(255, 0);
  ring2.animation_radiale().from(8000).to(10000).turns(2).value(0, 255);
  ring2.animation_radiale().from(10000).turns(2).value(255, 0);

  // chaque anneau doit être affiché
  ring1.show();
  ring2.show();
}

// A savoir :
// - les deux anneaux peuvent avoir un nombre de leds différent : LedRing ring2(24, 9);
// - un ruban de leds se déclare exactement comme un anneau.

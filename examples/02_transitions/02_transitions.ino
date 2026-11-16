// 02 - Transitions et couleurs fixes
//
// Une séquence complète de 8 secondes : une montée, des flashs, une descente.
// On y voit comment placer les animations les unes après les autres dans le temps,
// et ce qui se passe quand on ne précise pas .from() ou .to().
//
// Branchement : un anneau de 16 leds, "data in" sur la broche 6.

#include <LedAnimation.h>

Vec3 noir = Vec3(0, 0, 0);
Vec3 bleu = Vec3(0, 200, 255);
Vec3 rose = Vec3(220, 0, 120);

LedRing ring1(16, 6);  // nombre de leds, broche arduino

void setup() {
  ring1.begin();
}

void loop() {
  timeline(8000);  // la séquence dure 8 secondes puis recommence

  // --- de 0 à 2,5 secondes : montée du noir au bleu ---
  // .from() n'est pas écrit : par défaut une animation commence à 0.
  ring1.transition(noir, bleu).to(2500);

  // --- de 2,5 à 4 secondes : des flashs ---
  // fixed_color() affiche une couleur sans transition, pendant la durée demandée.
  ring1.fixed_color(noir).from(2500).to(2600);
  ring1.fixed_color(bleu).from(2600).to(2700);
  ring1.fixed_color(noir).from(2700).to(3000);
  ring1.fixed_color(rose).from(3000).to(3100);
  ring1.fixed_color(noir).from(3100).to(4000);

  // --- de 4 à 5 secondes : un flash répété 5 fois ---
  // Plutôt que d'écrire 10 lignes, une boucle for répète les deux mêmes lignes.
  // "debut" vaut 4000, puis 4200, 4400, 4600, 4800.
  for (int debut = 4000; debut < 5000; debut += 200) {
    ring1.fixed_color(bleu).from(debut).to(debut + 100);        // 100 ms allumé
    ring1.fixed_color(noir).from(debut + 100).to(debut + 200);  // 100 ms éteint
  }

  // --- de 5 à 7,5 secondes : descente du bleu au noir ---
  ring1.transition(bleu, noir).from(5000).to(7500);

  // --- de 7,5 secondes à la fin : noir ---
  // .to() n'est pas écrit : par défaut une animation dure jusqu'à la fin de la timeline.
  ring1.fixed_color(noir).from(7500);

  ring1.show();
}

// A savoir :
// - entre deux animations, s'il y a un "trou" dans le temps, les leds gardent
//   simplement leur dernière couleur.
// - si deux animations se jouent en même temps sur les mêmes leds, c'est la
//   dernière écrite dans le programme qui gagne.
// - écrivez les temps directement (60000) et pas sous forme de calcul (60 * 1000) :
//   sur une carte arduino uno ce calcul dépasse le plus grand nombre entier possible
//   et l'animation ne se jouera pas.

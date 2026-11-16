// 07 - Sunrise : une transition led par led
//
// transition() change toutes les leds en même temps.
// sunrise() change les leds les unes après les autres : la couleur part de la
// led 0 et fait le tour de l'anneau des deux côtés à la fois, jusqu'à la led
// opposée. Si la led 0 est en bas de l'anneau, la lumière monte comme un soleil
// qui se lève.
//
// Branchement : un anneau de 16 leds, "data in" sur la broche 6.

#include <LedAnimation.h>

Vec3 noir = Vec3(0, 0, 0);
Vec3 rouge = Vec3(255, 20, 0);
Vec3 jaune = Vec3(255, 170, 30);
Vec3 nuit = Vec3(0, 10, 60);

LedRing ring1(16, 6);

void setup() {
  ring1.begin();
}

void loop() {
  timeline(16000);

  // --- de 0 à 4 secondes : le lever du soleil ---
  // Chaque paire de leds passe du noir au rouge, l'une après l'autre.
  ring1.sunrise(noir, rouge).to(4000);

  // --- de 4 à 8 secondes : le jour se lève ---
  // Même principe, par dessus le rouge : les leds passent du rouge au jaune.
  //   .ease(SIN_OUT)   la progression ralentit en arrivant en haut de l'anneau
  ring1.sunrise(rouge, jaune).from(4000).to(8000).ease(SIN_OUT);

  // --- de 8 à 12 secondes : la nuit tombe ---
  // La nuit arrive elle aussi par le bas de l'anneau.
  ring1.sunrise(jaune, nuit).from(8000).to(12000);

  // --- de 12 à 16 secondes : retour au noir ---
  // Toutes les leds en même temps cette fois : c'est une transition classique.
  ring1.transition(nuit, noir).from(12000);

  ring1.show();
}

// A essayer :
// - une seule moitié de l'anneau : ring1.sunrise(noir, rouge).to(4000).leds(FIRST_HALF);
// - un lever de soleil très rapide suivi d'un long jour, en changeant les temps

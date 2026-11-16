// 01 - Première animation
//
// Le programme le plus court possible : un anneau qui passe du noir au bleu,
// puis du bleu au noir, en boucle.
//
// Branchement : un anneau de 16 leds, "data in" sur la broche 6.

// La bibliothèque : elle donne accès à LedRing, Vec3, timeline() et aux animations.
#include <LedAnimation.h>

// Les couleurs : on leur donne un nom pour pouvoir les réutiliser.
// Les trois nombres sont les quantités de rouge, de vert et de bleu, entre 0 et 255.
// Pour en ajouter une, copiez une ligne et changez le nom et les nombres.
Vec3 noir = Vec3(0, 0, 0);
Vec3 bleu = Vec3(0, 200, 255);

// L'anneau : on l'appelle "ring1", il a 16 leds et il est branché sur la broche 6.
LedRing ring1(16, 6);

// setup() est exécuté une seule fois, au démarrage de la carte.
void setup() {
  ring1.begin();  // démarrage de l'anneau
}

// loop() est exécuté en boucle, très vite, tant que la carte est alimentée.
void loop() {
  // La timeline : l'animation dure 5000 millisecondes (5 secondes) puis recommence.
  // Tous les temps s'écrivent en millisecondes : 1 seconde = 1000.
  timeline(5000);

  // Une animation s'écrit sur une ligne, en enchaînant des fonctions avec des points :
  //   ring1.                   sur l'anneau ring1
  //   transition(noir, bleu)   on passe du noir au bleu
  //   .from(0)                 l'animation commence à 0 ms
  //   .to(2500)                et se termine à 2500 ms
  ring1.transition(noir, bleu).from(0).to(2500);

  // Puis le chemin inverse pendant la seconde moitié de la timeline.
  ring1.transition(bleu, noir).from(2500).to(5000);

  // On envoie les couleurs aux leds : sans cette ligne rien ne s'allume.
  ring1.show();
}

// A essayer :
// - changer les nombres de la couleur bleu
// - ajouter une couleur et l'utiliser dans une des transitions
// - changer la durée de la timeline et les temps des deux transitions

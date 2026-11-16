// 09 - Ecrire ses propres animations
//
// Une animation est une fonction qui donne une couleur aux leds.
// Vous pouvez écrire les vôtres ici, dans votre programme, puis les jouer avec
// ring1.play(nom_de_la_fonction). Tous les réglages (.from() .to() .ease()
// .leds() .value() ...) fonctionnent alors comme pour les autres animations.
//
// Une animation s'écrit toujours sous cette forme :
//
//   void nom_de_la_fonction(Anim &a) {
//     ...
//   }
//
// Attention à bien écrire (Anim &a) avec le "&" : sans lui le programme peut
// se téléverser sans erreur mais l'animation ne fonctionnera pas.
//
// Dans la fonction, "a" donne accès à tout ce dont vous avez besoin :
//
//   a.n                  le nombre de leds de l'anneau
//   a.t                  l'avancement de l'animation, de 0 (début) à 1 (fin)
//   a.ms                 le temps écoulé depuis le début de l'animation, en millisecondes
//   a.mix(x, y)          un nombre qui va de x à y pendant l'animation
//   a.pixel(i, couleur)  donner une couleur à la led numéro i
//   a.fill(couleur)      donner une couleur à toutes les leds
//
// et une couleur s'écrit soit avec un Vec3 (rouge, vert, bleu), soit avec
//   hsv(teinte, saturation, luminosité)    teinte de 0 à 360, le reste de 0 à 255
//
// Branchement : un anneau de 16 leds, "data in" sur la broche 6.

#include <LedAnimation.h>

Vec3 noir = Vec3(0, 0, 0);
Vec3 blanc = Vec3(255, 255, 255);
Vec3 rose = Vec3(220, 0, 120);

LedRing ring1(16, 6);

////////////////////////////////////////////////////////////////////////
// Animation 1 : un arc-en-ciel qui s'allume
// La plus simple possible : une boucle for qui donne une couleur à chaque led.
void arc_en_ciel(Anim &a) {
  // la luminosité monte de 0 à 255 pendant l'animation
  float luminosite = a.mix(0, 255);
  // on répète pour chaque led : i vaut 0, puis 1, puis 2 ... jusqu'à la dernière
  for (int i = 0; i < a.n; i++) {
    // la teinte dépend du numéro de la led : 0 pour la première, presque 360 pour la dernière
    float teinte = 360. * i / a.n;
    a.pixel(i, hsv(teinte, 255, luminosite));
  }
}

////////////////////////////////////////////////////////////////////////
// Animation 2 : un clignotement
// Elle utilise le temps écoulé (a.ms) et les couleurs données avec .colors()
void clignotement(Anim &a) {
  // a.ms / 150 compte les tranches de 150 millisecondes : 0, 1, 2, 3 ...
  // "% 2" donne le reste de la division par 2 : 0, 1, 0, 1 ...
  // on a donc une alternance toutes les 150 millisecondes
  if ((a.ms / 150) % 2 == 0) {
    a.fill(a.color1);  // la première couleur de .colors()
  } else {
    a.fill(a.color2);  // la seconde couleur de .colors()
  }
}

////////////////////////////////////////////////////////////////////////
// Animation 3 : une comète qui tourne, avec une traîne qui s'estompe
// Elle utilise les réglages .hue() et .turns() : on peut donc changer sa
// couleur et sa vitesse au moment de la jouer, sans modifier la fonction.
void comete(Anim &a) {
  // la position de la tête de la comète, en nombre de leds parcourues :
  // a.t va de 0 à 1, a.nTurns est le nombre de tours demandé avec .turns()
  float tete = a.t * a.nTurns * a.n;
  // la teinte va de la première à la seconde valeur de .hue() pendant l'animation
  float teinte = a.mix(a.hue1, a.hue2);
  // la longueur de la traîne : la moitié de l'anneau
  float traine = a.n / 2.;

  for (int i = 0; i < a.n; i++) {
    // la distance entre la led i et la tête, en faisant le tour de l'anneau :
    // fmod calcule le reste d'une division, comme "%", mais pour les nombres à virgule
    float distance = fmod(tete - i, a.n);
    if (distance < 0) distance += a.n;
    // plus la led est loin derrière la tête, moins elle est lumineuse
    float luminosite = 255 * (1 - distance / traine);
    if (luminosite < 0) luminosite = 0;
    a.pixel(i, hsv(teinte, 255, luminosite));
  }
}

void setup() {
  ring1.begin();
}

void loop() {
  timeline(16000);

  // --- de 0 à 3 secondes : l'arc-en-ciel s'allume ---
  // On joue une animation personnelle avec play(), et on enchaîne les réglages.
  ring1.play(arc_en_ciel).to(3000).ease(SIN_IN);

  // --- de 3 à 5 secondes : clignotement ---
  // .colors() transmet deux couleurs à l'animation : a.color1 et a.color2
  ring1.play(clignotement).from(3000).to(5000).colors(rose, noir);

  // --- de 5 à 7 secondes : le même clignotement, autrement ---
  // Autres couleurs, et seulement une led sur deux : la fonction n'a pas changé.
  ring1.fixed_color(noir).from(5000).to(7000);
  ring1.play(clignotement).from(5000).to(7000).colors(blanc, noir).leds(EVEN);

  // --- de 7 à 12 secondes : la comète ---
  //   .hue(180, 300)   la comète passe du cyan au magenta : a.hue1 et a.hue2
  //   .turns(5)        elle fait 5 tours : a.nTurns
  ring1.play(comete).from(7000).to(12000).hue(180, 300).turns(5);

  // --- de 12 à 16 secondes : la comète ralentit et s'éteint ---
  //   .ease(SIN_OUT)   a.t avance de moins en moins vite : la comète ralentit
  //   .value(255, 0)   la luminosité générale descend, sans rien écrire dans la fonction
  ring1.play(comete).from(12000).hue(300, 300).turns(3).ease(SIN_OUT).value(255, 0);

  ring1.show();
}

// Les valeurs des réglages disponibles dans une animation :
//
//   .colors(c1, c2)   ->   a.color1   a.color2
//   .hue(h1, h2)      ->   a.hue1     a.hue2
//   .hueTo(h1, h2)    ->   a.hueTo1   a.hueTo2
//   .turns(k)         ->   a.nTurns
//   .speed(k)         ->   a.speedK
//
// Les réglages .from() .to() .ease() .leds() et .value() sont appliqués
// automatiquement : vous n'avez rien à écrire dans la fonction pour qu'ils
// fonctionnent.
//
// Les animations de la bibliothèque (transition, sunrise, chatoiement ...) sont
// écrites exactement de cette manière dans le fichier LedAnimation.h, partie 6 :
// vous pouvez les copier ici, les renommer et les modifier.

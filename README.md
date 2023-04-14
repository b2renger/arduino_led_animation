
A crash course focused on led animation with arduino for designers

Dans ce document nous allons nous concentrer sur la création d'animations basées sur une timeline qui se joueront sur des leds de type neopixels; 

Cette ressource n'a pas vocation à être une introduction à arduino, vous pourrez trouver une ressource plus générale sur arduino et suivant ce lien : https://github.com/b2renger/Introduction_Arduino

Il peut-être intéressant de lire l'introduction du lien ci-dessus si vous ne connaissez absolument pas arduino avant de continuer; notament si vous souhaitez découvrir l'anatomie d'une carte arduino, les principes de base des broches d'entrée et sortie, ainsi que le logiciel permettant de programmer les cartes.


<details>
  <summary><strong>TL;DR</strong> </summary>
  - brancher un anneau ou un ruban de 16 leds en utilisant la pin 6 pour le "data in"
  - avoir arduino et la bibliothèque neopixel d'adafruit installée

  ```c
// la bibliothèque pour les leds
#include <Adafruit_NeoPixel.h>

// la structure de données pour stocker les couleurs
struct Vec3 {
  float c1;
  float c2;
  float c3;
  Vec3()
    : c1(0), c2(0), c3(0) {}
  Vec3(const float c1, const float c2, const float c3)
    : c1(c1), c2(c2), c3(c3) {}
};

// quelques définitions de couleurs
Vec3 bleu = Vec3(0, 200, 255);
Vec3 orange = Vec3(255, 105, 0);
Vec3 noir = Vec3(0, 0, 0);
Vec3 rose = Vec3(220, 0, 120);

// initialisation des leds
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800); // nombre de leds, broche arduino, type de leds


void setup() {

  ring1.begin();// démarrage de l'anneau


}

void loop() {
  // gestion du temps
  long totalTime = 5000; // temps total de l'animation
  long dur = millis() % totalTime; // timing

  // créer des transitions à vous de jouer
  transition_lin(&ring1, NUMPIXELS, dur, 0, 2500, noir, bleu);
  transition_lin(&ring1, NUMPIXELS, dur, 2500, 5000, bleu, noir);

  ring1.show();
}

// toutes les transitions disponibles ci-dessous
void animation_radiale(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    int ledsOffset = map(currentTime, 0, 1000, 0, n * 8); // 8 représenter le nombre de tours pendant la durée de l'animation
    for (int i = 0 ; i < n ; i++) {
      float teinteHSB = map(i, 0, n, 100, 235); // on veut une teinte entre 100 et 235 pour notre dégradé 
      float teinteLeds = map(teinteHSB, 0, 360, 0, 65535); // on transforme dans le référentiel led 
      strip->setPixelColor((i + ledsOffset) % n, strip->gamma32(strip->ColorHSV(teinteLeds, 255, 255))); // on applique la couleur
    }
  }
}


// pas de transition : une couleur fixe pendant un durée
void fixed_color(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 c) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(c.c1, c.c2, c.c3));
    }
  }
}

// transition linéaire d'une couleur à une autre pendant une durée donnée
void transition_lin(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    float r = map(currentTime, 0, 1000, startC.c1, endC.c1);
    float g = map(currentTime, 0, 1000, startC.c2, endC.c2);
    float b = map(currentTime, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}

// transitions : https://easings.net/
// transition sine
void transition_sine_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = -(cos(PI * currentTime) - 1) / 2.;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_sine_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = 1 - cos((currentTime * PI) / 2.);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_sine_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = sin((currentTime * PI) / 2);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
// Transition circ
void transition_circ_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = currentTime < 0.5 ? (1 - sqrt(1 - pow(2 * currentTime, 2))) / 2 : (sqrt(1 - pow(-2 * currentTime + 2, 2)) + 1) / 2;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_circ_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = 1 - sqrt(1 - pow(currentTime, 2));
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_circ_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  sqrt(1 - pow(currentTime - 1, 2));
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
// Transition exponentielle
void transition_expo_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  currentTime == 0
               ? 0
               : currentTime == 1
               ? 1
               : currentTime < 0.5 ? pow(2, 20 * currentTime - 10) / 2
               : (2 - pow(2, -20 * currentTime + 10)) / 2;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_expo_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  currentTime == 1 ? 1 : 1 - pow(2, -10 * currentTime);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_expo_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =   currentTime == 0 ? 0 : pow(2, 10 * currentTime - 10);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
``` 
  

</details>


## Contenu

<!-- vscode-markdown-toc -->
1. [Brancher un anneau ou ruban de Leds](#BrancherunanneauourubandeLeds)
	* 1.1. [Installation de la bibliothèque](#Installationdelabibliothque)
	* 1.2. [Programmer les leds manuellement](#Programmerlesledsmanuellement)
	* 1.3. [Utiliser une boucle pour assigner toutes les leds](#Utiliserunebouclepourassignertouteslesleds)
	* 1.4. [Le mode de couleur HSB](#LemodedecouleurHSB)
2. [Programmation avancée](#Programmationavance)
	* 2.1. [Animation radiale](#Animationradiale)
	* 2.2. [Noise](#Noise)
3. [Créer des transitions](#Crerdestransitions)
	* 3.1. [Des structures de codes à savoir utiliser](#Desstructuresdecodessavoirutiliser)
		* 3.1.1. [Un structure pour stocker les couleurs](#Unstructurepourstockerlescouleurs)
		* 3.1.2. [Gérer le temps](#Grerletemps)
		* 3.1.3. [Des fonctions pour réaliser des transitions](#Desfonctionspourraliserdestransitions)
	* 3.2. [Transitions disponibles](#Transitionsdisponibles)
	* 3.3. [Transition simple](#Transitionsimple)
	* 3.4. [Un programme complet de transitions pour un anneau](#Unprogrammecompletdetransitionspourunanneau)
	* 3.5. [Alterner des transitions de couleurs pleines et des animations pixel par pixel](#Alternerdestransitionsdecouleurspleinesetdesanimationspixelparpixel)
4. [Brancher et assigner plusieurs anneaux](#Brancheretassignerplusieursanneaux)
	* 4.1. [Deux anneaux - alimentation par arduino](#Deuxanneaux:alimentationpararduino)
	* 4.2. [Deux anneaux - exemple complet](#Deuxanneaux:exemplecomplet)
	* 4.3. [Trois anneaux - alimentation externe](#Troisanneaux:alimentationexterne)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->



##  1. <a name='BrancherunanneauourubandeLeds'></a>Brancher un anneau ou ruban de Leds

Il faut comprendre qu'un anneau de leds consomme du courant et qu'une carte arduino ne peut fournir qu'une certaine quantité de courant, et donc à partir d'un certain nombre il faudra avoir recours à une alimentation externe. Mais nous détaillerons ça plus tard.

Commençons donc par brancher notre premier anneau, directement sur la carte :

![branchement simple en direct](./assets/fritzing_neopixel_1.PNG)

Nous pouvons maintenant ouvrir le logiciel [arduino](https://www.arduino.cc/en/software), afin de programmer !

[**^ Home**](#Contenu)

###  1.1. <a name='Installationdelabibliothque'></a>Installation de la bibliothèque

Afin de pouvoir adresser des couleurs aux leds il faut utiliser une bibliothèque logicielle dédiée au type de leds utiliser.

Il faut donc l'installer via **le gestionnaire de bibliothèques**, accessible depuis le menu *Croquis* -> *Inclure une bibliothèque* -> *Gérer les bibliothèques*.

![menu chargement de bibliothèque](./assets/menu_library.PNG)

Puis, dans le champ de recherche taper "neopixels" et choisir la bibliothèque *Adafruit NeoPixel*

![sélection de bibliothèque](./assets/gestionnaire_lib.PNG)

[**^ Home**](#Contenu)

###  1.2. <a name='Programmerlesledsmanuellement'></a>Programmer les leds manuellement

Pour commencer à produire de la lumière le plus simple est d'adresser les leds manuellement en utilisant l'index de la led que nous voulons éclairer. L'index étant un chiffre correspondant au numéro de la led sur l'anneau.


```c
// inclure la bibliothèque nécessaire
#include <Adafruit_NeoPixel.h>

// initialiser l'anneau de leds avec 16 leds connectés sur la pin 6
#define NUMPIXELS 16

Adafruit_NeoPixel ring1 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800); 
// notre anneau est maintenant représenté par l'alias "ring1"

void setup() {
  // intialiser l'anneau  
  ring1.begin(); 
}

void loop() {
    // donner une couleur à certains pixels en RGB (max = 255)
    // 0 -> rouge
    ring1.setPixelColor(0, ring1.Color(255, 0, 0)); 

    // 1 -> vert
    ring1.setPixelColor(1, ring1.Color(0, 255, 0)); 
    // 2 -> bleu
    ring1.setPixelColor(2, ring1.Color(0, 0, 255));
    // 3 -> blanc
    ring1.setPixelColor(3, ring1.Color(255, 255, 255)); 
    // 4 -> noir
    ring1.setPixelColor(4, ring1.Color(0, 0, 0)); // noir
    // 5 -> jaune
    ring1.setPixelColor(5, ring1.Color(255, 255, 0));
    // 6 -> rose
    ring1.setPixelColor(6, ring1.Color(255, 0, 255));
    // 7 -> turquoise
    ring1.setPixelColor(6, ring1.Color(0, 255, 255));
    // on actualise le ruban de led 
    ring1.show(); 
}
```

Vous devriez alors obtenir ce résultat :
![](./assets/exemple01.jpg)

Rien que pour 16 leds ce processus est un peu fastidieu; imaginez alors en avoir 32 ou 400 ! et ne parlons même pas des animations potentielles que nous souhaiterions créer ... 

[**^ Home**](#Contenu)

###  1.3. <a name='Utiliserunebouclepourassignertouteslesleds'></a>Utiliser une boucle pour assigner toutes les leds

En programmation nous pouvons utiliser une **boucle for** pour répéter une action plusieures fois.

Une boucle for s'écrit comme cela :

```c
for ( /* conditions à remplir */ ){
  /*
  code à éxécuter tant que les conditions sont vérifiées
  */
}
```

il est donc important de bien identifier les parenthèses - qui servent à délimiter les conditions d'éxécution du code, qui lui est entre accolades.

D'un point de vue pratique, les conditions vont être exprimée en 3 étapes :

- on va créer une variable (ici un nombre entier) et on va lui donner une valeur de démarrage :
  ```
  int cpt = 0;
  ```
  => notre variable s'appelle 'cpt' (pour compteur), et elle vaut zéro au début.

- on exprime ensuite la condition d'arrêt :
  ```
  cpt < 30;
  ```
  => si jamais i dépasse 29 on s'arrête et on sort de la boucle for; on continuera alors l'éxécution du programme de manière séquentielle (ligne par ligne).

- finalement on définit la manière dont notre compteur évolue après chaque éxécution du code entre accolades, par exemple :
  ```
  cpt = cpt + 5;
  ```
  => à chaque fois que le code est éxécuté on augmente notre variable cpt de 5.

Du coup, en écrivant : 
  ```c
  for (int cpt = 0 ; cpt < 30 ; cpt = cpt +5){

  }
  ```
Le code entre les accolades, sera éxécuté :
  - un première fois avec cpt valant 0
  - une deuxième fois avec cpt valant 5
  - etc
  - un cinquième fois avec cpt valant 25

A la sixième fois *cpt* vaudra 30 et ne sera donc plus strictement inférieur à 30. On sortira alors de la boucle pour éxécuter le code directement en dessous.

Si nous appliquons cela à nos leds, cela donne :

```c
// inclure la bibliothèque nécessaire
#include <Adafruit_NeoPixel.h>

// initialiser l'anneau de leds avec 16 leds connectés sur la pin 6
#define NUMPIXELS 16

Adafruit_NeoPixel ring1 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
// notre anneau est maintenant représenté par l'alias "ring1"

void setup() {
  // intialiser l'anneau
  ring1.begin();
}

void loop() {
  // on répéte une ligne de code pour chaque led
  // c'est à dire de la led à l'index 0 jusqu'à la led 15.
  for (int i = 0 ; i < NUMPIXELS ; i ++) {
    // pour la led à l'index i, on lui applique la couleur rouge
    ring1.setPixelColor(i, ring1.Color(255, 0, 0));
  }
  ring1.show();
}

```
Traditionellement, on utilise *i* car c'est plus court à écrire que *cpt*, mais on peut utiliser n'importe quel lettre ou mot.
![](./assets/exemple02.jpg)


Du coup on peut utiliser la valeur de notre compteur pour créer des dégradés. On peut utiliser la valeur que prend *i* à chaque répétition (dans notre cas elle augmente de 1), pour calculer d'autres nombres que l'on pourra utiliser pour nos composantes rgb.

```c
// inclure la bibliothèque nécessaire
#include <Adafruit_NeoPixel.h>

// initialiser l'anneau de leds avec 16 leds connectés sur la pin 6
#define NUMPIXELS 16

Adafruit_NeoPixel ring1 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
// notre anneau est maintenant représenté par l'alias "ring1"

void setup() {
  // intialiser l'anneau
  ring1.begin();
}

void loop() {
  // on répéte une ligne de code pour chaque led
  // c'est à dire de la led à l'index 0 jusqu'à la led 15.
  for (int i = 0 ; i < NUMPIXELS ; i ++) {
    // pour la led à l'index i, on lui applique une couleur
    // dominante bleue, mais dont la composante rouge augmente
    // au fur et à mesure jusqu'à 255 et dont la composante
    // bleue diminue jusqu'à 0
    // map permet de calculer pour i compris entre 0 et NUMPIXELS
    // une valeur a comprise entre 0 et 255.
    int a = map(i, 0, NUMPIXELS, 0, 255);
    int b = 255 - a;
    ring1.setPixelColor(i, ring1.Color(a, 0, b));
  }
  ring1.show();
}
```
Ici la fonction map permet de faire un calcul qui est une simple règle de proportionalité (ou règle de trois)

On range dans une variable appelée *a*, le résultat du calcul effectué par [*map*](https://www.arduino.cc/reference/en/language/functions/math/map/), puis on utilise cette variable pour la composante rouge de notre calcul.
On calcule aussi une variable appelée *b* qui sera le "miroir" de *a* par rapport à la valeur max de 255.

![](./assets/exemple02b.jpg)


[**^ Home**](#Contenu)

###  1.4. <a name='LemodedecouleurHSB'></a>Le mode de couleur HSB

Le mode RGB a ses limites, il permet de faire beaucoup de choses, mais il existe un autre mode appelé HSB pour : Hue Saturation Brightness. Cela signifie qu'en lieu et place de spécifier les composantes rouges, vertes et bleues, nous pouvons définir des couleur grâce à leur teinte, leur saturation et leur luminosité.

- la teinte est traditionellement exprimée entre 0 et 360 dans les sélécteurs de couleurs que vous pourez trouver en ligne. Ici elle sera exprimée entre 0 et 65535 (soit 2 puissance 16 pour avoir une finisse de sélection 16 bit).
- la saturation qui représente en réalité l'intensité des couleurs est traditionellement exprimée entre 0 et 100 ici elle sera exprimée entre 0 et 255. Si la saturation est à 0 nous aurons du blanc et si elle est à 255 nous aurons des couleurs pleines - très saturées.
- la luminosité est aussi traditionnellement exprimée entre 0 et 100 et encore une fois elle sera ici exprimée entre 0 et 255. Si la luminosité est à 0 nous aurons des leds éteintes (noires), si elle est à 255 nous aurons des leds allumées au maximum de leur puissance.

Il est alors très facile de créer un dégradé de teinte en utilisant une boucles for :

```c
// inclure la bibliothèque nécessaire
#include <Adafruit_NeoPixel.h>

// initialiser l'anneau de leds avec 16 leds connectés sur la pin 6
#define NUMPIXELS 16

Adafruit_NeoPixel ring1 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
// notre anneau est maintenant représenté par l'alias "ring1"

void setup() {
  // intialiser l'anneau
  ring1.begin();
}

void loop() {
  // on répéte une ligne de code pour chaque led
  // c'est à dire de la led à l'index 0 jusqu'à la led 15.
  for (int i = 0 ; i < NUMPIXELS ; i ++) {
    int teinte = map(i, 0, NUMPIXELS, 0, 65535);
    uint32_t rgbcolor = ring1.ColorHSV(teinte, 255,255);
    ring1.setPixelColor(i, rgbcolor);
  }
  ring1.show();
}
```

![](assets/exemple_03.jpg)

Il est aussi possible de faire un dégradé de saturation ou de luminosité. Mais attention obtenir un dégradé qui perceptuellement soit valable il faut utiliser une correction gamma (suivez [ce lien pour en savoir plus](https://learn.adafruit.com/led-tricks-gamma-correction)).

C'est souvent valable pour des dégradés dans les couleurs pastelles.


```c
// inclure la bibliothèque nécessaire
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 16

// initialiser l'anneau de leds avec 16 leds connectés sur la pin 6
Adafruit_NeoPixel ring1 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
// notre anneau est maintenant représenté par l'alias "ring1"

void setup() {
  // intialiser l'anneau
  ring1.begin();
}

void loop() {
  // on répéte une ligne de code pour chaque led
  // c'est à dire de la led à l'index 0 jusqu'à la led 15.
  for (int i = 0 ; i < NUMPIXELS ; i ++) {
    int sat = map(i, 0, NUMPIXELS, 0, 255);
    //uint32_t rgbcolor = ring1.ColorHSV(0, sat,255); // sans correction
    uint32_t rgbcolor = ring1.gamma32(ring1.ColorHSV(0, sat, 255));// avec correction
    ring1.setPixelColor(i, rgbcolor);
  }
  ring1.show();
}
```
Sans correction :

![](assets/exemple03_nogamma.jpg)

Avec correction :

![](assets/exemple03_gamma.jpg)


[**^ Home**](#Contenu)


##  2. <a name='Programmationavance'></a>Programmation avancée
Nous allons maintenant nous concentrer sur des animations un peu plus complexes qui vont assigner des valeurs différentes à chaque led d'un anneau.

###  2.1. <a name='Animationradiale'></a>Animation radiale

Nous allons réaliser cette animation :

![](./assets/exemple_05_radial.gif)

L'idée est assez simple nous reprenons le principe du dégradé en mode HSB sur l'anneau complet.

Nous allons ensuite créer un décalage (ou *offset*) qui lui va être changé par une timeline.

Il va falloir ensuite que nous attribuons la bonne teinte au bon pixel. Nous allons ajouter le *i* de notre boucle for à notre variable *offset* qui va donc augmenter de 1 entre 0 et le nombre de pixels que nous avons (*NUMPIXELS*).

Il faut cependant que le résultat de *i+offset* reste compris entre 0 et NUMPIXELS. L'astuce pour cela est d'utiliser l'opérateur *modulo* exprimé par le caractère '%' qui calcule le reste de la division euclidienne (explication simple à venir ...)

En gros si je fais un modulo 16 sur une valeur qui augmente de 1 à chaque calcul : j'obtiendrai un compteur qui augmente de 1 à chaque fois et lorsque j'arrive à 16 ou des multiples de 16 le résultat sera toujours zéro :

Si je fais mes divisions :
- 0 / 16 => cela vaut zéro et le reste vaut 0.
- 1 / 16 => cela vaut toujours zéro mais le reste vaut 1. 
- 2 / 16 => toujours zéro mais le reste vaut 2.
- ...
- 15 / 16 => toujours zéro mais le reste vaut 15.
- 16 / 16 => la division vaut 1 et le reste vaut 0.
- 17 / 16 => 1 et le reste vaut 1.
- ...
- 31 / 16 => 1 et le reste vaut 15.
- 32 / 16 => 2 et le reste vaut 0.
- etc.

Du coup :
```
(i + offset) % NUMPIXELS
```
donnera toujours un résultat entre 0 et NUMPIXELS ce qui était notre but.


[**^ Home**](#Contenu)

###  2.2. <a name='Noise'></a>Noise

Nous allons maintenant nous intéresser à une autre technique, permettant d'obtenir des rendu plus organiques / ondulants.

![](./assets/exemple_05_noise.gif)

L'idée est d'utiliser un algorithme de noise (qui permet d'obtenir des nombres aléatoires cohérents entre eux).
Pour vous donner une idée de ce que cela peu donner graphiquement, vous pouvez regarder ce tableau pinterest : https://www.pinterest.fr/vigo_spooliga/perlin-noise/

Pour cela nous allons encore importer un nouvelle bibliothèque qui n'est pas non plus disponnible dans le gestionnaire arduino.

Elle se trouve ici : https://github.com/jshaw/SimplexNoise

Comme pour la bibliothèque *Tween* vous pouvez cliquer sur le bouton vert *Code* et choisir *dowload as zip*.

Puis dans arduino vous sélectionnez le menu *Croquis* puis *inclure une bibliothèque* et *ajouter la bibliothèqe .ZIP* pour pouvoir choisir le fichier zip que vous venez de télécharger.

D'un point de vue code, c'est un peu plus compliqué à utiliser ...

Dans l'animation ci-dessus nous controllons une variation de teinte dans des couleurs bleutées et nous controllons aussi la luminosité générale de chacun des pixels.

Le *noise* est un algorithme qui dépend du temps, il va donc falloir lui fournir des variables qui augmentent lentement, plus ces variables augmentent longtemps, plus le résultat sera "souple" visuellement, plus elles augmentent vite plus le résultat sera "sacadé".

```c

#include <Adafruit_NeoPixel.h>
#include <SimplexNoise.h>

#define PIN        6
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// initialiser la bibliothèque pour le noise
SimplexNoise sn;

double nx;
double x = 0.0;

double ny;
double y = 0.0;


void setup() {

  ring1.begin(); 

  // obtenir des valeurs aléatoires au lancement du programme
  x= analogRead(0)/10.;
  y= analogRead(2)/ 10.;
}

void loop() {

  // on augmente les variables x et y lentement
  x += 0.005;
  y += 0.001;

  // on parcourt tous les pixels
  for (int i = 0; i < NUMPIXELS; i++) {

    // on calcule une valeur de noise (comprise entre 0 et 1)
    nx = sn.noise(x, i); 
    // on retransforme cette valeur en une valeur comprise entre 0 et 255
    int br = map(nx * 100, -100, 100, 0, 255); 

    // on fait de même avec notre seconde valeur.
    ny = sn.noise(y, i*2);
    // on retransforme pour avoir des valeurs entre 160 et 270
    // référentiel de la roue chromatique
    int h = map(ny*100, -100, 100, 160, 270);
    // on retransforme enfin pour avoir des valeurs entre 0 et 65535
    // dans le référentiel des anneaux de leds.
    int h1 = map(h, 0, 360, 0, 65535);
    
    ring1.setPixelColor(i, ring1.gamma32(ring1.ColorHSV (h1, 255, br)));
  }
  ring1.show();   


}
```
Ici vous pouvez jouer avec différentes valeurs pour obtenir des rendus différents.
Par exemple :

- *x += 0.005;* : vous pouvez changer la valeur 0.005 par une valeur plus grande ou plus petite
- idem pour *y += 0.001;*
- *nx = sn.noise(x, i);* : essayez de remplacer *i* par *i*5* ou *i*0.25* et d'autre valeurs
- idem pour *ny = sn.noise(y, i*2);*


[**^ Home**](#Contenu)

##  3. <a name='Crerdestransitions'></a>Créer des transitions

Pour créer des transitions nous allons utiliser du code qu'il n'est pas nécessaire de comprendre, mais qu'il est nécessaire de savoir utiliser !

Nous allons quand même détailler un peu le code à des fins pédagogiques pour ceux qui veulent aller un peu plus loin.

L'idée est d'avoir une structure de donnée permettant de stocker les couleurs sous forme de variables et d'avoir une série de fonctions qui auront des effets directes sur un anneau de led.

Vous trouverez à la fin de cette section un exemple de code complet et fonctionnel que vous n'aurez qu'à modifier plus tard.



[**^ Home**](#Contenu)

###  3.1. <a name='Desstructuresdecodessavoirutiliser'></a>Des structures de codes à savoir utiliser

####  3.1.1. <a name='Unstructurepourstockerlescouleurs'></a>Un structure pour stocker les couleurs

Une structure permet de créer un nouveau type de données. Ces données seront des vecteurs à trois composantes car les couleurs ont trois composantes que l'on soit en RGB ou en HSB.

Voici à quoi va ressembler notre structure de données :
```c
struct Vec3 {
  float c1;
  float c2;
  float c3;
  Vec3()
    : c1(0), c2(0), c3(0) {}
  Vec3(const float c1, const float c2, const float c3)
    : c1(c1), c2(c2), c3(c3) {}
};
```

Ces quelques lignes de code doivent être ajoutées tout au début de votre programme juste après avoir importé la bibliothèque adafruit pour les leds neopixel.

Cette structure va nous permettre de pouvoir définir des couleurs de cette manière :

```c
Vec3 bleu = Vec3(0, 200, 255);
Vec3 orange = Vec3(255, 105, 0);
Vec3 noir = Vec3(0, 0, 0);
Vec3 rose = Vec3(220, 0, 120);
```

Plus tard nous pourrons du coup directement utiliser les noms des couleurs pour les afficher sur nos anneaux.
Il suffit de copier/coller une ligne et de modifier les composantes à votre guise que ce soit en RGB ou en HSB.

Pour trouver des couleurs exprimées en rgb ou hsb vous pouvez vous rendre ici :
https://color.adobe.com/fr/create/color-wheel

[**^ Home**](#Contenu)

####  3.1.2. <a name='Grerletemps'></a>Gérer le temps

Il faudra arriver à gérer les timing des animations pour cela nous allons exprimer tous les temps en millisecondes.

Le premier truc qu'il nous faut sont des variables capables de stocker le temps écoulé depuis le début du programme et de boucler sur un temps défini.

Ces deux lignes de code seront à copier au début de la fonction loop() :

```c
long totalTime = 5000;
long dur = millis() % totalTime;
```

*totalTime* : permet de définir le temps au bout duquel on souhaite boucler; ici 5000 ms soit 5 secondes. Vous pouvez donc changer la valeur 5000 à la valeur nécessaire pour votre animation.

*dur* : est le temps actuel nous allons l'utiliser pour le passer à nos fonction d'animation pour que celles-ci puissent s'executer dans les timings définis.

[**^ Home**](#Contenu)

####  3.1.3. <a name='Desfonctionspourraliserdestransitions'></a>Des fonctions pour réaliser des transitions

Nous allons pouvoir réaliser différents type de transitions, d'une manière générale nous allons utiliser des équations mathématiques qui s'appellent des courbes de easing et qui permettent d'avoir des transitions avec des dynamiques différentes.

D'une manière générale le code d'une fonction de easing ressemble à cela :

```c
// transition linéaire d'une couleur à une autre pendant une durée donnée
void transition_lin(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    float r = map(currentTime, 0, 1000, startC.c1, endC.c1);
    float g = map(currentTime, 0, 1000, startC.c2, endC.c2);
    float b = map(currentTime, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
```
Il n'est pas nécessaire de le comprendre intégralement, mais il faut comprendre comment l'utiliser :)


Nous disposerons de différentes fonctions, mais il est possible d'en créer d'autres.

Pour avoir accès au différentes transitions vous devez copier/coller le code ci-dessous dans votre programme, tout en bas sous l'accolade fermante du *void loop(){}* :

```c

// pas de transition : une couleur fixe pendant un durée
void fixed_color(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 c) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(c.c1, c.c2, c.c3));
    }
  }
}

// transition linéaire d'une couleur à une autre pendant une durée donnée
void transition_lin(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    float r = map(currentTime, 0, 1000, startC.c1, endC.c1);
    float g = map(currentTime, 0, 1000, startC.c2, endC.c2);
    float b = map(currentTime, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}

// transitions : https://easings.net/
// transition sine
void transition_sine_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = -(cos(PI * currentTime) - 1) / 2.;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_sine_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = 1 - cos((currentTime * PI) / 2.);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_sine_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = sin((currentTime * PI) / 2);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
// Transition circ
void transition_circ_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = currentTime < 0.5 ? (1 - sqrt(1 - pow(2 * currentTime, 2))) / 2 : (sqrt(1 - pow(-2 * currentTime + 2, 2)) + 1) / 2;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_circ_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = 1 - sqrt(1 - pow(currentTime, 2));
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_circ_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  sqrt(1 - pow(currentTime - 1, 2));
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
// Transition exponentielle
void transition_expo_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  currentTime == 0
                      ? 0
                      : currentTime == 1
                               ? 1
                               : currentTime < 0.5 ? pow(2, 20 * currentTime - 10) / 2
                               : (2 - pow(2, -20 * currentTime + 10)) / 2;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_expo_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  currentTime == 1 ? 1 : 1 - pow(2, -10 * currentTime);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_expo_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =   currentTime == 0 ? 0 : pow(2, 10 * currentTime - 10);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}

```


###  3.2. <a name='Transitionsdisponibles'></a>Transitions disponibles

Une fois le code ci-dessus copié/collé, vous pouvez réaliser des transitions.

Les fonctions disponibles sont alors :
- fixed_color : pour une couleur fixe
- transition_lin : pour une transition linéaire d'une couleur à une autre.
- transition_sine_InOut
- transition_sine_In
- transition_sine_Out
- transition_circ_InOut
- transition_circ_In
- transition_circ_Out
- transition_expo_InOut
- transition_expo_In
- transition_expo_Out

Vous pouvez vous référer à cette page pour comprendre comment fonctionnent les différentes fonctions de easing : https://easings.net/


[**^ Home**](#Contenu)


###  3.3. <a name='Transitionsimple'></a>Transition simple

Ici nous avons réalisé des transitions linéaires, mais il est possible d'utiliser des courbes de **easing** qui permettent de gérer des accélération et ralentissements.


Quand on a le code ci-dessus dans notre programme, on peut alors écrire :

```c
transition_lin(&ring1, NUMPIXELS, dur, 0, 2500, noir, bleu);
```

Pour créer une transition lumineuse :
  - &ring1 -> sur les leds nommées ring1
  - NUMPIXELS -> 16 leds dans notre cas - cf initialisation des leds
  - dur -> le temps actuel
  - 0 -> la transition démarre quand dur vaut 0 ms
  - 2500 -> elle se finie quand dur vaut 2500 ms
  - elle commence au noir
  - elle finie au bleu

La plupart des transitions fonctionnent de cette façon avec ces mêmes arguments à préciser entre les parenthèse.

[**^ Home**](#Contenu)

###  3.4. <a name='Unprogrammecompletdetransitionspourunanneau'></a>Un programme complet de transitions pour un anneau
![](./assets/transition.gif)

```c
// la bibliothèque pour les leds
#include <Adafruit_NeoPixel.h>

// la structure de données pour stocker les couleurs
struct Vec3 {
  float c1;
  float c2;
  float c3;
  Vec3()
    : c1(0), c2(0), c3(0) {}
  Vec3(const float c1, const float c2, const float c3)
    : c1(c1), c2(c2), c3(c3) {}
};

// quelques définitions de couleurs
Vec3 bleu = Vec3(0, 200, 255);
Vec3 orange = Vec3(255, 105, 0);
Vec3 noir = Vec3(0, 0, 0);
Vec3 rose = Vec3(220, 0, 120);

// initialisation des leds
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800); // nombre de leds, broche arduino, type de leds


void setup() {

  ring1.begin();// démarrage de l'anneau

}

void loop() {
  // gestion du temps
  long totalTime = 8000; // temps total de l'animation
  long dur = millis() % totalTime; // timing

  // créer des transitions

  // transition linéaires au bleu
  transition_lin(&ring1, NUMPIXELS, dur, 0, 2500, noir, bleu);

  // flash de couleurs fixes
  fixed_color(&ring1, NUMPIXELS, dur, 2500, 2600, noir);
  fixed_color(&ring1, NUMPIXELS, dur, 2700, 2800, bleu);
  fixed_color(&ring1, NUMPIXELS, dur, 2900, 3000, noir);
  fixed_color(&ring1, NUMPIXELS, dur, 3000, 3100, rose);
  fixed_color(&ring1, NUMPIXELS, dur, 3100, 3200, noir);
  fixed_color(&ring1, NUMPIXELS, dur, 3200, 3300, rose);
  fixed_color(&ring1, NUMPIXELS, dur, 3300, 3400, bleu);
  fixed_color(&ring1, NUMPIXELS, dur, 3400, 3500, rose);
  fixed_color(&ring1, NUMPIXELS, dur, 3500, 4000, noir);
  // flashs rapides (on utilise une boucle for pour répéter plusieures fois un flash) sur une durée totale d'une seconde
  for (int i = 0 ; i < 20 ; i += 4) {
    fixed_color(&ring1, NUMPIXELS, dur, 4000 + i * 50, 4000 + (i + 1) * 50, bleu);
    fixed_color(&ring1, NUMPIXELS, dur, 4000 + (i + 2) * 50, 4000 + (i + 3) * 50, noir);
  }
  //retour au noir
  transition_sine_Out(&ring1, NUMPIXELS, dur, 5000, 7500, bleu, noir);
  fixed_color(&ring1, NUMPIXELS, dur, 7500, 8000, noir);

  ring1.show();


}

// pas de transition : une couleur fixe pendant un durée
void fixed_color(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 c) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(c.c1, c.c2, c.c3));
    }
  }
}

// transition linéaire d'une couleur à une autre pendant une durée donnée
void transition_lin(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    float r = map(currentTime, 0, 1000, startC.c1, endC.c1);
    float g = map(currentTime, 0, 1000, startC.c2, endC.c2);
    float b = map(currentTime, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}

// transitions : https://easings.net/
// transition sine
void transition_sine_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = -(cos(PI * currentTime) - 1) / 2.;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_sine_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = 1 - cos((currentTime * PI) / 2.);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_sine_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = sin((currentTime * PI) / 2);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
// Transition circ
void transition_circ_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = currentTime < 0.5 ? (1 - sqrt(1 - pow(2 * currentTime, 2))) / 2 : (sqrt(1 - pow(-2 * currentTime + 2, 2)) + 1) / 2;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_circ_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = 1 - sqrt(1 - pow(currentTime, 2));
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_circ_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  sqrt(1 - pow(currentTime - 1, 2));
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
// Transition exponentielle
void transition_expo_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  currentTime == 0
               ? 0
               : currentTime == 1
               ? 1
               : currentTime < 0.5 ? pow(2, 20 * currentTime - 10) / 2
               : (2 - pow(2, -20 * currentTime + 10)) / 2;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_expo_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  currentTime == 1 ? 1 : 1 - pow(2, -10 * currentTime);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_expo_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =   currentTime == 0 ? 0 : pow(2, 10 * currentTime - 10);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}

```
[**^ Home**](#Contenu)

###  3.5. <a name='Alternerdestransitionsdecouleurspleinesetdesanimationspixelparpixel'></a>Alterner des transitions de couleurs pleines et des animations pixel par pixel

En vous basant sur la manière dont les transitions ci-dessus sont écrites il devrait être possible d'écrire des animations spécifiques comme vues dans la partie "programmation avancée" !

Par exemple en considérant cette fonction générique on peut facilement l'adapter  pour exécuter le code souhaité :
```c
void animation_specifique (Adafruit_NeoPixel *strip, int n, float t, float startT, float endT) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);



    /*
    * Ecrire ici du code spécifique à votre animation
    * vous pouvez notament utiliser le mode HSB
    */
    
  }
}
```

Ici pour réaliser une animation de dégradé radial en rotation !

```c
void animation_radiale(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    int ledsOffset = map(currentTime, 0, 1000, 0, n * 8); // 8 représenter le nombre de tours pendant la durée de l'animation
    for (int i = 0 ; i < n ; i++) {
      float teinteHSB = map(i, 0, n, 100, 235); // on veut une teinte entre 100 et 235 pour notre dégradé 
      float teinteLeds = map(teinteHSB, 0, 360, 0, 65535); // on transforme dans le référentiel led 
      strip->setPixelColor((i + ledsOffset) % n, strip->gamma32(strip->ColorHSV(teinteLeds, 255, 255))); // on applique la couleur
    }
  }
}
``` 

[**^ Home**](#Contenu)

##  4. <a name='Brancheretassignerplusieursanneaux'></a>Brancher et assigner plusieurs anneaux

###  4.1. <a name='Deuxanneaux:alimentationpararduino'></a>Deux anneaux : alimentation par arduino

Pour brancher plusieurs anneaux en même temps, vous pouvez suivre le schéma de cablage suivant :

![](./assets/fritzing_neopixel_2.PNG)

Nous avons donc deux anneaux branchés sur les broches 6 et 9, il est donc possible de définir deux objets de type *Adafruit_NeoPixel* que l'on pourra par exemple appeler *ring1* et *ring2* :

```c
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800); // nombre de leds, broche arduino, type de leds
Adafruit_NeoPixel ring2(NUMPIXELS, 9, NEO_GRB + NEO_KHZ800);
```

Ensuite il nous reste à adapter le reste du code pour afficher des choses sur les deux anneaux en passant le nom de l'anneau aux fonctions d'animation :)

###  4.2. <a name='Deuxanneaux:exemplecomplet'></a>Deux anneaux : exemple complet

![](./assets/2rings.gif)

```c
// la bibliothèque pour les leds
#include <Adafruit_NeoPixel.h>

// la structure de données pour stocker les couleurs
struct Vec3 {
  float c1;
  float c2;
  float c3;
  Vec3()
    : c1(0), c2(0), c3(0) {}
  Vec3(const float c1, const float c2, const float c3)
    : c1(c1), c2(c2), c3(c3) {}
};

// quelques définitions de couleurs
Vec3 bleu = Vec3(0, 200, 255);
Vec3 orange = Vec3(255, 105, 0);
Vec3 noir = Vec3(0, 0, 0);
Vec3 rose = Vec3(220, 0, 120);

// initialisation des leds
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800); // nombre de leds, broche arduino, type de leds
Adafruit_NeoPixel ring2(NUMPIXELS, 9, NEO_GRB + NEO_KHZ800);


void setup() {

  ring1.begin();// démarrage de l'anneau
  ring2.begin();

}

void loop() {
  // gestion du temps
  long totalTime = 5000; // temps total de l'animation
  long dur = millis() % totalTime; // timing

  // créer des transitions

  // transitions linéaires
  transition_lin(&ring1, NUMPIXELS, dur, 0, 2500, noir, bleu);
  transition_lin(&ring1, NUMPIXELS, dur, 2500, 5000, bleu, noir);

  animation_radiale(&ring2, NUMPIXELS, dur, 0, 5000);

  ring1.show();
  ring2.show();

}

void animation_radiale(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    int ledsOffset = map(currentTime, 0, 1000, 0, n * 8); // 8 représenter le nombre de tours pendant la durée de l'animation
    for (int i = 0 ; i < n ; i++) {
      float teinteHSB = map(i, 0, n, 100, 235); // on veut une teinte entre 100 et 235 pour notre dégradé 
      float teinteLeds = map(teinteHSB, 0, 360, 0, 65535); // on transforme dans le référentiel led 
      strip->setPixelColor((i + ledsOffset) % n, strip->gamma32(strip->ColorHSV(teinteLeds, 255, 255))); // on applique la couleur
    }
  }
}


// pas de transition : une couleur fixe pendant un durée
void fixed_color(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 c) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(c.c1, c.c2, c.c3));
    }
  }
}

// transition linéaire d'une couleur à une autre pendant une durée donnée
void transition_lin(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    float r = map(currentTime, 0, 1000, startC.c1, endC.c1);
    float g = map(currentTime, 0, 1000, startC.c2, endC.c2);
    float b = map(currentTime, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}

// transitions : https://easings.net/
// transition sine
void transition_sine_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = -(cos(PI * currentTime) - 1) / 2.;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_sine_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = 1 - cos((currentTime * PI) / 2.);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_sine_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = sin((currentTime * PI) / 2);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
// Transition circ
void transition_circ_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = currentTime < 0.5 ? (1 - sqrt(1 - pow(2 * currentTime, 2))) / 2 : (sqrt(1 - pow(-2 * currentTime + 2, 2)) + 1) / 2;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_circ_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t = 1 - sqrt(1 - pow(currentTime, 2));
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_circ_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  sqrt(1 - pow(currentTime - 1, 2));
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
// Transition exponentielle
void transition_expo_InOut(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  currentTime == 0
               ? 0
               : currentTime == 1
               ? 1
               : currentTime < 0.5 ? pow(2, 20 * currentTime - 10) / 2
               : (2 - pow(2, -20 * currentTime + 10)) / 2;
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_expo_Out(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =  currentTime == 1 ? 1 : 1 - pow(2, -10 * currentTime);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
void transition_expo_In(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 startC, Vec3 endC) {
  if (t > startT && t < endT) {
    float currentTime = map(t, startT, endT, 0, 1000) / 1000.;
    float t =   currentTime == 0 ? 0 : pow(2, 10 * currentTime - 10);
    float r = map(t * 1000, 0, 1000, startC.c1, endC.c1);
    float g = map(t * 1000, 0, 1000, startC.c2, endC.c2);
    float b = map(t * 1000, 0, 1000, startC.c3, endC.c3);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(r, g, b));
    }
  }
}
``` 

[**^ Home**](#Contenu)

###  4.3. <a name='Troisanneaux:alimentationexterne'></a>Trois anneaux : alimentation externe

Pour ajouter une troisième source de lumière il faudra avoir recours à une alimentation externe pour avoir plus de courant disponible. Voici le schéma de branchement qui vous permettra de réaliser le circuit électrique nécessaire.

![](./assets/fritzing_neopixel_3.PNG)

Normalement en vous inspirant du code pour deux anneaux vous devriez réussir à l'adapter pour 3 anneaux, car vous savez déjà beaucoup de choses !

Pour dimensionner une alimentation il faut compter qu'une led consomme au maximum 60mA lorsqu'elle est allumée en blanc. Théoriquement si on allume toutes les leds d'un anneau en blanc on consomme donc 0,96 A. Pour deux anneaux 1,92 A et pour 3 anneaux.


[**^ Home**](#Contenu)


Les différentes transitions possibles basées sur cette page : https://easings.net/

```c
fixed_color(ring1, 16, dur, 1000, 2000, bleu);

transition_lin(ring1, 16, dur, 1000, 2000,noir, bleu);

transition_sine_InOut(ring1, 16, dur, 1000, 2000,noir, bleu);
transition_sine_In(ring1, 16, dur, 1000, 2000,noir, bleu);
transition_sine_Out(ring1, 16, dur, 1000, 2000,noir, bleu);

transition_circ_InOut(ring1, 16, dur, 1000, 2000,noir, bleu);
transition_circ_In(ring1, 16, dur, 1000, 2000,noir, bleu);
transition_circ_Out(ring1, 16, dur, 1000, 2000,noir, bleu);

transition_expo_InOut(ring1, 16, dur, 1000, 2000,noir, bleu);
transition_expo_In(ring1, 16, dur, 1000, 2000,noir, bleu);
transition_sine_Out(ring1, 16, dur, 1000, 2000,noir, bleu);
```

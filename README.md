
A crash course focused on led animation with arduino for designers

Dans ce document nous allons nous concentrer sur la création d'animations basées sur une timeline qui se joueront sur des leds de type neopixels; 

Cette ressource n'a pas vocation à être une introduction à arduino, vous pourrez trouver une ressource plus générale sur arduino et suivant ce lien : https://github.com/b2renger/Introduction_Arduino

Il peut-être intéressant de lire l'introduction du lien ci-dessus si vous ne connaissez absolument pas arduino avant de continuer; notament si vous souhaitez découvrir l'anatomie d'une carte arduino, les principes de base des broches d'entrée et sortie, ainsi que le logiciel permettant de programmer les cartes.




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

## Créer des transitions

Pour créer des transitions nous allons utiliser du code qu'il n'est pas nécessaire de comprendre, mais qu'il est nécessaire de savoir utiliser !

Nous allons quand même détailler un peu le code à des fins pédagogiques pour ceux qui veulent aller un peu plus loin.

L'idée est d'avoir une structure de donnée permettant de stocker les couleurs sous forme de variables et d'avoir une série de fonctions qui auront des effets directes sur un anneau de led.

Vous trouverez à la fin de cette section un exemple de code complet et fonctionnel que vous n'aurez qu'à modifier plus tard.



[**^ Home**](#Contenu)

### Des structures de codes à savoir utiliser

#### Un structure pour stocker les couleurs

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

#### Gérer le temps

[**^ Home**](#Contenu)

#### Des fonctions pour réaliser des transitions

[**^ Home**](#Contenu)


###  2.2. <a name='Transitionsimple'></a>Transition simple


Ici nous avons réalisé des transitions linéaires, mais il est possible d'utiliser des courbes de **easing** qui permettent de gérer des accélération et ralentissements.

Vous pouvez vous référer à cette page pour comprendre comment fonctionnent les différentes fonctions de easing : https://easings.net/




[**^ Home**](#Contenu)

##  3. <a name='Programmationavance'></a>Programmation avancée
Nous allons maintenant nous concentrer sur des animations un peu plus complexes qui vont assigner des valeurs différentes à chaque led d'un anneau.

###  3.1. <a name='Animationradiale'></a>Animation radiale

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

###  3.2. <a name='Noise'></a>Noise

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

###  3.3. <a name='Mlangertimelineetanimations'></a>Mélanger timeline et animations

Le dernier exemple avancé va vous permettre de mélanger des éléments liés à la timeline et des animations pixel par pixel.


![](./assets/exemple_05_complete.gif)




[**^ Home**](#Contenu)

##  4. <a name='Brancheretassignerplusieursanneaux'></a>Brancher et assigner plusieurs anneaux

###  4.1. <a name='Deuxanneauxalimentationpararduino'></a>Deux anneaux : alimentation par arduino

Pour brancher plusieurs anneaux en même temps, vous pouvez suivre le schéma de cablage suivant :

![](./assets/fritzing_neopixel_2.PNG)

Le code va un peu se complexifier mais aussi se simplifier en même temps car nous allons devoir créer une structure de données pour pouvoir manipuler les composantes des couleurs.

Cette structure a juste pour but de stocker des couleurs et de permettre à la timeline de fonctionner avec plus de composantes.

Il vous faudra copier ce code tout en haut de votre programme pour en profiter (il n'est pas nécessaire de tout comprendre - mais il faut s'avoir l'utiliser !) :

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


Ce bout de code va nous permettre de créer des variables d'un nouveau type qui representeront une couleur et seront manipulables par notre timeline. 

Par exemple je peux créer la couleur orange en écrivant :
```c
Vec3 orange = Vec3(255, 105, 0);
``` 
Ensuite je pourrai accéder à la composante rouge de la couleur orange en écrivant *orange.c1*



Vous pourez trouver ci-dessous un exemple de code commenté permettant de créer cette animation :

![](./assets/exemple_06_rings.gif)

```c


``` 

[**^ Home**](#Contenu)

###  4.2. <a name='Troisanneauxalimentationexterne'></a>Trois anneaux : alimentation externe

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

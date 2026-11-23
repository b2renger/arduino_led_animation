🇫🇷 [traduction française](README.md)

A crash course focused on led animation with arduino for designers

In this document we will focus on creating timeline-based animations that will play on neopixel-type LEDs;

This resource is not intended to be an introduction to arduino, you can find a more general resource on arduino by following this link: https://github.com/b2renger/Introduction_Arduino

It may be interesting to read the introduction from the link above if you are not familiar with arduino at all before continuing; especially if you want to discover the anatomy of an arduino board, the basic principles of input and output pins, as well as the software used to program the boards.


## TL;DR

- connect a ring or strip of 16 LEDs using pin 6 for "data in"
- install the *Adafruit NeoPixel* library from the arduino library manager
- download this repository as a zip (green *Code* button then *Download ZIP*) and add it in arduino through the menu *Sketch* -> *Include Library* -> *Add .ZIP Library*
- open an example from the menu *File* -> *Examples* -> *LedAnimation*: they are commented line by line (in French) and numbered from the simplest to the most advanced

```c
#include <LedAnimation.h>

// some color definitions
// you can add others in RGB format
Vec3 bleu = Vec3(0, 200, 255);
Vec3 noir = Vec3(0, 0, 0);

LedRing ring1(16, 6);  // number of LEDs, arduino pin
//LedRing ring2(16, 9);

void setup() {
  ring1.begin();  // start the ring
}

void loop() {
  timeline(5000);  // total animation time: it loops every 5 seconds

  // create transitions, your turn to play!
  ring1.transition(noir, bleu).from(0).to(2500);                   // from black to blue between 0 and 2.5 seconds
  ring1.transition(bleu, noir).from(2500).to(5000).ease(SIN_OUT);  // back to black with a different dynamic

  // or
  //ring1.animation_radiale();
  //ring1.chatoiement().hue(160, 270);
  //ring1.sunrise(noir, bleu).leds(FIRST_HALF);

  ring1.show();
}
```

You chain functions separated by dots: everything you don't specify takes a default value. All the possibilities are listed in section [5. The LedAnimation library](#5-the-ledanimation-library).

The rest of this document explains how all of this works. The old "everything in one file" versions are still available in [code/tldr_code](code/tldr_code/tldr_code.ino) and [code/tldr_code_chatoiement](code/tldr_code_chatoiement/tldr_code_chatoiement.ino).


## Contents

<!-- vscode-markdown-toc -->
1. [Connecting an LED ring or strip](#1-connecting-an-led-ring-or-strip)
	* 1.1. [Library installation](#11-library-installation)
	* 1.2. [Programming LEDs manually](#12-programming-leds-manually)
	* 1.3. [Using a loop to assign all LEDs](#13-using-a-loop-to-assign-all-leds)
	* 1.4. [HSB color mode](#14-hsb-color-mode)
2. [Advanced programming](#2-advanced-programming)
	* 2.1. [Radial animation](#21-radial-animation)
	* 2.2. [Noise](#22-noise)
3. [Creating transitions](#3-creating-transitions)
	* 3.1. [Code structures to know how to use](#31-code-structures-to-know-how-to-use)
		* 3.1.1. [A structure to store colors](#311-a-structure-to-store-colors)
		* 3.1.2. [Managing time](#312-managing-time)
		* 3.1.3. [Functions to create transitions](#313-functions-to-create-transitions)
	* 3.2. [Available transitions](#32-available-transitions)
	* 3.3. [Simple transition](#33-simple-transition)
	* 3.4. [A complete transition program for a ring](#34-a-complete-transition-program-for-a-ring)
	* 3.5. [Alternating solid color transitions and pixel-by-pixel animations](#35-alternating-solid-color-transitions-and-pixel-by-pixel-animations)
4. [Connecting and assigning multiple rings](#4-connecting-and-assigning-multiple-rings)
	* 4.1. [Two rings - arduino power supply](#41-two-rings---arduino-power-supply)
	* 4.2. [Two rings - complete example](#42-two-rings---complete-example)
	* 4.3. [Three rings - external power supply](#43-three-rings---external-power-supply)
5. [The LedAnimation library](#5-the-ledanimation-library)
	* 5.1. [Installation and examples](#51-installation-and-examples)
	* 5.2. [Structure of a program](#52-structure-of-a-program)
	* 5.3. [The animations](#53-the-animations)
	* 5.4. [The settings](#54-the-settings)
	* 5.5. [Choosing the LEDs](#55-choosing-the-leds)
	* 5.6. [Writing your own animation](#56-writing-your-own-animation)
	* 5.7. [Pitfalls to avoid](#57-pitfalls-to-avoid)
	* 5.8. [Differences with the code from sections 3 and 4](#58-differences-with-the-code-from-sections-3-and-4)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->



##  1. <a name='ConnectinganLEDringorstrip'></a>Connecting an LED ring or strip

You need to understand that an LED ring consumes current and an arduino board can only provide a certain amount of current, so after a certain number you will need to use an external power supply. But we'll detail that later.

So let's start by connecting our first ring, directly to the board:

![simple direct connection](./assets/fritzing_neopixel_1.PNG)

We can now open the [arduino](https://www.arduino.cc/en/software) software to program!

[**^ Home**](#contents)

###  1.1. <a name='Libraryinstallation'></a>Library installation

In order to address colors to the LEDs, you need to use a software library dedicated to the type of LEDs being used.

You need to install it via the **library manager**, accessible from the *Sketch* menu -> *Include Library* -> *Manage Libraries*.

![library loading menu](./assets/menu_library.PNG)

Then, in the search field type "neopixels" and choose the *Adafruit NeoPixel* library

![library selection](./assets/gestionnaire_lib.PNG)

[**^ Home**](#contents)

###  1.2. <a name='ProgrammingLEDsmanually'></a>Programming LEDs manually

To start producing light, the simplest approach is to address the LEDs manually using the index of the LED we want to light up. The index being a number corresponding to the LED number on the ring.


```c
// include the necessary library
#include <Adafruit_NeoPixel.h>

// initialize the LED ring with 16 LEDs connected to pin 6
#define NUMPIXELS 16

Adafruit_NeoPixel ring1 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800); 
// our ring is now represented by the alias "ring1"

void setup() {
  // initialize the ring  
  ring1.begin(); 
}

void loop() {
    // give a color to certain pixels in RGB (max = 255)
    // 0 -> red
    ring1.setPixelColor(0, ring1.Color(255, 0, 0)); 

    // 1 -> green
    ring1.setPixelColor(1, ring1.Color(0, 255, 0)); 
    // 2 -> blue
    ring1.setPixelColor(2, ring1.Color(0, 0, 255));
    // 3 -> white
    ring1.setPixelColor(3, ring1.Color(255, 255, 255)); 
    // 4 -> black
    ring1.setPixelColor(4, ring1.Color(0, 0, 0)); // black
    // 5 -> yellow
    ring1.setPixelColor(5, ring1.Color(255, 255, 0));
    // 6 -> pink
    ring1.setPixelColor(6, ring1.Color(255, 0, 255));
    // 7 -> turquoise
    ring1.setPixelColor(6, ring1.Color(0, 255, 255));
    // we update the LED strip 
    ring1.show(); 
}
```

You should then get this result:
![](./assets/exemple01.jpg)

Just for 16 LEDs this process is a bit tedious; imagine having 32 or 400! And let's not even talk about potential animations we might want to create... 

[**^ Home**](#contents)

###  1.3. <a name='UsingalooptoassignallLEDs'></a>Using a loop to assign all LEDs

In programming we can use a **for loop** to repeat an action multiple times.

A for loop is written like this:

```c
for ( /* conditions to be met */ ){
  /*
  code to execute as long as the conditions are verified
  */
}
```

it is therefore important to identify the parentheses - which serve to delimit the execution conditions of the code, which is between braces.

From a practical point of view, the conditions will be expressed in 3 steps:

- we will create a variable (here an integer) and give it a starting value:
  ```
  int cpt = 0;
  ```
  => our variable is called 'cpt' (for counter), and it equals zero at the beginning.

- we then express the stopping condition:
  ```
  cpt < 30;
  ```
  => if cpt ever exceeds 29 we stop and exit the for loop; we will then continue program execution sequentially (line by line).

- finally we define how our counter evolves after each execution of the code between braces, for example:
  ```
  cpt = cpt + 5;
  ```
  => each time the code is executed we increase our variable cpt by 5.

So, by writing: 
  ```c
  for (int cpt = 0 ; cpt < 30 ; cpt = cpt +5){

  }
  ```
The code between the braces will be executed:
  - a first time with cpt equal to 0
  - a second time with cpt equal to 5
  - etc
  - a fifth time with cpt equal to 25

On the sixth time *cpt* will equal 30 and will therefore no longer be strictly less than 30. We will then exit the loop to execute the code directly below.

If we apply this to our LEDs, we get:

```c
// include the necessary library
#include <Adafruit_NeoPixel.h>

// initialize the LED ring with 16 LEDs connected to pin 6
#define NUMPIXELS 16

Adafruit_NeoPixel ring1 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
// our ring is now represented by the alias "ring1"

void setup() {
  // initialize the ring
  ring1.begin();
}

void loop() {
  // we repeat a line of code for each LED
  // that is from the LED at index 0 to LED 15.
  for (int i = 0 ; i < NUMPIXELS ; i ++) {
    // for the LED at index i, we apply the color red
    ring1.setPixelColor(i, ring1.Color(255, 0, 0));
  }
  ring1.show();
}

```
Traditionally, we use *i* because it's shorter to write than *cpt*, but we can use any letter or word.
![](./assets/exemple02.jpg)


So we can use the value of our counter to create gradients. We can use the value that *i* takes at each repetition (in our case it increases by 1), to calculate other numbers that we can use for our rgb components.

```c
// include the necessary library
#include <Adafruit_NeoPixel.h>

// initialize the LED ring with 16 LEDs connected to pin 6
#define NUMPIXELS 16

Adafruit_NeoPixel ring1 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
// our ring is now represented by the alias "ring1"

void setup() {
  // initialize the ring
  ring1.begin();
}

void loop() {
  // we repeat a line of code for each LED
  // that is from the LED at index 0 to LED 15.
  for (int i = 0 ; i < NUMPIXELS ; i ++) {
    // for the LED at index i, we apply a color
    // dominant blue, but whose red component increases
    // gradually up to 255 and whose blue component
    // decreases to 0
    // map allows us to calculate for i between 0 and NUMPIXELS
    // a value a between 0 and 255.
    int a = map(i, 0, NUMPIXELS, 0, 255);
    int b = 255 - a;
    ring1.setPixelColor(i, ring1.Color(a, 0, b));
  }
  ring1.show();
}
```
Here the map function allows us to do a calculation which is a simple rule of proportionality (or rule of three)

We store in a variable called *a*, the result of the calculation performed by [*map*](https://www.arduino.cc/reference/en/language/functions/math/map/), then we use this variable for the red component of our calculation.
We also calculate a variable called *b* which will be the "mirror" of *a* relative to the max value of 255.

![](./assets/exemple02b.jpg)


[**^ Home**](#contents)

###  1.4. <a name='HSBcolormode'></a>HSB color mode

RGB mode has its limitations, it allows you to do many things, but there is another mode called HSB for: Hue Saturation Brightness. This means that instead of specifying red, green and blue components, we can define colors through their hue, saturation and brightness.

- hue is traditionally expressed between 0 and 360 in color selectors you can find online. Here it will be expressed between 0 and 65535 (i.e. 2 to the power of 16 to have 16-bit selection finesse).
- saturation which actually represents the intensity of colors is traditionally expressed between 0 and 100 here it will be expressed between 0 and 255. If saturation is 0 we will have white and if it is 255 we will have full colors - very saturated.
- brightness is also traditionally expressed between 0 and 100 and again it will be expressed here between 0 and 255. If brightness is 0 we will have LEDs turned off (black), if it is 255 we will have LEDs turned on at maximum power.

![](./assets/HSV_schema.png)

It is then very easy to create a hue gradient using a for loop:

```c
// include the necessary library
#include <Adafruit_NeoPixel.h>

// initialize the LED ring with 16 LEDs connected to pin 6
#define NUMPIXELS 16

Adafruit_NeoPixel ring1 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
// our ring is now represented by the alias "ring1"

void setup() {
  // initialize the ring
  ring1.begin();
}

void loop() {
  // we repeat a line of code for each LED
  // that is from the LED at index 0 to LED 15.
  for (int i = 0 ; i < NUMPIXELS ; i ++) {
    int teinte = map(i, 0, NUMPIXELS, 0, 65535);
    uint32_t rgbcolor = ring1.ColorHSV(teinte, 255,255);
    ring1.setPixelColor(i, rgbcolor);
  }
  ring1.show();
}
```

![](assets/exemple_03.jpg)

It is also possible to create a saturation or brightness gradient. But be careful to obtain a perceptually valid gradient you need to use gamma correction (follow [this link to learn more](https://learn.adafruit.com/led-tricks-gamma-correction)).

This is often valid for gradients in pastel colors.


```c
// include the necessary library
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 16

// initialize the LED ring with 16 LEDs connected to pin 6
Adafruit_NeoPixel ring1 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
// our ring is now represented by the alias "ring1"

void setup() {
  // initialize the ring
  ring1.begin();
}

void loop() {
  // we repeat a line of code for each LED
  // that is from the LED at index 0 to LED 15.
  for (int i = 0 ; i < NUMPIXELS ; i ++) {
    int sat = map(i, 0, NUMPIXELS, 0, 255);
    //uint32_t rgbcolor = ring1.ColorHSV(0, sat,255); // without correction
    uint32_t rgbcolor = ring1.gamma32(ring1.ColorHSV(0, sat, 255));// with correction
    ring1.setPixelColor(i, rgbcolor);
  }
  ring1.show();
}
```
Without correction:

![](assets/exemple03_nogamma.jpg)

With correction:

![](assets/exemple03_gamma.jpg)


[**^ Home**](#contents)


##  2. <a name='Advancedprogramming'></a>Advanced programming
We will now focus on slightly more complex animations that will assign different values to each LED of a ring.

###  2.1. <a name='Radialanimation'></a>Radial animation

We will create this animation:

![](./assets/exemple_05_radial.gif)

The idea is quite simple, we take the gradient principle in HSB mode on the complete ring.

We will then create an offset that will be changed by a timeline.

We will then need to assign the right hue to the right pixel. We will add the *i* from our for loop to our *offset* variable which will therefore increase by 1 between 0 and the number of pixels we have (*NUMPIXELS*).

However, the result of *i+offset* must remain between 0 and NUMPIXELS. The trick for this is to use the *modulo* operator expressed by the '%' character which calculates the remainder of the Euclidean division (simple explanation to come...)

Basically if I do a modulo 16 on a value that increases by 1 with each calculation: I will get a counter that increases by 1 each time and when I reach 16 or multiples of 16 the result will always be zero:

If I do my divisions:
- 0 / 16 => this equals zero and the remainder is 0.
- 1 / 16 => this still equals zero but the remainder is 1. 
- 2 / 16 => still zero but the remainder is 2.
- ...
- 15 / 16 => still zero but the remainder is 15.
- 16 / 16 => the division equals 1 and the remainder is 0.
- 17 / 16 => 1 and the remainder is 1.
- ...
- 31 / 16 => 1 and the remainder is 15.
- 32 / 16 => 2 and the remainder is 0.
- etc.

So:
```
(i + offset) % NUMPIXELS
```
will always give a result between 0 and NUMPIXELS which was our goal.


[**^ Home**](#contents)

###  2.2. <a name='Noise'></a>Noise

We will now look at another technique, allowing for more organic/undulating renders.

![](./assets/exemple_05_noise.gif)

The idea is to use a noise algorithm (which allows you to get random numbers that are coherent with each other).
To give you an idea of what this can give graphically, you can look at this pinterest board: https://www.pinterest.fr/vigo_spooliga/perlin-noise/

For this we will import another new library which is also not available in the arduino manager.

It can be found here: https://github.com/jshaw/SimplexNoise

Like for the *Tween* library you can click on the green *Code* button and choose *download as zip*.

Then in arduino you select the *Sketch* menu then *include a library* and *add the .ZIP library* to be able to choose the zip file you just downloaded.

From a code point of view, it's a bit more complicated to use...

In the animation above we control a hue variation in blue colors and we also control the general brightness of each pixel.

*Noise* is an algorithm that depends on time, so we will need to provide it with variables that increase slowly, the longer these variables increase, the more "smooth" the result will be visually, the faster they increase, the more "jerky" the result will be.

```c

#include <Adafruit_NeoPixel.h>
#include <SimplexNoise.h>

#define PIN        6
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// initialize the library for noise
SimplexNoise sn;

double nx;
double x = 0.0;

double ny;
double y = 0.0;


void setup() {

  ring1.begin(); 

  // get random values at program startup
  x= analogRead(0)/10.;
  y= analogRead(2)/ 10.;
}

void loop() {

  // we increase variables x and y slowly
  x += 0.005;
  y += 0.001;

  // we go through all pixels
  for (int i = 0; i < NUMPIXELS; i++) {

    // we calculate a noise value (between 0 and 1)
    nx = sn.noise(x, i); 
    // we transform this value to a value between 0 and 255
    int br = map(nx * 100, -100, 100, 0, 255); 

    // we do the same with our second value.
    ny = sn.noise(y, i*2);
    // we transform to have values between 160 and 270
    // color wheel reference frame
    int h = map(ny*100, -100, 100, 160, 270);
    // we finally transform to have values between 0 and 65535
    // in the LED ring reference frame.
    int h1 = map(h, 0, 360, 0, 65535);
    
    ring1.setPixelColor(i, ring1.gamma32(ring1.ColorHSV (h1, 255, br)));
  }
  ring1.show();   


}
```
Here you can play with different values to get different renders.
For example:

- *x += 0.005;* : you can change the value 0.005 to a larger or smaller value
- same for *y += 0.001;*
- *nx = sn.noise(x, i);* : try replacing *i* with *i*5* or *i*0.25* and other values
- same for *ny = sn.noise(y, i*2);*


[**^ Home**](#contents)

##  3. <a name='Creatingtransitions'></a>Creating transitions

> **This section explains how transitions work "under the hood".** All the code it details is now gathered in the *LedAnimation* library, which is much simpler to use: see section [5. The LedAnimation library](#5-the-ledanimation-library). Do not mix the two ways of doing things in the same program: the code from this section must not be copied into a program that contains `#include <LedAnimation.h>`.

To create transitions we will use code that is not necessary to understand, but it is necessary to know how to use!

We will still detail the code a bit for educational purposes for those who want to go a little further.

The idea is to have a data structure to store colors as variables and to have a series of functions that will have direct effects on an LED ring.

You will find at the end of this section a complete and functional code example that you will only need to modify later.



[**^ Home**](#contents)

###  3.1. <a name='Codestructurestoknowhowtouse'></a>Code structures to know how to use

####  3.1.1. <a name='Astructuretostorecolors'></a>A structure to store colors

A structure allows you to create a new data type. This data will be three-component vectors because colors have three components whether in RGB or HSB.

Here's what our data structure will look like:
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

These few lines of code must be added at the very beginning of your program just after importing the adafruit library for neopixel LEDs.

This structure will allow us to define colors this way:

```c
Vec3 blue = Vec3(0, 200, 255);
Vec3 orange = Vec3(255, 105, 0);
Vec3 black = Vec3(0, 0, 0);
Vec3 pink = Vec3(220, 0, 120);
```

Later we can directly use the color names to display them on our rings.
Just copy/paste a line and modify the components as you wish whether in RGB or HSB.

To find colors expressed in rgb or hsb you can go here:
https://color.adobe.com/en/create/color-wheel

[**^ Home**](#contents)

####  3.1.2. <a name='Managingtime'></a>Managing time

We will need to manage animation timings, for this we will express all times in milliseconds.

The first thing we need are variables capable of storing the time elapsed since the beginning of the program and looping over a defined time.

These two lines of code should be copied at the beginning of the loop() function:

```c
long totalTime = 5000;
long dur = millis() % totalTime;
```

*totalTime* : allows you to define the time after which we want to loop; here 5000 ms or 5 seconds. You can therefore change the value 5000 to the value needed for your animation.

*dur* : is the current time we will use it to pass to our animation functions so that they can execute in the defined timings.

[**^ Home**](#contents)

####  3.1.3. <a name='Functionstocreatetransitions'></a>Functions to create transitions

We will be able to create different types of transitions, generally we will use mathematical equations called easing curves that allow transitions with different dynamics.

Generally the code for an easing function looks like this:

```c
// linear transition from one color to another over a given duration
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
It's not necessary to understand it completely, but you need to understand how to use it :)


We will have different functions available, but it's possible to create others.

To have access to the different transitions you must copy/paste the code below into your program, at the very bottom under the closing brace of *void loop(){}*:

```c

// no transition: a fixed color for a duration
void fixed_color(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 c) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(c.c1, c.c2, c.c3));
    }
  }
}

// linear transition from one color to another over a given duration
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

// transitions: https://easings.net/
// sine transition
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
// Circular transition
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
// Exponential transition
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


###  3.2. <a name='Availabletransitions'></a>Available transitions

Once the code above is copied/pasted, you can create transitions.

The available functions are then:
- fixed_color : for a fixed color
- transition_lin : for a linear transition from one color to another.
- transition_sine_InOut
- transition_sine_In
- transition_sine_Out
- transition_circ_InOut
- transition_circ_In
- transition_circ_Out
- transition_expo_InOut
- transition_expo_In
- transition_expo_Out

You can refer to this page to understand how the different easing functions work: https://easings.net/


[**^ Home**](#contents)


###  3.3. <a name='Simpletransition'></a>Simple transition

Here we created linear transitions, but it's possible to use **easing** curves that allow you to manage acceleration and deceleration.


When we have the code above in our program, we can then write:

```c
transition_lin(&ring1, NUMPIXELS, dur, 0, 2500, black, blue);
```

To create a light transition:
  - &ring1 -> on the LEDs named ring1
  - NUMPIXELS -> 16 LEDs in our case - cf LED initialization
  - dur -> the current time
  - 0 -> the transition starts when dur equals 0 ms
  - 2500 -> it ends when dur equals 2500 ms
  - it starts at black
  - it ends at blue

Most transitions work this way with these same arguments to specify between the parentheses.

[**^ Home**](#contents)

###  3.4. <a name='Acompletetransitionprogramforaring'></a>A complete transition program for a ring
![](./assets/transition.gif)

```c
// the library for LEDs
#include <Adafruit_NeoPixel.h>

// the data structure to store colors
struct Vec3 {
  float c1;
  float c2;
  float c3;
  Vec3()
    : c1(0), c2(0), c3(0) {}
  Vec3(const float c1, const float c2, const float c3)
    : c1(c1), c2(c2), c3(c3) {}
};

// some color definitions
Vec3 blue = Vec3(0, 200, 255);
Vec3 orange = Vec3(255, 105, 0);
Vec3 black = Vec3(0, 0, 0);
Vec3 pink = Vec3(220, 0, 120);

// LED initialization
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800); // number of LEDs, arduino pin, LED type


void setup() {

  ring1.begin();// start the ring

}

void loop() {
  // time management
  long totalTime = 8000; // total animation time
  long dur = millis() % totalTime; // timing

  // create transitions

  // linear transitions to blue
  transition_lin(&ring1, NUMPIXELS, dur, 0, 2500, black, blue);

  // color flashes
  fixed_color(&ring1, NUMPIXELS, dur, 2500, 2600, black);
  fixed_color(&ring1, NUMPIXELS, dur, 2700, 2800, blue);
  fixed_color(&ring1, NUMPIXELS, dur, 2900, 3000, black);
  fixed_color(&ring1, NUMPIXELS, dur, 3000, 3100, pink);
  fixed_color(&ring1, NUMPIXELS, dur, 3100, 3200, black);
  fixed_color(&ring1, NUMPIXELS, dur, 3200, 3300, pink);
  fixed_color(&ring1, NUMPIXELS, dur, 3300, 3400, blue);
  fixed_color(&ring1, NUMPIXELS, dur, 3400, 3500, pink);
  fixed_color(&ring1, NUMPIXELS, dur, 3500, 4000, black);
  // quick flashes (we use a for loop to repeat several flashes) over a total duration of one second
  for (int i = 0 ; i < 20 ; i += 4) {
    fixed_color(&ring1, NUMPIXELS, dur, 4000 + i * 50, 4000 + (i + 1) * 50, blue);
    fixed_color(&ring1, NUMPIXELS, dur, 4000 + (i + 2) * 50, 4000 + (i + 3) * 50, black);
  }
  //return to black
  transition_sine_Out(&ring1, NUMPIXELS, dur, 5000, 7500, blue, black);
  fixed_color(&ring1, NUMPIXELS, dur, 7500, 8000, black);

  ring1.show();


}

// no transition: a fixed color for a duration
void fixed_color(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 c) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(c.c1, c.c2, c.c3));
    }
  }
}

// linear transition from one color to another over a given duration
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

// transitions: https://easings.net/
// sine transition
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
// Circular transition
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
// Exponential transition
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
[**^ Home**](#contents)

###  3.5. <a name='Alternatingsolidcolortransitionsandpixel-by-pixelanimations'></a>Alternating solid color transitions and pixel-by-pixel animations

Based on how the transitions above are written, it should be possible to write specific animations as seen in the "advanced programming" section!

For example by considering this generic function we can easily adapt it to execute the desired code:
```c
void specific_animation (Adafruit_NeoPixel *strip, int n, float t, float startT, float endT) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);



    /*
    * Write here code specific to your animation
    * you can notably use HSB mode
    */
    
  }
}
```

Here to create a rotating radial gradient animation!

```c
void radial_animation(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    int ledsOffset = map(currentTime, 0, 1000, 0, n * 8); // 8 represents the number of turns during the animation duration
    for (int i = 0 ; i < n ; i++) {
      float teinteHSB = map(i, 0, n, 100, 235); // we want a hue between 100 and 235 for our gradient 
      float teinteLeds = map(teinteHSB, 0, 360, 0, 65535); // we transform to the LED reference frame 
      strip->setPixelColor((i + ledsOffset) % n, strip->gamma32(strip->ColorHSV(teinteLeds, 255, 255))); // we apply the color
    }
  }
}
``` 
Here to create a function from the Noise example (here shimmer), where `x` and `y` are incremented slowly in the `loop()`:
```c
void shimmer(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, float h1, float h2) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    x += 0.005; // x and y only increment during the animation
    y += 0.001;
    for (int i = 0; i < n; i++) {
      nx = sn.noise(x, i);
      int valeur = map(nx * 100, -100, 100, 0, 255);  //the LED value shimmers between 0 and 255
      ny = sn.noise(y, i * 2);
      int teinte = map(ny * 100, -100, 100, h1, h2);                                   //the hue is between values h1 and h2 in the function arguments
      int teinte1 = map(teinte, 0, 360, 0, 65535);                                     // we transform to the LED reference frame
      strip->setPixelColor(i, strip->gamma32(strip->ColorHSV(teinte1, 255, valeur)));  // we apply the color
    }
  }
}
```

Here from the previous shimmer function to create a transition in the total value (= brightness) of the ring to turn off or turn on the ring in the shimmer:
```c
void shimmer_value(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, float h1, float h2, float valeur1, float valeur2) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    x += 0.005; // x and y only increment during the animation
    y += 0.001;
    for (int i = 0; i < n; i++) {
      nx = sn.noise(x, i);
      float valeur = map(currentTime, 0, 1000, valeur1, valeur2);     //the value goes from valeur1 to valeur2 during the transition duration
      float noiseValeur = map(nx * 100, -100, 100, 0, 1000) / 1000.;  //we create noise between 0 and 1 for the final value
      ny = sn.noise(y, i * 2);
      int teinte = map(ny * 100, -100, 100, h1, h2);  //the hue is between values h1 and h2 in the function arguments
      int teinte1 = map(teinte, 0, 360, 0, 65535);
      strip->setPixelColor(i, strip->gamma32(strip->ColorHSV(teinte1, 255, noiseValeur * valeur)));  // we apply the color with the value that shimmers and goes from valeur1 to valeur2
    }
  }
}

```

Here from the previous shimmer function to create a color transition (= hue) to change the color in the shimmer:
```c
void shimmer_color(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, float h_debut_1, float h_debut_2, float h_fin_1, float h_fin_2) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    x += 0.005; // x and y only increment during the animation
    y += 0.001;
    for (int i = 0; i < n; i++) {
      nx = sn.noise(x, i);
      int valeur = map(nx * 100, -100, 100, 0, 255);  //the LED value shimmers between 0 and 255
      ny = sn.noise(y, i * 2);
      int h1 = map(currentTime, 0, 1000, h_debut_1, h_fin_1);
      int h2 = map(currentTime, 0, 1000, h_debut_2, h_fin_2);
      int teinte = map(ny * 100, -100, 100, h1, h2);  //
      int teinteFinale = map(teinte, 0, 360, 0, 65535);
      strip->setPixelColor(i, strip->gamma32(strip->ColorHSV(teinteFinale, 255, valeur)));  // we apply the color with the value that shimmers and goes from valeur1 to valeur2
    }
  }
```

[**^ Home**](#contents)

##  4. <a name='Connectingandassigningmultiplerings'></a>Connecting and assigning multiple rings

###  4.1. <a name='Tworings-arduinopowersupply'></a>Two rings - arduino power supply

To connect several rings at the same time, you can follow the wiring diagram below:

![](./assets/fritzing_neopixel_2.PNG)

We therefore have two rings connected to pins 6 and 9, so it is possible to define two objects of type *Adafruit_NeoPixel* that we can for example call *ring1* and *ring2*:

```c
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800); // number of LEDs, arduino pin, LED type
Adafruit_NeoPixel ring2(NUMPIXELS, 9, NEO_GRB + NEO_KHZ800);
```

Then we need to adapt the rest of the code to display things on both rings by passing the ring name to the animation functions :)

###  4.2. <a name='Tworings-completeexample'></a>Two rings - complete example

> The same principle with the *LedAnimation* library: example [08_deux_anneaux](examples/08_deux_anneaux/08_deux_anneaux.ino).

![](./assets/2rings.gif)

```c
// the library for LEDs
#include <Adafruit_NeoPixel.h>

// the data structure to store colors
struct Vec3 {
  float c1;
  float c2;
  float c3;
  Vec3()
    : c1(0), c2(0), c3(0) {}
  Vec3(const float c1, const float c2, const float c3)
    : c1(c1), c2(c2), c3(c3) {}
};

// some color definitions
Vec3 blue = Vec3(0, 200, 255);
Vec3 orange = Vec3(255, 105, 0);
Vec3 black = Vec3(0, 0, 0);
Vec3 pink = Vec3(220, 0, 120);

// LED initialization
#define NUMPIXELS 16
Adafruit_NeoPixel ring1(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800); // number of LEDs, arduino pin, LED type
Adafruit_NeoPixel ring2(NUMPIXELS, 9, NEO_GRB + NEO_KHZ800);


void setup() {

  ring1.begin();// start the ring
  ring2.begin();

}

void loop() {
  // time management
  long totalTime = 5000; // total animation time
  long dur = millis() % totalTime; // timing

  // create transitions

  // linear transitions
  transition_lin(&ring1, NUMPIXELS, dur, 0, 2500, black, blue);
  transition_lin(&ring1, NUMPIXELS, dur, 2500, 5000, blue, black);

  radial_animation(&ring2, NUMPIXELS, dur, 0, 5000);

  ring1.show();
  ring2.show();

}

void radial_animation(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    int ledsOffset = map(currentTime, 0, 1000, 0, n * 8); // 8 represents the number of turns during the animation duration
    for (int i = 0 ; i < n ; i++) {
      float teinteHSB = map(i, 0, n, 100, 235); // we want a hue between 100 and 235 for our gradient 
      float teinteLeds = map(teinteHSB, 0, 360, 0, 65535); // we transform to the LED reference frame 
      strip->setPixelColor((i + ledsOffset) % n, strip->gamma32(strip->ColorHSV(teinteLeds, 255, 255))); // we apply the color
    }
  }
}


// no transition: a fixed color for a duration
void fixed_color(Adafruit_NeoPixel *strip, int n, float t, float startT, float endT, Vec3 c) {
  if (t > startT && t < endT) {
    int currentTime = map(t, startT, endT, 0, 1000);
    for (int i = 0; i < n ; i++) {
      strip->setPixelColor(i, strip->Color(c.c1, c.c2, c.c3));
    }
  }
}

// linear transition from one color to another over a given duration
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

// transitions: https://easings.net/
// sine transition
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
// Circular transition
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
// Exponential transition
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

[**^ Home**](#contents)

###  4.3. <a name='Threerings-externalpowersupply'></a>Three rings - external power supply

To add a third light source you will need to use an external power supply to have more current available. Here is the wiring diagram that will allow you to create the necessary electrical circuit.

![](./assets/fritzing_neopixel_3.PNG)

Normally by adapting the code for two rings to 3 rings, you should succeed because you already know many things!

To size a power supply you need to count that one LED consumes a maximum of 60mA when lit in white. Theoretically if we turn on all the LEDs of a ring in white we therefore consume 0.96 A. For two rings 1.92 A and for 3 rings 2.88 A.


[**^ Home**](#contents)


##  5. <a name='TheLedAnimationlibrary'></a>The LedAnimation library

The *LedAnimation* library gathers in a single file, [src/LedAnimation.h](src/LedAnimation.h), the principles detailed in the previous sections: you no longer need to copy/paste long functions into your programs, nor to pass a long list of arguments to each of them.

An animation is written on one line, by chaining functions with dots:

```c
ring1.transition(noir, bleu).from(0).to(2500).ease(SIN_IN_OUT);
```

- *ring1* -> on the LEDs named ring1
- *transition(noir, bleu)* -> the animation to play
- *.from(0).to(2500).ease(SIN_IN_OUT)* -> the settings, in any order you like. The ones you don't write keep their default value.

The shortest possible animation is therefore `ring1.chatoiement();`

[**^ Home**](#contents)

###  5.1. <a name='Installationandexamples'></a>Installation and examples

- install the *Adafruit NeoPixel* library from the library manager (see [1.1](#11-library-installation))
- download this repository as a zip: green *Code* button then *Download ZIP*

- in arduino, menu *Sketch* -> *Include Library* -> *Add .ZIP Library* and choose the downloaded file

![](./assets/Import_zip.PNG)

The examples are then available in the menu *File* -> *Examples* -> *LedAnimation*. They are commented line by line (in French) and numbered from the simplest to the most advanced:

| example | content |
|---|---|
| [01_premiere_animation](examples/01_premiere_animation/01_premiere_animation.ino) | the shortest possible program, explained line by line |
| [02_transitions](examples/02_transitions/02_transitions.ino) | placing transitions and fixed colors in time, repeating with a for loop |
| [03_dynamiques](examples/03_dynamiques/03_dynamiques.ino) | comparing dynamics (easing) on the two halves of the ring |
| [04_selection_de_leds](examples/04_selection_de_leds/04_selection_de_leds.ino) | using only some of the LEDs: ready-made selections, range, list of numbers |
| [05_animation_radiale](examples/05_animation_radiale/05_animation_radiale.ino) | the rotating gradient: hues, number of turns, direction of rotation |
| [06_chatoiement](examples/06_chatoiement/06_chatoiement.ino) | the shimmering: hues, hue change, speed, fading in and out |
| [07_sunrise](examples/07_sunrise/07_sunrise.ino) | a transition LED by LED |
| [08_deux_anneaux](examples/08_deux_anneaux/08_deux_anneaux.ino) | synchronizing two rings |
| [09_animation_perso](examples/09_animation_perso/09_animation_perso.ino) | writing your own animations and making them adjustable |

[**^ Home**](#contents)

###  5.2. <a name='Structureofaprogram'></a>Structure of a program

- `#include <LedAnimation.h>` at the very top
- your colors: `Vec3 bleu = Vec3(0, 200, 255);` (red, green, blue between 0 and 255, see [3.1.1](#311-a-structure-to-store-colors))
- `LedRing ring1(16, 6);` : a ring of 16 LEDs on pin 6. It is an *Adafruit_NeoPixel*: `ring1.setPixelColor()`, `ring1.ColorHSV()` etc. seen in sections 1 and 2 still work.
- `ring1.begin();` in *setup()*
- `timeline(5000);` at the beginning of *loop()*: the total animation time in milliseconds, which loops. This replaces the *totalTime* and *dur* variables.
- your animations, one per line
- `ring1.show();` at the end of *loop()*

For several rings, add a line `LedRing ring2(16, 9);`, a `ring2.begin();` and a `ring2.show();`: all rings share the same timeline.

[**^ Home**](#contents)

###  5.3. <a name='Theanimations'></a>The animations

| animation | effect | specific settings |
|---|---|---|
| `fixed_color(color)` | a fixed color | |
| `transition(color1, color2)` | goes from one color to another, all LEDs at the same time | |
| `sunrise(color1, color2)` | goes from one color to another LED by LED, starting from LED 0 and going around the ring on both sides at once | |
| `animation_radiale()` | a rotating hue gradient | `.hue(100, 235)` `.turns(8)` |
| `chatoiement()` | shimmering: brightness and hue of each LED undulate thanks to noise | `.hue(160, 270)` `.hueTo()` `.speed(1)` |
| `play(function)` | your own animation, see [5.6](#56-writing-your-own-animation) | all |

The values in parentheses in the last column are the default values.

[**^ Home**](#contents)

###  5.4. <a name='Thesettings'></a>The settings

These settings work with every animation:

| setting | effect | default |
|---|---|---|
| `.from(ms)` | start of the animation in milliseconds | 0 |
| `.to(ms)` | end of the animation in milliseconds | the end of the timeline |
| `.ease(dynamic)` | LIN, SIN_IN_OUT, SIN_IN, SIN_OUT, CIRC_IN_OUT, CIRC_IN, CIRC_OUT, EXPO_IN_OUT, EXPO_IN, EXPO_OUT (see https://easings.net/) | LIN |
| `.leds(...)` | only use some of the LEDs, see [5.5](#55-choosing-the-leds) | all |
| `.value(start, end)` | overall brightness at the start and at the end, between 0 and 255, to fade any animation in or out | 255, 255 |
| `.dithering(true)` | smooths very slow changes: each LED changes level at a different moment from its neighbours (see [5.7](#57-pitfalls-to-avoid)) | `false` |

These are only used by some animations (see the previous table) and by your own:

| setting | effect | default |
|---|---|---|
| `.hue(h1, h2)` | the hues used, between 0 and 360 | depends on the animation |
| `.hueTo(h1, h2)` | the hues at the end of the animation, to change color along the way | same as `.hue()` |
| `.turns(number)` | the number of turns during the animation; a negative number rotates the other way | 8 |
| `.speed(speed)` | 2 = twice as fast, 0.5 = twice as slow | 1 |
| `.colors(color1, color2)` | two colors, for your own animations | black, black |

Some examples:

```c
ring1.fixed_color(rose).from(2500).to(2600);                   // a 100 ms flash
ring1.transition(noir, bleu).to(2500).ease(EXPO_IN);           // from 0 to 2.5 seconds
ring1.sunrise(noir, orange).from(1000).to(4000);               // LED by LED
ring1.animation_radiale().hue(0, 60).turns(-2).ease(SIN_OUT);  // during the whole timeline, backwards
ring1.chatoiement().to(3000).value(0, 255);                    // the shimmering fades in
ring1.chatoiement().from(3000).hueTo(0, 60).speed(2);          // then changes hue, twice as fast
```

Between two animations, if there is a "gap" in time, the LEDs keep their last color. If two animations play at the same time on the same LEDs, the last one written in the program wins.

[**^ Home**](#contents)

###  5.5. <a name='ChoosingtheLEDs'></a>Choosing the LEDs

By default an animation uses all the LEDs. `.leds()` lets you use only some of them, in three ways:

```c
ring1.transition(noir, bleu).leds(EVEN);             // a ready-made selection
ring1.transition(noir, bleu).leds(4, 11);            // from LED 4 to LED 11
ring1.transition(noir, bleu).leds({ 0, 4, 5, 9 });   // a list of numbers, between braces

int croix[] = { 0, 4, 8, 12 };                       // a list stored in a variable ...
ring1.transition(noir, bleu).leds(croix);            // ... to reuse it
```

The ready-made selections:

| selection | LEDs used |
|---|---|
| `FULL` | all (default value) |
| `EVEN` | even LEDs: 0, 2, 4 ... |
| `ODDS` | odd LEDs: 1, 3, 5 ... |
| `FIRST_HALF` | the first half of the ring |
| `SECOND_HALF` | the second half of the ring |

LEDs are numbered starting from 0: a ring of 16 LEDs goes from 0 to 15. The LEDs that are not selected are not modified: you can therefore play two different animations at the same time on two parts of the same ring.

```c
ring1.chatoiement().hue(0, 40).leds(FIRST_HALF);
ring1.chatoiement().hue(160, 270).leds(SECOND_HALF);
```

A list of numbers can only contain numbers between 0 and 255.

[**^ Home**](#contents)

###  5.6. <a name='Writingyourownanimation'></a>Writing your own animation

An animation is a simple function that gives a color to the LEDs. You can write your own directly in your program (above *setup()*), without touching the library:

```c
// a rainbow that fades in
void arc_en_ciel(Anim &a) {
  float luminosite = a.mix(0, 255);      // from 0 to 255 during the animation
  for (int i = 0; i < a.n; i++) {        // for each LED
    float teinte = 360. * i / a.n;       // a hue that depends on the LED number
    a.pixel(i, hsv(teinte, 255, luminosite));
  }
}
```

then play it with `play()`. All the settings remain usable:

```c
ring1.play(arc_en_ciel).from(0).to(3000).ease(SIN_IN).leds(FIRST_HALF);
```

An animation is always written in the form `void name(Anim &a) { ... }`. In the function, *a* gives access to:

| | |
|---|---|
| `a.n` | the number of LEDs |
| `a.t` | the progress of the animation, from 0 (start) to 1 (end). The `.ease()` dynamic is already applied |
| `a.ms` | the time elapsed since the start of the animation, in milliseconds |
| `a.mix(x, y)` | a number that goes from x to y during the animation (also works with two *Vec3* colors) |
| `a.pixel(i, color)` | give a color to LED number i |
| `a.fill(color)` | give a color to all the LEDs |

A color is written either with a *Vec3* (red, green, blue), or with `hsv(hue, saturation, brightness)`: hue from 0 to 360, saturation and brightness from 0 to 255.

The settings `.from()` `.to()` `.ease()` `.leds()` and `.value()` are applied automatically: there is nothing to write in the function for them to work. The other settings are used to pass values to your function, to make it adjustable without modifying it:

| setting | in the function |
|---|---|
| `.colors(c1, c2)` | `a.color1` `a.color2` |
| `.hue(h1, h2)` | `a.hue1` `a.hue2` |
| `.hueTo(h1, h2)` | `a.hueTo1` `a.hueTo2` |
| `.turns(k)` | `a.nTurns` |
| `.speed(k)` | `a.speedK` |

For example:

```c
// blinking between the two colors given with .colors()
void clignotement(Anim &a) {
  if ((a.ms / 150) % 2 == 0) {   // alternates every 150 milliseconds
    a.fill(a.color1);
  } else {
    a.fill(a.color2);
  }
}
```

```c
ring1.play(clignotement).from(3000).to(5000).colors(rose, noir);
ring1.play(clignotement).from(5000).to(7000).colors(blanc, noir).leds(EVEN);
```

The example [09_animation_perso](examples/09_animation_perso/09_animation_perso.ino) contains these two animations and a third, more advanced one (a comet). The library's animations are written exactly this way in part 6 of the file [src/LedAnimation.h](src/LedAnimation.h): you can copy them into your program, rename them and modify them.

[**^ Home**](#contents)

###  5.7. <a name='Pitfallstoavoid'></a>Pitfalls to avoid

- **Times are written in milliseconds, directly.** `.to(2.5)` does not mean 2.5 seconds but 2 milliseconds: write `.to(2500)`. And write `timeline(60000)` rather than `timeline(60 * 1000)`: on an arduino uno board this calculation exceeds the largest possible integer and the animation will not play.
- **One line = one animation.** Do not store an animation in a variable (`Anim a = ring1.transition(...)`): it plays at the end of its line, at the semicolon.
- **`(Anim &a)` with the "&"** when you write your own animation. Without it, the program may upload without error on an uno board but the animation will not work.
- **Do not mix with the code from sections 3 and 4.** A program that contains `#include <LedAnimation.h>` must not also contain the *Vec3* structure, the *LIN*, *SIN_IN* ... variables or the old functions `transition(LIN, &ring1, NUMPIXELS, ...)`: compilation would fail.
- **`ring1.show()` at the end of *loop()***: without it nothing lights up.

[**^ Home**](#contents)

###  5.8. <a name='Differenceswiththecodefromsections3and4'></a>Differences with the code from sections 3 and 4

If you are adapting an old program:

| before | now |
|---|---|
| `Adafruit_NeoPixel ring1(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);` | `LedRing ring1(16, 6);` |
| `long totalTime = 5000; long dur = millis() % totalTime;` | `timeline(5000);` |
| `transition(SIN_OUT, &ring1, NUMPIXELS, dur, 0, 2500, noir, bleu);` | `ring1.transition(noir, bleu).from(0).to(2500).ease(SIN_OUT);` |
| `fixed_color(&ring1, NUMPIXELS, dur, 0, 500, rose);` | `ring1.fixed_color(rose).from(0).to(500);` |
| `animation_radiale(LIN, &ring1, NUMPIXELS, dur, 0, 5000, 100, 235);` | `ring1.animation_radiale().from(0).to(5000).hue(100, 235);` |
| `chatoiement(LIN, &ring1, NUMPIXELS, dur, 0, 5000, 160, 270);` | `ring1.chatoiement().from(0).to(5000).hue(160, 270);` |
| `chatoiement_valeur(..., 160, 270, 0, 255);` | `ring1.chatoiement().hue(160, 270).value(0, 255);` |
| `chatoiement_color(..., 160, 255, 0, 60);` | `ring1.chatoiement().hue(160, 255).hueTo(0, 60);` |

The result is not strictly identical:

- the shimmering no longer needs the *SimplexNoise* library: a noise function is included. Its pattern is slightly different, and its speed no longer depends on how fast the board is: it is set with `.speed()`.
- gamma correction (see [1.4](#14-hsb-color-mode)) is applied to every animation, including `fixed_color`: a fixed color and the end of a transition towards that color are therefore identical.
- an LED only has 256 levels per color: in a very slow brightness change (several minutes), each change of level is visible. *Dithering* smooths it and is off by default: add `.dithering(true)` to an animation, or `ring1.dithering(true);` in `setup()` for every animation of a ring. Each LED then rounds its value at a different moment from its neighbours (*dithering*). In a very dark color, two neighbouring LEDs can therefore be one level apart; behind a diffuser this is not visible.
- an animation is active from the `.from()` instant included, until the `.to()` instant excluded: two animations that follow each other chain without a lost frame.

[**^ Home**](#contents)

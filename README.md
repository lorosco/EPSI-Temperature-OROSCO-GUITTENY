# EPSI-Temperature-OROSCO-GUITTENY

Ce projet réalisé par Clément Guitteny et Léonard Orosco se place dans le contexte du cours de Conception Embarqué proposé par Benjamin Freeman à l'EPSI.

Dans le cadre de ce cours, afin de mettre en exercice les compétences acquises en Architecture des Systèmes Embarqués, nous nous sommes lancés sur un projet Arduino Uno initié par PlatformIO.

Ce projet consiste en la programmation de l'affichage de la température transmise par un capteur ASAIR AM2302. En fonction du temps et de notre efficacité nous espérons produire différents affichages:
* Dans un premier temps: affichage de la température sur un affcheur 7 segments.
* Dans un second temps: transmettre les données relevées à une base de données pour pouvoir afficher la température depuis un site et/ou une application mobile
* Dans un dernier temps: proposer en plus de l'affichage en temps réel de la température une courbe d'évolution des températures relevées.

## Définition des variables et constantes

Pour programmer notre système nous avons besoin de 3 pins pour le capteur (2 pour l'alimentention et 1 pour la connection du capteur au système), 4 pins pour l'afficheur 7 segments (1 pin par digit) et 8 pins pour gérer l'allumage des digits (7 pour les segments du digit et 1 pour le point utiliser comme séparation des décimales).
Nous aurons donc besoin de manipuler 13 pins pour mettre en place ce système.

### Le capteur thermique
Nous utiliserons le port d'alimentation de 5V et le prise de masse pour alimenter le capteur. Il sera relié au système en utilisant le port 9 que nous identifierons comme:
```C++
#include "cactus_io_AM2302.h"
int termo = 9;

AM2302 dht(termo);
```
Afin d'utiliser le capteur thermique, on utilise la classe AM2302 de la bibliothèque cactus_io_AM2302.h. Ainsi le thermomètre et ses fonctionnalitées seront accessibles via la variable dht initialisée sur le pin associé à la variable thermo (9).

### L'afficheur 7 segments
Pour identifier les 4 pins correspondant aux différents digits, nous utiliserons les ports analogique de A1 à A4 que nous identifierons comme:
```C++
int D1 = A1; // pour le digit leplus à gauche
int D2 = A2;
int D3 = A3;
int D4 = A4;// pour le digit le plus à dorite
```
Pour identifier les segments d'un digit, on utilisera les ports 2 à 8, pour le point on utilisera le port A0 identifier comme suit:
```C++
int pinA = 2;
int pinB = 3;
int pinC = 4;
int pinD = 5;
int pinE = 6;
int pinF = 7;
int pinG = 8;
int pinH = A0;
```
Nous avons également défini les différents affichages possibles pour les digits dans le tableau
```C++
const int number[11] = {0b1000000, 0b1111001, 0b0100100, 0b0110000, 0b0011001, 0b0010010, 0b0000010, 0b1111000, 0b0000000, 0b0010000, 0b0111111};
```
où chaque valeurs correspond à l'affichage du chiffre de l'indice correspondant pour les indices de 0 à 9. L'élément 10 du tableau correspond au signe '-' pour les températures négatives.

Les nombres stockés dans ce tableau correspondent à la représentation en binaire du digit correspondant à l'indice. Le bit le plus à gauche représente la valeur pour le segment A et le bit le plus à droite, la valeur pour le segment G (c.f. le schéma ci-après), où 1 correspond à 'éteint' et 0 'allumé'.

```
    A
   ---
F |   | B
  | G |
   ---
E |   | C
  |   |
   ---   .H
    D
```
Le "segment" H correspond au point de séparation des décimales qui sera manipuler au moment voulu.
## Initialisation des paramètres du système

A l'exception du thermomètre, tous les pins que nous utilisons seront utilisés en sortie. On va donc initialiser les pins de A à H et les pins D1 à D4 en OUTPUT en utilisant le fonction pinMode.
On utilise la fonction dht.begin() de la classe AM2302 pour initialiser le thermomètre.
## La boucle du système

Pour faire fonctionner notre système, nous aurons besoin d'une fonction permettant d'écrire sur l'afficheur les températures relevées par le capteur. Pour se faire on utilisera 4 fonctions:

```C++
void setDigit(int digit);//précise sur quel digit on écrit
void writeNumber(int i);//écrit un chiffre dans un digit
void writeTemp(float i);//écrit la température sur l'afficheur
```
## La fonction setDigit


```C++
/**
 * @brief allume le digit
 * @param digit le digit que l'on souhaite utiliser
 */

void setDigit(int digit){
  if(digit>=1 && digit<=4){
    switch (digit)
    {
    case 1:
      digitalWrite(D1, HIGH);
      digitalWrite(D2, LOW);
      digitalWrite(D3, LOW);
      digitalWrite(D4, LOW);
      break;

    case 2:
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, LOW);
      digitalWrite(D4, LOW);
      break;

    case 3:
      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, LOW);
      break;

    case 4:
      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      digitalWrite(D3, LOW);
      digitalWrite(D4, HIGH);
      break;
    
    default:
      break;
    }
  }
}
```
## La fonction writeNumber

```C++
/**
 * @brief écrire un chiffre dans le digit activer
 * @param i le chiffre à écrire
 */

void writeNumber(int i){
  if(i>=0 && i<=9){
    int pin;
    int a;
    for (pin = 2, a = 0; a < 7; pin++, a++){
      digitalWrite(pin, bitRead(number[i], a));
    }
  }
}
```
Nous allons expliquer le fonctionnement de la boucle:

Pour chacun des 7 pins qui agissent sur les segments, on affecte la valeur correspondante à son bit dans l'écriture binaire du chiffre dans le tableau number.

pin représente le pin que l'on souhaite activer ou non (activé(1)<=>éteint; désactivé(0)<=>allumé)

a représente la position de la valeur à affecter au bit dans son écriture binaire.
## La fonction wrtiteTemp

Dans cette fonction, pour chaque digit on écrira le chiffre ou symbole correspondant. Pour de raison d'affichage, on laissera un délais de 5ms entre chaque affichage.

Le premier digit sera utilisé comme signe en cas de température négative. On utilise donc une méthode semblable à celle de la fonction writeNumber pour afficher le signe '-' si la valeur relevée par le thermomètre est négative.

Comme le capteur thermique ne relève pas de température supérieure à 80°C, le second digit représentera le chiffre des dizaines de la température relevée (que l'on récupère grace à une simple division entière par 10) que l'on écrit avec la fonction writeNumber.

Même méthode avec le troisième digit sauf qu'on écrit le reste de la division par 10. C'est sur ce digit qu'on active le pin associé à la virgule, on place donc le pinH à LOW.

On récupère la valeur décimale en récupérant le reste de la division par 10 de 10 fois la température relevée.

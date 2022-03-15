#include <Arduino.h>
#include "cactus_io_AM2302.h"

#define termo 13

/*
Schéma des pins utilisées par le set segment
    A
   ---
F |   | B
  | G |
   ---
E |   | C
  |   |
   ---   .H
    D
 */

// Définition variables et constantes

int pinA = 2;
int pinB = 3;
int pinC = 4;
int pinD = 5;
int pinE = 6;
int pinF = 7;
int pinG = 8;
int pinH = A0;
int D1 = 9;
int D2 = 10;
int D3 = 11;
int D4 = 12;
int loops = 500;
AM2302 dht(termo);
const int number[11] = {0b1000000, 0b1111001, 0b0100100, 0b0110000, 0b0011001, 0b0010010, 0b0000010, 0b1111000, 0b0000000, 0b0010000, 0b0111111};

// Allumage du digit correspondant à la valeur à afficher
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

//Ecrire un nombre au format digital
void writeNumber(int i){
  if(i>=0 && i<=9){
    int pin;
    int a;
    for (pin = 2, a = 0; a < 7; pin++, a++){
      digitalWrite(pin, bitRead(number[i], a));
    }
  }
}

//Ecrire la température transmise par le capteur thermique
void writeTemp(float i){
  //signe température
  setDigit(1);
  if(i<0){
    for (int pin =2, a = 0; a < 7;pin++, a++){
      digitalWrite(pin, bitRead(number[10], a));
    }
  } else {
    digitalWrite(D1, LOW);
  }
  delay(5);

  //dizaine température
  setDigit(2);
  int ten = (int) i/10;
  writeNumber(ten);
  delay(5);

  //unité température
  setDigit(3);
  int unit = ((int) i)%10;
  writeNumber(unit);
  digitalWrite(pinH, LOW);
  delay(5);
  
  //valeur décimale de la T
  //( (int)(floor( fabs( num ) * 10 ) ) ) % 10
  setDigit(4);
  int decimal =(int) ( ((float) i)*10)%10;
  digitalWrite(pinH, HIGH);
  writeNumber(decimal);

}

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pins as outputs.
  Serial.begin(9600);
  pinMode(pinA, OUTPUT);     
  pinMode(pinB, OUTPUT);     
  pinMode(pinC, OUTPUT);     
  pinMode(pinD, OUTPUT);     
  pinMode(pinE, OUTPUT);     
  pinMode(pinF, OUTPUT);     
  pinMode(pinG, OUTPUT);
  pinMode(pinH, OUTPUT);   
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT);
  dht.begin();
}

void loop() {
  if(loops <= 0){
    dht.readTemperature();
    loops = 500;
  }
  writeTemp(dht.temperature_C);
  delay(5);
  loops--;
}

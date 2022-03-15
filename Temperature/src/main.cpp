#include <Arduino.h>
#include "cactus_io_AM2302.h"

#define termo 13

/*
    A
   ---
F |   | B
  | G |
   ---
E |   | C
  |   |
   ---
    D
 */

int pinA = 2;
int pinB = 3;
int pinC = 4;
int pinD = 5;
int pinE = 6;
int pinF = 7;
int pinG = 8;
int D1 = 9;
int D2 = 10;
int D3 = 11;
int D4 = 12;
int loops = 500;
AM2302 dht(termo);
const int number[11] = {0b1000000, 0b1111001, 0b0100100, 0b0110000, 0b0011001, 0b0010010, 0b0000010, 0b1111000, 0b0000000, 0b0010000};


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
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT);
  dht.begin();
}

void writeNumber(int i){
  if(i>=0 && i<=9){
    int pin1;
    int a;
    for (pin1 = 2, a = 0; a < 7; pin1++, a++){
      digitalWrite(pin1, bitRead(number[i], a));
    }
  }
}

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

void writeTemp(int i){
  setDigit(2);
  writeNumber(i%10);
  delay(5);
  setDigit(1);
  writeNumber((int) i/10);
}

void loop() {
  if(loops <= 0){
    dht.readTemperature();
    loops = 500;
  }
  writeTemp((int) dht.temperature_C);
  delay(5);
  loops--;
}

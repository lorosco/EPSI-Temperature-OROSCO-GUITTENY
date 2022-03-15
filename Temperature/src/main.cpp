#include <Arduino.h>
#include "cactus_io_AM2302.h"

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

int termo = 9;
int pinA = 2;
int pinB = 3;
int pinC = 4;
int pinD = 5;
int pinE = 6;
int pinF = 7;
int pinG = 8;
int pinH = A0;
int D1 = A1;
int D2 = A2;
int D3 = A3;
int D4 = A4;
int loops = 500;
AM2302 dht(termo);
const int number[11] = {0b1000000, 0b1111001, 0b0100100, 0b0110000, 0b0011001, 0b0010010, 0b0000010, 0b1111000, 0b0000000, 0b0010000, 0b0111111};

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;

int    HTTP_PORT   = 80;
String HTTP_METHOD = "GET";
char   HOST_NAME[] = "temperature.guitteny.net"; // change to your PC's IP address
String PATH_NAME   = "/insert_temp.php";
String queryString = "?username=epsi&password=epsi&temperature=";

void setDigit(int digit);
void writeNumber(int i);
void writeTemp(float i);
void sendDataToDB(float temp);

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

  // initialize the Ethernet shield using DHCP:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to obtaining an IP address using DHCP");
    //while(true);
  }
}

void loop() {
  if(loops <= 0){
    dht.readTemperature();
    loops = 500;
    sendDataToDB(dht.temperature_C);
  }
  writeTemp(dht.temperature_C);
  delay(5);
  loops--;
}

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

void sendDataToDB(float temp){
  // connect to web server on port 80:
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    // if connected:
    Serial.println("Connected to server");
    // make a HTTP request:
    // send HTTP header
    client.println(HTTP_METHOD + " " + PATH_NAME + queryString + temp + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header

    while(client.connected()) {
      if(client.available()){
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }

    // the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  } else {// if not connected:
    Serial.println("connection failed");
  }
}
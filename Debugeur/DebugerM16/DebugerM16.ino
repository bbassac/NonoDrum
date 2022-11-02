#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>


int pinToTest = 35;

void setup() {
  //Serial.begin (31250); // attention vitesse de transmission MIDI = 31250, Moniteur Série = 9600
  Serial.begin (9600);
 
  //Navigation
  pinMode (pinToTest, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

void loop() {

    int value = digitalRead (pinToTest); // lecture des boutons
   
    if (value == HIGH) {

    digitalWrite(13, LOW);

  } else {

    digitalWrite(13, HIGH);

  }
}





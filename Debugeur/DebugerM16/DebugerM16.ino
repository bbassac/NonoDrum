#include <Arduino.h>
#include <SPI.h>


void setup() {
  Serial.begin (9600);

}

void loop() {

  

  testPins();
}


void testPins(){
    int value = analogRead (A7); // lecture des boutons
   Serial.print("A7=");
   Serial.println(value);
   int value2 = analogRead (A15); // lecture des boutons
   Serial.print("A15=");
   Serial.println(value2);
  delay(500);
}





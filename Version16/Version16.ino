#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// Nombre d'entrées analogiques:
int const nb_fad = 8;
int const nb_pot = 8;
// nombre de boutons
int const Ent_Numerique = 16;
//sensibilité: 2, ça a l'air bien
int sensibilite = 30;


// déclaration des variables pour les boutons
// tableau 1             |brocheBouton|note en HEX|
int bouton[16][2] = { 52, 40,
                      50, 41,
                      48, 42,
                      46, 43,
                      44, 44,
                      42, 45,
                      40, 46,
                      38, 47,
                      53, 48,
                      51, 49,
                      49, 50,
                      47, 51,
                      45, 52,
                      43, 53,
                      41, 54,
                      39, 55 };


// tableau 2              |etat du bouton actuel | etat du bouton antérieur |
boolean prev_button_value[16][2] = { 0, 0,
                                     0, 0,
                                     0, 0,
                                     0, 0,
                                     0, 0,
                                     0, 0,
                                     0, 0,
                                     0, 0,
                                     0, 0,
                                     0, 0,
                                     0, 0,
                                     0, 0,
                                     0, 0,
                                     0, 0,
                                     0, 0,
                                     0, 0 };

// déclaration des variables pour les potentiomètres

// définition broches potentiomètres faders (de droite a gauche)
int const fad[8] = { A0, A1, A2, A3, A4, A5, A6, A7 };
int const pot[8] = { A8, A9, A10, A11, A12, A13, A14, A15};
// attribution control change Midi
int Cc_pot[8] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
int Cc_fad[8] = { 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };

// valeur initiale
int prev_pot_value[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int prev_fad_value[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
byte midiChannel = 0;  // canal 1
int LEFT = 37;
int UP = 35;
int RIGHT = 33;
int DOWN = 31;
int prev_nav_left = HIGH;
int prev_nav_up = HIGH;
int prev_nav_right = HIGH;
int prev_nav_down = HIGH;

void setup() {
  Serial.begin (31250); // attention vitesse de transmission MIDI = 31250, Moniteur Série = 9600
  //Serial.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.clearDisplay();
  displayLaunchScreen();

  // initialisation des broches
  for (int i = 0; i < Ent_Numerique; i++) {
    pinMode(bouton[i][0], INPUT_PULLUP);
  }
  //Navigation
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
}

void loop() {
  
  manageDigitalButtons();
  manageNavigation();
  
  // Les faders
   for (int j = 0; j < nb_fad ; j++) {
    prev_fad_value [j] = rafraichir (fad[j], prev_fad_value[j], Cc_fad[j],j);
  }

  // Les potentiometres
   for (int j = 0; j < nb_pot ; j++) {
    prev_pot_value [j] = rafraichir (pot[j], prev_pot_value[j], Cc_pot[j],j);
  }

}

void manageNavigation(){
  
  //Navigation LEFT
  if (prev_nav_left==HIGH && digitalRead(LEFT)== LOW) {
    //Serial.println("LEFT appuyé");
    prev_nav_left = LOW;
  } else if (prev_nav_left==LOW && digitalRead(LEFT) == HIGH){
    //Serial.println("LEFT relaché");
    prev_nav_left = HIGH;
  } 
  //Navigation UP
  if (prev_nav_up==HIGH && digitalRead(UP)== LOW) {
    //Serial.println("UP appuyé");
    midiChannel = (midiChannel +1 ) %16 ;
    prev_nav_up = LOW;
  } else if (prev_nav_up==LOW && digitalRead(UP) == HIGH){
    //Serial.println("UP relaché");
    prev_nav_up = HIGH;
  } 
  //Navigation RIGHT
  else if (prev_nav_right==HIGH && digitalRead(RIGHT)== LOW) {
    //Serial.println("RIGHT appuyé");
    prev_nav_right = LOW;
  }else if (prev_nav_right==LOW && digitalRead(RIGHT) == HIGH){
    //Serial.println("Right relaché");
    prev_nav_right = HIGH;
  }  
  //Navigation DOWN
  else if (prev_nav_down==HIGH && digitalRead(DOWN)== LOW) {
    //Serial.println("DOWN appuyé");
    midiChannel = (midiChannel -1 ) %16 ;
    prev_nav_down = LOW;
  }else if (prev_nav_down==LOW && digitalRead(DOWN) == HIGH){
    //Serial.println("DOWN relaché");
    prev_nav_down = HIGH;
  } 
}

void manageDigitalButtons(){
  // Les boutons
  for (int i = 0; i < Ent_Numerique; i++) {
    prev_button_value[i][0] = digitalRead(bouton[i][0]);  // lecture des boutons
    // detection des changements d'état des boutons
    if (!prev_button_value[i][0] && prev_button_value[i][1]) {  //
      sendMessage(0x90, bouton[i][1], 127);                     // envoie noteON
    }
    if (prev_button_value[i][0] && !prev_button_value[i][1]) {
      sendMessage(0x80, bouton[i][1], 127);  // envoie noteOFF
    }

    prev_button_value[i][1] = prev_button_value[i][0];
  }
}

int rafraichir(int const pot, int potVal, int Cc,int j) {
  int tmp = analogRead(pot);
  /*Serial.print("A");
  Serial.print(j,DEC);
  Serial.print("=");
  Serial.println(tmp);
  delay(300); */ 
  if (potVal + sensibilite < tmp || potVal - sensibilite > tmp) {
    potVal = tmp;
    sendMessage(0xB0, Cc, potVal >> 3);  //envoie valeur non-clippée
  }

  return potVal;  // version 1: else return analogRead(pot); repare le 5/5/2020
}



// Fonctions communes
void sendMessage(byte cmd, byte cc, byte val) {
  cmd = cmd | byte(midiChannel);
  Serial.write(cmd);
  Serial.write(cc);
  Serial.write(val);

  /*Serial.println("Debug ");
  Serial.print("Canal : ");
  Serial.print (midiChannel +1);
  Serial.println(" ");
  Serial.print("cmd : ");
  Serial.print ((int)cmd, DEC);
  Serial.println(" ");
  Serial.print("cc : ");
  Serial.print ((int)cc, DEC);
  Serial.println(" ");
  Serial.print("val : ");
  Serial.print ((int)val, DEC);
  Serial.println ("");
 */
  display.clearDisplay();
  display.setTextSize(2);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0, 0);
  display.print("Canal : ");
  display.println(midiChannel + 1);

  display.print("cmd : ");
  display.println((int)cmd, DEC);

  display.print("cc : ");
  display.println((int)cc, DEC);

  display.print("val : ");
  display.println((int)val, DEC);

  display.display();
}

void displayLaunchScreen(void) {
  display.clearDisplay();
  display.setTextSize(2);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0, 0);              // Start at top-left corner
  display.println("Boot");
  display.println("Loading !");
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  // Draw 'inverse' text
  display.display();
  delay(1000);
}
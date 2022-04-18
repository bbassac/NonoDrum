// _____ A définir ____
// Nombre d'entrées analogiques:
int Ent_Analog = 16;
// nombre de boutons
int const Ent_Numerique = 16;
//sensibilité: 2, ça a l'air bien
int sensibilite = 2;

// _________ A ne pas modifier _____________

// déclaration des variables



// déclaration des variables pour les boutons
// tableau 1             |brocheBouton|note en HEX|
int bouton [16][2] = {      38,           20,
                            39,           21,
                            40,           22,
                            41,           23,
                            42,           24,
                            43,           25,
                            44,           26,
                            45,           27,
                            46,           28,
                            47,           29,
                            48,           30,
                            49,           31,
                            50,           32,
                            51,           33,
                            52,           34,
                            53,           35
                     };


// tableau 2              |etat du bouton actuel | etat du bouton antérieur |
boolean EtatBouton[16][2] = {         0,                        0,
                                      0,                        0,
                                      0,                        0,
                                      0,                        0,
                                      0,                        0,
                                      0,                        0,
                                      0,                        0,
                                      0,                        0,
                                      0,                        0,
                                      0,                        0,
                                      0,                        0,
                                      0,                        0,
                                      0,                        0,
                                      0,                        0,
                                      0,                        0,
                                      0,                        0
                            };
int i;



// déclaration des variables pour les potentiomètres

// définition broches potentiomètres faders (de droite a gauche)
int const pot[16] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};

// attribution control change Midi
int Cc[16] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};

// valeur initiale
int potVal [16];

int j;


#define midiChannel (byte) 0 // canal 1


void setup() {
  Serial.begin (31250); // attention vitesse de transmission MIDI = 31250, Moniteur Série = 9600

  // initialisation des broches
  for (i = 0; i < Ent_Numerique; i++) {
    pinMode (bouton [i][0], INPUT_PULLUP);
  }

  // initialisation des valeur du potentiomètre
  for (int i = 0; i < 16 ; i++) {
    potVal[i] = 0;
  }
}

void loop() {
  // Les boutons
  for (i = 0; i < Ent_Numerique; i++) {
    EtatBouton [i][0] = digitalRead (bouton [i][0]); // lecture des boutons

    // detection des changements d'état des boutons
    if (!EtatBouton [i][0] &&  EtatBouton [i][1]) { //
      sendMessage (0x90, bouton [i][1] , 127); // envoie noteON
    }
    if (EtatBouton [i][0] && !EtatBouton [i][1]) {
      sendMessage (0x80, bouton [i][1], 127); // envoie noteOFF
    }

    EtatBouton [i][1] = EtatBouton [i][0];
  }

  // Les potentiomètres
  for (int j = 0; j < Ent_Analog ; j++) {
    potVal [j] = rafraichir (pot[j], potVal[j], Cc[j], j);
  }
}


int rafraichir (int const pot, int potVal, int Cc, int i) {
  if (potVal + sensibilite < analogRead (pot) || potVal - sensibilite > analogRead (pot)) {
    potVal = analogRead (pot);
    int Val = potVal / 8;

    sendMessage(0xB0, Cc, Val);   //envoie valeur non-clippée
  }

  return potVal; // version 1: else return analogRead(pot); repare le 5/5/2020
}



// Fonctions communes
void sendMessage (byte cmd, byte ch, byte val) {
  cmd = cmd | byte (midiChannel);
  Serial.write (cmd);
  Serial.write (ch);
  Serial.write (val);
}

/*
  // version débuggage, penser à changer la vitesse (9600 ou 31250)
  void sendMessage (int cmd, int cc, int val){
  cmd= cmd|byte (midiChannel);
  Serial.print("cmd : ");
  Serial.print (cmd, HEX);
  Serial.print(" ");
  Serial.print("cc : ");
  Serial.print (cc, HEX);
  Serial.print(" ");
  Serial.print("val : ");
  Serial.print (val, DEC);
  Serial.println ("");
  }
*/

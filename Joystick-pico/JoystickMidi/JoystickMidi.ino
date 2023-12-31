#include <MIDI.h>

const int joystickXPin = A1;  // Connect the X-axis of the joystick to analog pin A0
const int joystickYPin = A0;  // Connect the Y-axis of the joystick to analog pin A1
const int PINJOYSTICK_BUTTON = 22;
const int PIN_GREEN_LED = 21;
int prev_xValue = 0;
int prev_yMidivalue = 64;
int seuil = 3;
int seuilPitch = 15;
const int pitchBendRange = 8192;  // MIDI pitch bend range is 0 to 16383, so half is 8192

int prevButtonValue = HIGH;
MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(9600);
  pinMode(PINJOYSTICK_BUTTON, INPUT_PULLUP);
  pinMode(PIN_GREEN_LED, OUTPUT);
}

void loop() {
  int xValue = analogRead(joystickXPin);
  int yMidiValue = map(analogRead(joystickYPin) / 8, 0, 127, 127, 0);  // Convert 0->1024 to 0-> 127 ; and invert 127->0
  int buttonValue = digitalRead(PINJOYSTICK_BUTTON);

  if (abs(xValue - prev_xValue) > seuilPitch) {
    int pitchBendValue = map(xValue, 0, 1023, -pitchBendRange, pitchBendRange);  //remap  pitchbend
    prev_xValue = xValue;

    // Send MIDI messages
    Serial.print("X :");
    Serial.print(xValue);
    Serial.print("bend :");
    Serial.println(pitchBendValue);
  }

  if (abs(yMidiValue - prev_yMidivalue) > seuil) {
    prev_yMidivalue = yMidiValue;
    Serial.print(" Y :");
    Serial.println(yMidiValue);  // Most significant bits of pitch bend
  }

  if (buttonValue != prevButtonValue) {
    prevButtonValue = buttonValue;
    if (buttonValue == LOW){
      Serial.println("Led ON");
      digitalWrite(PIN_GREEN_LED, HIGH);
    }else {
      Serial.println("Led Off");
      digitalWrite(PIN_GREEN_LED, LOW);
    }
    Serial.print("Button is ");
    Serial.println(buttonValue);
  }
}
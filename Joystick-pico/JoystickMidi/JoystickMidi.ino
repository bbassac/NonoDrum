#include <MIDI.h>

const int PIN_JOY_X = A1;  // Connect the X-axis of the joystick to analog pin A0
const int PIN_JOY_Y = A0;  // Connect the Y-axis of the joystick to analog pin A1
const int PIN_JOY_BUTTON = 22;
const int PIN_GREEN_LED = 21;
const int PIN_POWER_LED = 18;

int prev_xValue = 0;
int prev_yMidivalue = 64;
int prevButtonValue = HIGH;

const int THRESHOLD = 3;
const int THRESHOLD_PITCH = 15;
const int PITCH_BEND_RANGE = 8192;  // MIDI pitch bend range is 0 to 16383, so half is 8192


MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(9600);
  pinMode(PIN_JOY_BUTTON, INPUT_PULLUP);
  pinMode(PIN_GREEN_LED, OUTPUT);
  pinMode(PIN_POWER_LED, OUTPUT);
  digitalWrite(PIN_POWER_LED, HIGH);
}

void loop() {
  int xValue = analogRead(PIN_JOY_X);
  int yMidiValue = map(analogRead(PIN_JOY_Y) / 8, 0, 127, 127, 0);  // Convert 0->1024 to 0-> 127 ; and invert 127->0
  int buttonValue = digitalRead(PIN_JOY_BUTTON);

  if (abs(xValue - prev_xValue) > THRESHOLD_PITCH) {
    int pitchBendValue = map(xValue, 0, 1023, -PITCH_BEND_RANGE, PITCH_BEND_RANGE);  //remap  pitchbend
    prev_xValue = xValue;

    // Send MIDI messages
    Serial.print("X :");
    Serial.print(xValue);
    Serial.print("bend :");
    Serial.println(pitchBendValue);
  }

  if (abs(yMidiValue - prev_yMidivalue) > THRESHOLD) {
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
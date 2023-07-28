//Constants
#define BRIGHTNESS 200

//Parameters
const int ledPin = 25;

//Variables
bool ledStatus = 0;

void setup() {
  //Init Serial USB
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
  //Init led
  pinMode(ledPin, OUTPUT);

}

void loop() {
  testLed();
}

void testLed( ) {
  digitalWrite(ledPin, HIGH);
  Serial.println(F("LED ON"));
  delay(500);
  digitalWrite(ledPin, LOW);
  Serial.println(F("LED OFF"));
  delay(500);
}

const int ButtonPin = D6;
int buttonState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ButtonPin, INPUT_PULLUP);
}

void loop() {
  buttonState = digitalRead(ButtonPin);
  Serial.println(buttonState);
  delay(100);
}

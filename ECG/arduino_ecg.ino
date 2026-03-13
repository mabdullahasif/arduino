// Arduino ECG Reader

const int ecgPin = A0;
const int samplingDelay = 5; // about 200Hz sampling

void setup() { Serial.begin(115200); }

void loop() {
  int ecgValue = analogRead(ecgPin);

  Serial.println(ecgValue);

  delay(samplingDelay);
}
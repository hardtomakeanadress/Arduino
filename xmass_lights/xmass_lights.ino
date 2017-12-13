//used with attiny85 digispark

int led = 1;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  blynk();
}

int blynk() {
  int randomTime  = random(50,1000);
  digitalWrite(led, HIGH);
  delay(randomTime);
  digitalWrite(led, LOW);
  delay(randomTime);  
}

int sensorValue;
bool isTriggered = false;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() {
  isTriggered();
}

void isTriggered() {
  
  sensorValue = analogRead(A0);
  
  if (sensorValue <= 30) {
    isTriggered = true;
    }
  else {
    isTriggered = false;
    }
  
  
}

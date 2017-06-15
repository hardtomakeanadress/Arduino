
int lightPin = 5;
int ledPin   =10;


void setup() {

  Serial.begin(9600);

  pinMode(ledPin, OUTPUT );
}


void loop() {
  
  int citire = (analogRead(lightPin));
  
  Serial.println(citire);
  checkGreenLed(citire);
  delay(100);
 
}

void checkGreenLed(int valoare) {
  if (valoare < 900 ) {
    digitalWrite(ledPin, HIGH);
  }
  else {
    digitalWrite(ledPin, LOW);
  }
}


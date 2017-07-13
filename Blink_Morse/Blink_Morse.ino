/*
  Blink Morse SOS Signal
  by Stefan Adrian
 */


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever; · · · – – – · · ·) pauza 500; punct 500; lung 1000
void loop() {
  digitalWrite(13, HIGH);   
  delay(100);              // FACE PUNCT
  
  digitalWrite(13, LOW);    
  delay(100);             // FACE SPATIU
  
  digitalWrite(13, HIGH);
  delay(100);             // FACE PUNCT
  
  digitalWrite(13,LOW);
  delay(100);             // FACE SPATIU
  
  digitalWrite(13, HIGH);
  delay(100);            // FACE PUNCT
  
  digitalWrite(13, LOW);
  delay(500);           // FACE SPATIU
  
  digitalWrite(13, HIGH);
  delay(500);         // FACE LINIE
  
  digitalWrite(13, LOW);
  delay(100);          // FACE SPATIU
  
  digitalWrite(13, HIGH);
  delay(500);        // FACE LINIE
  
  digitalWrite(13, LOW);
  delay(100);          // FACE SPATIU
  
  digitalWrite(13, HIGH);
  delay(500);        // FACE LINIE
  
  digitalWrite(13, LOW);
  delay(500);          // FACE SPATIU
  
  digitalWrite(13, HIGH);   
  delay(100);              // FACE PUNCT
  
  digitalWrite(13, LOW);    
  delay(100);             // FACE SPATIU
  
  digitalWrite(13, HIGH);
  delay(100);             // FACE PUNCT
  
  digitalWrite(13,LOW);
  delay(100);             // FACE SPATIU
  
  digitalWrite(13, HIGH);
  delay(100);            // FACE PUNCT
  
  digitalWrite(13, LOW);
  delay(2000);           // FACE SPATIU
}

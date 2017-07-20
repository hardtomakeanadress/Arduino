#define decreasePulseButton 2
#define increasePulseButton 3
#define fireButton          8
#define firePin             4
#define ledOne              5
#define ledTwo              6
#define ledThree            7


int pulse = 1;
int pulseDuration;// 50-100-150 ms, depending on pulse variable
int buttonStateDecrease, buttonStateIncrease, fireButtonState ;

void setup() {
  Serial.begin(9600);
  pinMode(decreasePulseButton, INPUT);
  pinMode(increasePulseButton, INPUT);
  pinMode(fireButton, INPUT);
  
  pinMode(firePin, OUTPUT);
  pinMode(ledOne, OUTPUT);
  pinMode(ledTwo, OUTPUT);
  pinMode(ledThree, OUTPUT);

  digitalWrite(firePin, LOW);
  digitalWrite(ledOne, LOW);
  digitalWrite(ledTwo, LOW);
  digitalWrite(ledThree, LOW);
  
}

void loop() {
  checkIfAnyButtonsArePressed();
  displayPulseLength();
  takeAction();
}


void checkIfAnyButtonsArePressed(){
  buttonStateDecrease = digitalRead(decreasePulseButton);
  delay(2);
  buttonStateIncrease = digitalRead(increasePulseButton);
  delay(2);
  fireButtonState     = digitalRead(fireButton);
  delay(2);
}

void displayPulseLength() {
  switch (pulse) {
    case 1: {
      digitalWrite(ledOne, HIGH);
      break;
    }
    case 2: {
      digitalWrite(ledOne, HIGH);
      digitalWrite(ledTwo, HIGH);
      break;
    }
    case 3: {
      digitalWrite(ledOne, HIGH);
      digitalWrite(ledTwo, HIGH);
      digitalWrite(ledThree, HIGH);
      break;
    }
  }
}

void takeAction() {
  
  //Minus Button
  if ((buttonStateDecrease == HIGH) && 
      (pulse > 1) &&
      (!buttonStateIncrease == HIGH)) {
    pulse -= 1;
    Serial.print("Button Down was pressed");
  }
  
  //Plus Button
  if ((buttonStateIncrease == HIGH) && 
      (pulse < 3) &&
      (!buttonStateDecrease == HIGH)) {
    pulse += 1;      
    Serial.print("Button Up was pressed");
  }
  
  //Fire Button
//  if (fireButtonState == HIGH) {
//    pulseDuration = 50 * pulse;
//    Serial.print("Fire Button was pressed");
//    Serial.println(pulseDuration);
//    //INITIAL SHORT pulse
//    digitalWrite(firePin, HIGH); 
//    delay(50);
//    digitalWrite(firePin, LOW); 
//    //MAIN WELD pulse
//    digitalWrite(firePin, HIGH); 
//    delay(pulseDuration);
//    digitalWrite(firePin, LOW);   
//  }
}


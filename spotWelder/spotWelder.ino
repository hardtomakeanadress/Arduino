#define decreasePulseButton 2
#define increasePulseButton 3
#define firePin             4
#define ledOne              5
#define ledTwo              6
#define ledThree            7

int pulse = 1; // 5-10-15 ms
int buttonStateDecrease, buttonStateIncrease;

void setup() {
  pinMode(decreasePulseButton, INPUT);
  pinMode(increasePulseButton, INPUT);
  pinMode(firePin, OUTPUT);
  pinMode(ledOne, OUTPUT);
  pinMode(ledTwo, OUTPUT);
  pinMode(ledThree, OUTPUT);
}

void loop() {
  checkIfAnyButtonsArePressed();
  displayPulseLength();
}


void checkIfAnyButtonsArePressed(){
  buttonStateDecrease = digitalRead(decreasePulseButton);
  buttonStateIncrease = digitalRead(increasePulseButton);

  if ((buttonStateDecrease == HIGH) && (pulse > 1)) {
    pulse -= 1;      
  }

  if ((buttonStateIncrease == HIGH) && (pulse < 3)) {
    pulse += 1;      
  }
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


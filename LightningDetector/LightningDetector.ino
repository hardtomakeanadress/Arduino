// Maurice Ribble 
// 6-1-2008
// http://www.glacialwanderer.com/hobbyrobotics

// This code uses my camera trigger and lightning detector.
// It waits for a sudden change in the light intensity
// and then triggers the camera.

#define SHUTTER_PIN 13
#define LIGHTNING_TRIGGER_ANALOG_PIN 0
#define TRIGGER_THRESHHOLD 50

const int pizeo = 8;
int lightningVal;

void setup()
{
  pinMode(SHUTTER_PIN, OUTPUT);
  pinMode(pizeo, OUTPUT);
  
  digitalWrite(SHUTTER_PIN, LOW);
  Serial.begin(9600); // open serial

  lightningVal = analogRead(LIGHTNING_TRIGGER_ANALOG_PIN);
}

void loop()
{
  int newLightningVal = analogRead(LIGHTNING_TRIGGER_ANALOG_PIN);
    
  if ((lightningVal - newLightningVal) > TRIGGER_THRESHHOLD)
  {
    digitalWrite(SHUTTER_PIN, HIGH);
    delay(100);
    digitalWrite(SHUTTER_PIN, LOW);

    makeTone();
  }
}

void makeTone() {

 tone(pizeo,1000,1000);  
}

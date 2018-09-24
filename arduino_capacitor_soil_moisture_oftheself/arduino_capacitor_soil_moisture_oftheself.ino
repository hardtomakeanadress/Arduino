#include "LowPower.h"
#include <VirtualWire.h>

int sensorPin           = A0;
int attachmentsPowerPin = A1;
 
void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(attachmentsPowerPin, OUTPUT);

  vw_setup(2000);   // Bits per sec
}

void loop(){
  int capAverage = 0;
  
  digitalWrite(attachmentsPowerPin, HIGH);
  delay(100);
  
  for (int i=0;i<10;i++) {
    capAverage += analogRead(sensorPin);
  }    
  
  capAverage = capAverage / 10;
  capAverage = map(capAverage, 595, 330, 1, 100);
  capAverage = char(capAverage);
  vw_send((uint8_t *)capAverage, strlen(capAverage));
  vw_wait_tx(); // Wait until the whole mes
  
  Serial.println(capAverage);
  delay(100);
  
  digitalWrite(attachmentsPowerPin, LOW);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);    
}

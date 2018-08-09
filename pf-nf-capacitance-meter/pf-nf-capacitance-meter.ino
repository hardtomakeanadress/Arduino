const int OUT_PIN = A2;
const int IN_PIN = A0;
const float IN_STRAY_CAP_TO_GND = 24.48;
const float IN_CAP_TO_GND  = IN_STRAY_CAP_TO_GND;
const int MAX_ADC_VALUE = 1023;

void setup()
{
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  float capAverage = 0.0;
  
  for (int i=0;i<100;i++) {
    capAverage = capAverage + readValue();
  }    
  
  capAverage = capAverage / 100;
  capAverage = map(capAverage, 7.2, 20, 1, 100);

  
      
  Serial.println(capAverage);
      

}


float readValue() {
  pinMode(IN_PIN, INPUT);
  digitalWrite(OUT_PIN, HIGH);
  int val = analogRead(IN_PIN);
  digitalWrite(OUT_PIN, LOW);
  pinMode(IN_PIN, OUTPUT);
  float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);
  return capacitance;
}

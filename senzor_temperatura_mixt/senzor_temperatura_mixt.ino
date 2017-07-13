//https://learn.adafruit.com/thermistor/using-a-thermistor
// which analog pin to connect
#define THERMISTORPIN A0         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 11200      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 22 //se poate "regla " de aici termistorul   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 100000    
 
float tempp;
int samples[NUMSAMPLES];
int tempPin = 1;
 
void setup(void) {
  Serial.begin(9600);
  
}
 
void loop(void) {
  uint8_t i;
  float average;
 
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
 
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  
 
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 274;                         // convert to C, am modificat eu pentru calibrare.
 
  Serial.print("Temperatura termistor 10K : "); 
  Serial.print(steinhart);
  Serial.print((char)176);
  Serial.print("C");
  Serial.println();
  tempp = analogRead(tempPin);
  tempp = tempp * 0.4887585532746823;
  Serial.print("Temperatura LM35DZ : ");
  Serial.print(tempp);
  Serial.print((char)176);
  Serial.print("C");
  Serial.println();
  delay(2000);
}

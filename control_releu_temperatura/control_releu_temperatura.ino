// temperatura in grade celsius

// pinul 13 va fi releul, se opreste incalzirea cand temperatura trece de 28 grade Celsius

float temp;
int tempPin = 1;
int led = 13;

void setup() 
{
 //Initialize serial 
  Serial.begin(9600);
 
} 
void loop()
{
  temp = analogRead(tempPin);
  temp = temp * 0.4887585532746823;
  Serial.print("TEMPERATURA ESTE: ");
  Serial.print(temp);
  Serial.print((char)176);
  Serial.print("C");
  Serial.println();
  	if (temp >28) {
digitalWrite(13,LOW);
} else {
digitalWrite(13,HIGH);
}
  delay(1000);
}


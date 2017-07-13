// temperatura in grade celsius
// Serial.print(176, BYTE); - simbolul pt "grad celsius"

float temp;
int tempPin = 0;

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
  delay(5000);
}

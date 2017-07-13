#include <notele_de_stoc.h>

// temperatura in grade celsius

// pinul 13 va fi releul, se opreste incalzirea cand temperatura trece de 28 grade Celsius

float tempvechi;
float tempnou;
int lm35vechi = A5;
int lm35nou = A1;
int led = 13;
int alarma = 8;
int note1 = NOTE_A6;  

void setup() 
{
 //Porneste portul serial 
  Serial.begin(9600);
 pinMode(13, OUTPUT);
 digitalWrite(13, LOW);
} 
void loop()
{
  
  tempnou = analogRead(lm35nou);
  tempnou = tempnou * 0.4887585532746823;
  Serial.print("TEMPERATURA lm35 NOU: ");
  Serial.print(tempnou);
  Serial.print((char)176);
  Serial.print("C");
  Serial.println();
  
  
  
  tempvechi = analogRead(lm35vechi);
  tempvechi = tempvechi * 0.4887585532746823;
  Serial.print("TEMPERATURA lm35vechi: ");
  Serial.print(tempvechi);
  Serial.print((char)176);
  Serial.print("C");
  Serial.println();
  	if (tempvechi >25) {
digitalWrite(led,LOW);
} 
else 
{
digitalWrite(led,HIGH);
}
// Conditia de Sunet de ALARMA
if (tempvechi >27) {
// sound tone
tone(8, note1);
}
else {
//opreste alarma
noTone(8);
}
// pana aici

// sa scrie ALARMA

if (tempvechi >28) {

Serial.println("Alarma, Temperatura CRITICA !");
}
else {
Serial.println("Status OK");

}
//pana aici

  delay(1000);
}


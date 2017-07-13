#include <notele_de_stoc.h>

// pinul 2 va fi releul, se opreste incalzirea cand temperatura trece de 38 grade Celsius


float temp;
int senzor = 3;
int led = 2;
int alarma = 1;
int note1 = NOTE_A6;  

void setup() 
{
 pinMode(3, INPUT);
 pinMode(2, OUTPUT);
 pinMode(1, OUTPUT);
 digitalWrite(2, LOW);
} 
void loop()
{
  
  temp = analogRead(senzor);
  temp = temp * 0.4887585532746823;
  
  if (temp >38) 
  {
digitalWrite(led,LOW);
} 
else 
{
digitalWrite(led,HIGH);
}
// Conditia de Sunet de ALARMA
if (temp >39) 
{
// sound tone
tone(1, note1);
}
else {
//opreste alarma
noTone(1);
}
//kkkkkk
if (temp <35) 
{
// sound tone
tone(1, note1);
}
else {
//opreste alarma
noTone(1);
}
  delay(5000);
}


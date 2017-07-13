/* I had no succes using the "native" arduino RX and TX pins (digital 0 and digital 1) to communicate with my HC-05 bluetooth module, 
 *  so I used the informations from here http://www.instructables.com/id/Arduino-AND-Bluetooth-HC-05-Connecting-easily/?ALLSTEPS 
to go arround this issue.*/

#include <SoftwareSerial.h>

SoftwareSerial BluetoothSerial(10, 11);
double t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,temp;
int tempPin = 0;

void setup() 
{
 //Initialize the pin 10 & 11 -meaning the bluetooth serial 
 
  BluetoothSerial.begin(9600);
  
  
} 
void loop()
{
  
  // take ten readings with 10ms delay in between
  t1 = analogRead(tempPin);
  delay(10);
  t2 = analogRead(tempPin);
  delay(10);
  t3 = analogRead(tempPin);
  delay(10);
  t4 = analogRead(tempPin);
  delay(10);
  t5 = analogRead(tempPin);
  delay(10);
  t6 = analogRead(tempPin);
  delay(10);
  t7 = analogRead(tempPin);
  delay(10);
  t8 = analogRead(tempPin);
  delay(10);
  t9 = analogRead(tempPin);
  delay(10);
  t10 = analogRead(tempPin);
  delay(10);

  //make an average temp
  temp = ((t1+t2+t3+t4+t5+t6+t7+t8+t9+t10) / 10) * 0.488;
  
  
  
  

  // serial print over bluetooth connection
  
  BluetoothSerial.print(temp);
  BluetoothSerial.println();
  
  delay(60000);
}

/* I had no succes using the "native" arduino RX and TX pins (digital 0 and digital 1) to communicate with my HC-05 bluetooth module,
 *  so I used the informations from here http://www.instructables.com/id/Arduino-AND-Bluetooth-HC-05-Connecting-easily/?ALLSTEPS 
 *  to go arround this issue. Adi
*/

#include <SoftwareSerial.h>
#include <DHT.h>
#include  <JeeLib.h>// Low power functions library
ISR(WDT_vect) { Sleepy::watchdogEvent(); } // Setup the watchdog
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);


SoftwareSerial BluetoothSerial(10, 11);


void setup() 
{
 //Initialize the pin 10 & 11 -meaning the bluetooth serial 
 
  BluetoothSerial.begin(9600);
  
  dht.begin();
} 


void loop()
{
  
  Sleepy::loseSomeTime(60000); //1 min sleep
  
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  


  
  // serial print over bluetooth connection
  BluetoothSerial.print(h);
  BluetoothSerial.print(",");
  BluetoothSerial.print(t);
  BluetoothSerial.print("\n");
  
  
  
  
  
  
  
}

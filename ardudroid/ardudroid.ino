/*================================= EG LABS =======================================
The demonstration of simple read write using the Arduino on SD card
 
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 * LED attached from pin 6 to ground through a 1K resistor
================================== EG LABS =======================================*/
 
#include <SD.h>
 
File myFile;                                                                  // variable required to hold the file descriptor
const int chipSelect = 4;                                                     // the pin number which is needs to be used as a SS pin
 
void setup()
{
   Serial.begin(9600);                                                        // initialize the serial port at baud rate 9600
   Serial.print("Initializing SD card...");                                  
   pinMode(10, OUTPUT);                                                       // It is a must to set the hardware SS pin as output eventhough it is not using
  
   while(!SD.begin(chipSelect));                                              // initialize the SD card
   Serial.println("card initialized.");                                       
   
   while(!(myFile = SD.open("new.txt", FILE_WRITE)));                         // open a file for writing
   myFile.println("ENGINEERS GARAGE Inspiring creations");                    // write the string into the file
   myFile.close();                                                            // close the file
   
   while(!(myFile = SD.open("new.txt")));                                     // open the file for reading
   while (myFile.available())                                                 // read the file till the last byte
    Serial.write(myFile.read());
   myFile.close();                                                            // close the file
   
   pinMode(6, OUTPUT);                                                        // make the pin where the LED is connected as output
}
 
void loop()
{
  digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(6, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);  
}

/* Dumps the "test.txt" file from SD card over the serial port ( works even with bluetooth adapter conected to arduino`s RX and TX). Temp senzor pin = 0 */

#include <SPI.h>
#include <SD.h>

// set pin numbers:

int tempPin = 0;

float temp;

File myFile;

void setup() {
  Serial.begin(9600);
  
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.Note that even if it's not used as the CS pin, the hardware SS pin (10 on most Arduino boards, 53 on the Mega) must be left as an output or the SD library functions will not work. 
   pinMode(10, OUTPUT);
   
 
  if (!SD.begin(4)) {
    Serial.println("Initialization failed!");
    return;
  }
 }
 
 void write_temp_onSD()
 {
  temp = analogRead(tempPin);
  temp = temp * 0.4887585532746823;
  File myFile = SD.open("test.txt", FILE_WRITE);
   // if the file is available, write to it:
  if (myFile) {
    myFile.println(temp);
    myFile.close();
    }
  else {
    Serial.println("error opening test.txt");
  }
  
 }
 
 
 void dump(){
  Serial.println("Wait for data: ");
// open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    
  // read from the file until there's nothing else in it:
    while (myFile.available()) {
    	Serial.write(myFile.read());
    }
    
    while (myFile.available())
    {
    
    }
    // close the file:
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.println("Error opening test.txt database !");
  }
}

void loop() {
  if (Serial.available())
   {     
       dump();
  } 
  else {
      write_temp_onSD();
    }
}

/*
 Dumps the "test.txt" file from SD card over the serial port ( works even with bluetooth adapter conected to arduino`s RX and TX). 
 
 Temp senzor pin = 0
 
 
*/
#include <SPI.h>
#include <SD.h>

// set pin numbers:
const int buttonPin = 8;     // the number of the pushbutton pin
int tempPin = 0;
float temp;
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

File myFile;


void setup() {
      
  // initialize the pushbutton pin as an input:
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  
  
  
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
   
 
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
 }
 
 void dump()
{
  Serial.println("Accesam fisierul ");
// open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    
  // read from the file until there's nothing else in it:
    while (myFile.available()) {
    	Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.println("Eroare la deschiderea fisierului test.txt");
  }
  
}

void loop(){
  temp = analogRead(tempPin);
  temp = temp * 0.4887585532746823;
  
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  delay(50);
   
   // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    return;
  } 
  else {
    
    dump();
   delay(2000); 
  }
   
   File myFile = SD.open("test.txt", FILE_WRITE);
   // if the file is available, write to it:
  if (myFile) {
    myFile.println(temp);
    myFile.close();
    
    delay(60000);
    
  }
  else {
    Serial.println("error opening test.txt");
  }
   
   
  
}

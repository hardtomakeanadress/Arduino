//1711591394 power button; 1711591414 OK button - samsung remote

#include <IRremote.h>

int IRpin = 2;  // pin for the IR sensor
int LED = 12;    // LED pin  
IRrecv irrecv(IRpin);
decode_results results;

boolean LEDon = true; // initializing LEDon as true

void setup()
{
 
  irrecv.enableIRIn(); // Start the receiver
  pinMode(LED, OUTPUT);
}

void loop() 
{
   
  if (irrecv.decode(&results)) 
    {
      
      irrecv.resume();   // Receive the next value
    }
  
  switch(results.value)
 {

  case 1711591394:
  digitalWrite(LED,LOW);
  break;
  
  case 1711591414:
  digitalWrite(LED,HIGH);
  break;
  
 
  
  }
  
}

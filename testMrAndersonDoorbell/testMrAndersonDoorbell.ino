/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);

  // Optional set pulse length.
   mySwitch.setPulseLength(205);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
}

void loop() {

  /* See Example: TypeA_WithDIPSwitches */
//  mySwitch.switchOn("4489215");
//  delay(1000);
//  mySwitch.switchOff("11111", "00010");
//  delay(1000);

  /* Same switch as above, but using decimal code */
//  mySwitch.send("4489215", 24);
//  delay(1000);  
//  mySwitch.send(4489215, 24);
//  delay(1000);  

  /* Same switch as above, but using binary code */
  mySwitch.send("010001000111111111111111");
//  delay(1000);  
//  mySwitch.send("000000000001010100010100");
//  delay(1000);

  /* Same switch as above, but tri-state code */ 
//  mySwitch.sendTriState("4489215");
//  delay(1000);  
//  mySwitch.sendTriState("00000FFF0FF0");
//  delay(1000);

  delay(20000);
}

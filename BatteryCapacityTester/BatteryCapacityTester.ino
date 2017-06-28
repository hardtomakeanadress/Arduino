#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define NUM_SAMPLES 10
int sum = 0; 
unsigned char sample_count = 0;

float voltage = 0.0; 

static const byte sensorPin = A6;
static const byte MOSFET = 2;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);



//new code from here
#define MOSFET_Pin 2

float Capacity = 0.0; // Capacity in mAh
float Res_Value = 10.1;  // Resistor Value in Ohm

void setup() {
  
    lcd.begin();
    // Turn on the blacklight and print a message.
    lcd.backlight();
    pinMode(MOSFET, OUTPUT);
}


void loop() {
  
  calculateVoltage();
  printVoltage();
//  printAmps();
//  printDischargeTime();
//  printBatteryCapacity();

}

void calculateVoltage() {
  while (sample_count < NUM_SAMPLES) {
        sum += analogRead(sensorPin);
        sample_count++;
        delay(20);
    };
    
  voltage = (((float)sum / (float)NUM_SAMPLES * 5.0) / 1024.0 ) * 1.005;
  sample_count = 0;
  sum = 0;
  }

void printVoltage() {
  lcd.setCursor(0, 0);
  lcd.print("Volti: ");
  lcd.print(voltage, 2);
  }

void printAmps(){
  lcd.setCursor(0, 0);
  lcd.print("Amperi: ");
  }

void printDischargeTime(){
  lcd.setCursor(0, 0);
  lcd.print("Run Time: ");
  }

void printBatteryCapacity() {
  lcd.setCursor(0, 0);
  lcd.print("Capacity: ");
  }


/* EOF - Thermometer.ino  */


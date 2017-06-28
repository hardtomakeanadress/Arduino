//In testing
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define MOSFET_Pin 2
#define Bat_Pin A6
#define Res_Pin A7

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

float Capacity = 0.0; // Capacity in mAh
float Res_Value = 10.0;  // Resistor Value in Ohm
float Vcc = 5; // Voltage of Arduino 5V pin ( Mesured by Multimeter )
float Current = 0.0; // Current in Amp
float mA=0;         // Current in mA
float Bat_Volt = 0.0;  // Battery Voltage 
float Res_Volt = 0.0;  // Voltage at lower end of the Resistor 
float Bat_High = 4.2; // Battery High Voltage
float Bat_Low = 3.2; // Discharge Cut Off Voltage
unsigned long previousMillis = 0; // Previous time in ms
unsigned long millisPassed = 0;  // Current time in ms
float sample1 =0;
float sample2= 0;

//************************ OLED Display Draw Function *******************************************************
void draw(void) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Volts:");
  lcd.print(Bat_Volt);
  lcd.setCursor(0, 1);
  lcd.print("Capacity:");
  lcd.print(Capacity, 2);
}

  void setup() {
    lcd.begin();
    lcd.backlight();
    pinMode(MOSFET_Pin, OUTPUT);
    digitalWrite(MOSFET_Pin, LOW);  // MOSFET is off during the start
   
  }
  
//************ Measuring Battery Voltage ***********
void measureBattery() {
    
//  for(int i = 1; i <= 10; i++) {
//   sample1 = sample1 + analogRead(Bat_Pin); //read the voltage from the divider circuit
//   delay (2);
//  }
//  sample1 = sample1 / 10; 
//  Bat_Volt = sample1 * Vcc / 1024.0;
  sample1=analogRead(Bat_Pin);
  Bat_Volt = sample1 * Vcc / 1024.0;
}

// *********  Measuring Resistor Voltage ***********
void measureResistor() {

//  for(int i = 1; i <= 10; i++) {
//    sample2 = sample2 + analogRead(Res_Pin); //read the voltage from the divider circuit
//    delay (2);
//  }
//  sample2 = sample2 / 10;
//  Res_Volt = sample2 * Vcc / 1024.0;
  sample2 = analogRead(Res_Pin);
  Res_Volt = sample2 * Vcc / 1024.0;

}

void makeCalculation() {
  if ( Bat_Volt > Bat_High) {
    digitalWrite(MOSFET_Pin, LOW); // Turned Off the MOSFET // No discharge 
    delay(1000);
  }
  else if(Bat_Volt < Bat_Low){
    digitalWrite(MOSFET_Pin, LOW);
    delay(1000);
  }
  else if (Bat_Volt > Bat_Low && Bat_Volt < Bat_High  ) { // Check if the battery voltage is within the safe limit
    digitalWrite(MOSFET_Pin, HIGH);
    millisPassed = millis() - previousMillis;
    Current = (Bat_Volt - Res_Volt) / Res_Value;
    mA = Current * 1000.0 ;
    Capacity = Capacity + mA * (millisPassed / 3600000.0); // 1 Hour = 3600000ms
    previousMillis = millis();
    delay(4000); 
  }
}
  
void loop() {
  draw();
  measureBattery();
  measureResistor();
  makeCalculation();  
 }    

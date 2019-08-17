#include <LiquidCrystal_I2C.h>

#define MOSFET_Pin 2
#define Bat_Pin A6
#define Res_Pin A7

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

float Capacity = 0.0; // Capacity in mAh
float Res_Value = 10.0;  // Resistor Value in Ohm
float Vcc = 5.2; // Voltage of Arduino 5V pin ( Mesured by Multimeter )
float Current = 0.0; // Current in Amp
float mA=0;         // Current in mA
float Bat_Volt = 0.0;  // Battery Voltage 
float Res_Volt = 0.0;  // Voltage at lower end of the Resistor 
float Bat_High = 4.2; // Battery High Voltage
float Bat_Low = 3.0; // Discharge Cut Off Voltage
unsigned long previousMillis = 0; // Previous time in ms
unsigned long millisPassed = 0;  // Current time in ms
bool secondMenu = false;
unsigned int internalResistance = 0;

//************************ OLED Display Draw Function *******************************************************
void draw(void) {
  lcd.clear();
  
  //first row
  lcd.setCursor(0, 0);
  lcd.print("Capacity:");
  lcd.print(Capacity, 2);

  //second row
  lcd.setCursor(0, 1);

  if (secondMenu) {
    lcd.print("Amps:");
    lcd.print(Current);  
  }
  else{
    lcd.print("Volts:");
    lcd.print(Bat_Volt);
  }
  secondMenu = !secondMenu;
}

void setup() {
  Serial.begin(9600);
  lcd.begin();
//  lcd.backlight();
  pinMode(MOSFET_Pin, OUTPUT);
  digitalWrite(MOSFET_Pin, LOW);  // MOSFET is off during the start
  measureInternalResistance();
}

//measure battery internal resistance
void measureInternalResistance() {
  delay(2000);
  float sumBat = 0.0;
  for(int i = 1; i<=100; i++){
    sumBat = sumBat + analogRead(Bat_Pin);    
  }
  Bat_Volt = (sumBat / 100) * (Vcc / 1023.0);
}
  
//************ Measuring Battery Voltage ***********
void measureBattery() {
  analogReference(DEFAULT);
  analogRead(Bat_Pin);
  delay(100);
  float sumBat = 0.0;
  for(int i = 1; i<=100; i++){
    sumBat = sumBat + analogRead(Bat_Pin);    
  }
  Bat_Volt = (sumBat / 100) * (Vcc / 1023.0);
}

// *********  Measuring Resistor Voltage ***********
void measureResistor() {
  analogReference(INTERNAL);
  analogRead(Res_Pin);
  //wait for voltage to stabilize                        
  delay(100);
  float sumRes = 0.0;
  for (int i=1; i<=100; i++) {
    sumRes = sumRes + analogRead(Res_Pin); 
  }
  Res_Volt = (sumRes / 100) * (1.1 / 1023.0);
}

void makeCalculation() {

  if(Bat_Volt <= Bat_Low){
    digitalWrite(MOSFET_Pin, LOW);
    delay(1000);
  }
  else { // Check if the battery voltage is within the safe limit
    digitalWrite(MOSFET_Pin, HIGH);
    millisPassed = millis() - previousMillis;
    Current = (Bat_Volt - Res_Volt) / Res_Value;
    mA = Current * 1000.0 ;
    Capacity = Capacity + mA * (millisPassed / 3600000.0); // 1 Hour = 3600000ms
    previousMillis = millis();
    delay(1000); 
  }
}
  
void loop() {
  measureBattery();
  measureResistor();
  makeCalculation(); 
  draw();
 }    

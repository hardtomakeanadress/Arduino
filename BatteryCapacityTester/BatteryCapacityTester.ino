//needs to be redone from scratch

#include <LiquidCrystal_I2C.h>

#define MOSFET_Pin 2
#define Bat_Pin A0
#define Res_Pin A1

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

float Capacity = 0.0; // Capacity in mAh
float Res_Value = 10.0;  // Resistor Value in Ohm
float Vcc = 4.92; // Should be measured at the 5V pin, by Multimeter )
float Current = 0.0; // Current in Amp
float mA=0;         // Current in mA
float Bat_Volt = 0.0;  // Battery Voltage 
float Res_Volt = 0.0;  // Voltage at lower end of the Resistor 
float Bat_High = 4.2; // Battery High Voltage
float Bat_Low = 3.0; // Discharge Cut Off Voltage
unsigned long previousMillis = 0; // Previous time in ms
unsigned long millisPassed = 0;  // Current time in ms
bool secondMenu = false;
bool internalResistanceCheck = true; //should be set to false by the function
float internalResistance = 0.0;

//************************ OLED Display Draw Function *******************************************************
void draw(void) {
  lcd.clear();

  if (secondMenu) {
    //first row
    lcd.setCursor(0, 0);
    lcd.print("Volts: ");
    lcd.print(Bat_Volt);

    //second row
    lcd.setCursor(0, 1);
    lcd.print("Amps: ");
    lcd.print(Current);  
  }
  else{
    //first row
    lcd.setCursor(0, 0);
    lcd.print("Capacity: ");
    lcd.print(Capacity, 2);
  
    //second row
    lcd.setCursor(0, 1);
    lcd.print("Int. Res: ");
    lcd.print(internalResistance);
  }
  secondMenu = !secondMenu;
}

void setup() {
  Serial.begin(9600);
  lcd.begin();
  pinMode(MOSFET_Pin, OUTPUT);
  digitalWrite(MOSFET_Pin, LOW);  // MOSFET is off during the start
}

//measure battery internal resistance
void measureInternalResistance() {
  delay(2000);

  //measure voltage with no load
  measureBattery();
  float noLoadVBat = Bat_Volt;
  
  digitalWrite(MOSFET_Pin, HIGH);
  delay(1000);
  
  //measure voltage under load
  measureBattery();
 
  measureResistor();
  
  digitalWrite(MOSFET_Pin, LOW);

  Current = (Bat_Volt - Res_Volt) / Res_Value;
  
  internalResistance = ((noLoadVBat - Bat_Volt) / Current)* 1000;
  Serial.print("Bat_Volt: ");
  Serial.println(Bat_Volt);
  Serial.print("Res_Volt: ");
  Serial.println(Res_Volt);
  Serial.print("Current: ");
  Serial.println(Current);
  
  Current = 0.0;
  Bat_Volt = 0.0;
  Res_Volt = 0.0;
  internalResistanceCheck = false;
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

  if (Bat_Volt < 1) {
    Bat_Volt = 0;
  }
  
  Serial.print("Battery: ");
  Serial.println(Bat_Volt);
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
  Serial.print("Resistor: ");
  Serial.println(Res_Volt);
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
  if (internalResistanceCheck && (Bat_Volt > Bat_Low)) {
    measureInternalResistance();  
  }
  measureBattery();
  measureResistor();
  makeCalculation(); 
  draw();
 }    

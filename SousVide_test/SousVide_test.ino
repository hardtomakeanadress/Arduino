
#include <LiquidCrystal_I2C.h>
#include <OneWire.h> //for sensor DS18B20
#include <DallasTemperature.h> //for sensor DS18B20

#define ONE_WIRE_PIN 3
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte selectButton = 5;
const byte downButton = 4;

bool alarm = false;
bool timeHasPassed = false;
bool wait4Heat = false;

bool heatOn = false;

int targetTime = 0;
int timeLeft = 0;

float targetTemp = 0.0;
float currentTemp = 0.0;

byte menu = 1;
byte relayPin = 2;

unsigned long counter1;
unsigned long counter2;
unsigned long counter3;
unsigned long measureTime1;
unsigned long measureTime2;

void setup() {
//  Serial.begin(9600);
  sensors.begin();
  pinMode(selectButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  lcd.begin();
  updateMenu();
  counter1 = millis();
  counter3 = millis();
}

void loop() {
  checkButtons();
  refreshScreen();
  
  readTemp();
  compareTemp();
  
  compareTime();
  check1Min();
  relayControl();
//  Serial.print("heatON ");
//  Serial.print(heatOn);
//  Serial.print(" ");
//  Serial.print("timeHasPassed ");
//  Serial.println(timeHasPassed);
}

void check1Min() {
  if (millis() - counter3 >= 60000) {
    counter3 = millis();
    wait4Heat = !wait4Heat;
  }  
}

void relayControl() {
  if((targetTime > 0) && (targetTemp > 0)){
    if(timeHasPassed == false){
      if((heatOn == true) && (!wait4Heat)){
        digitalWrite(relayPin, HIGH);
      }
      else{
        digitalWrite(relayPin, LOW);  
      }
    }
    else{
      digitalWrite(relayPin, LOW);
    } 
  }
  else{
    digitalWrite(relayPin, LOW);
  } 
}

void readTemp() {
  sensors.requestTemperatures();
  currentTemp = sensors.getTempCByIndex(0);
}

void compareTemp(){
  if((targetTemp > 0)&&(currentTemp < targetTemp)) {
    heatOn = true;  
  }
  else{
    heatOn = false;
  }
}

void compareTime(){
  measureTime2 = millis();
  int timePassed = toMinutes(measureTime2) - toMinutes(counter2);
  if (targetTime > 0) {
    if (timePassed <= targetTime){
      timeHasPassed = false;
      timeLeft = targetTime - timePassed;
    }
    else {
      timeHasPassed = true;
    }
  }
}

void checkButtons(){
  if (!digitalRead(downButton)){
    menu++;
    updateMenu();
    delay(50);
    while (!digitalRead(downButton));
  }
  
  if (!digitalRead(selectButton)){
    executeAction();
    updateMenu();
    delay(50);
    while (!digitalRead(selectButton));
  }
}

void refreshScreen(){
  measureTime1 = millis();
  if(measureTime1 - counter1 > 3000){
    counter1 = measureTime1;
    updateMenu();
  }
}

int toMinutes(unsigned long param){
  return ((param / 1000)/60);
}

void updateMenu() {
  switch (menu) {
    case 1:
      lcd.clear();
      lcd.print(">Set Timer");
      lcd.setCursor(0, 1);
      lcd.print(" Set Temperature");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Set Timer");
      lcd.setCursor(0, 1);
      lcd.print(">Set Temperature");
      break;
    case 3:
      lcd.clear();
      lcd.print(">Set Alarm");
      lcd.setCursor(0, 1);
      lcd.print(" Status");
      break;
    case 4:
      lcd.clear();
      lcd.print(" Set Alarm");
      lcd.setCursor(0, 1);
      lcd.print(">Status");
      break;
    case 5:
      menu = 1;
      updateMenu();
      break;
    case 11:
      lcd.clear();
      lcd.print(">2 Hours");
      lcd.setCursor(0, 1);
      lcd.print(" 3 Hours");
      break;
    case 12:
      lcd.clear();
      lcd.print(" 2 Hours");
      lcd.setCursor(0, 1);
      lcd.print(">3 Hours");
      break;
    case 13:
      lcd.clear();
      lcd.print(">4 Hours");
      lcd.setCursor(0, 1);
      lcd.print(" main menu");
      break;
    case 14:
      lcd.clear();
      lcd.print(" 4 Hours");
      lcd.setCursor(0, 1);
      lcd.print(">main menu");
      break;
    case 15:
      menu = 11;
      updateMenu();
      break;
    case 21:
      lcd.clear();
      lcd.print(">55 'C");
      lcd.setCursor(0, 1);
      lcd.print(" 60 'C");
      break;
    case 22:
      lcd.clear();
      lcd.print(" 55 'C");
      lcd.setCursor(0, 1);
      lcd.print(">60 'C");
      break;
    case 23:
      lcd.clear();
      lcd.print(">65 'C");
      lcd.setCursor(0, 1);
      lcd.print(" main menu");
      break;
    case 24:
      lcd.clear();
      lcd.print(" 65 'C");
      lcd.setCursor(0, 1);
      lcd.print(">main menu");
      break;
    case 25:
      menu = 21;
      updateMenu();
      break;
    case 31:
      lcd.clear();
      lcd.print(">Alarm ON");
      lcd.setCursor(0, 1);
      lcd.print(" Alarm OFF");
      break;
    case 32:
      lcd.clear();
      lcd.print(" Alarm ON");
      lcd.setCursor(0, 1);
      lcd.print(">Alarm OFF");
      break;
    case 33:
      lcd.clear();
      lcd.print(">main menu");
      break;
    case 34:
      menu = 31;
      updateMenu();
      break;
    case 41:
      lcd.clear();
      lcd.print(">");
      lcd.print("Temp");
      lcd.print(targetTemp);
      lcd.print("|");
      lcd.print(currentTemp);
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.print("Time");
      lcd.print(targetTime);
      lcd.print("|");
      lcd.print(timeLeft);
      break;
    case 42:
      lcd.clear();
      lcd.print(" ");
      lcd.print("Temp");
      lcd.print(targetTemp);
      lcd.print("|");
      lcd.print(currentTemp);
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print("Time");
      lcd.print(targetTime);
      lcd.print("|");
      lcd.print(timeLeft);
      break;
    case 43:
      lcd.clear();
      lcd.print(">main menu");
      break;
    case 44:
      menu = 41;
      updateMenu();
      break;
  }
}

void executeAction() {
  switch (menu) {
    case 1:
      goTo11();
      break;
    case 2:
      goTo21();
      break;
    case 3:
      goTo31();
      break;
    case 4:
      goTo41();
      break;
    case 5:
      goTo51();
      break;
    case 11:
      set2Hours();
      break;
    case 12:
      set3Hours();
      break;
    case 13:
      set4Hours();
      break;
    case 14:
      goToMainMenu();
      break;
    case 21:
      set55Degrees();
      break;
    case 22:
      set60Degrees();
      break;
    case 23:
      set65Degrees();
      break;
    case 24:
      goToMainMenu();
      break;
    case 31:
      setAlarmON();
      break;
    case 32:
      setAlarmOFF();
      break;
    case 33:
      goToMainMenu();
      break;
    case 43:
      goToMainMenu();
      break;
  }
}

void goToMainMenu() {
  lcd.clear();
  lcd.print(" ...loading");
  delay(500);
  menu = 1;
}

void goTo11() {
  lcd.clear();
  lcd.print(" ...loading");
  delay(500);
  menu = 11;
}
void goTo21() {
  lcd.clear();
  lcd.print(" ...loading");
  delay(500);
  menu = 21;
}
void goTo31() {
  lcd.clear();
  lcd.print(" ...loading");
  delay(500);
  menu = 31;
}
void goTo41() {
  lcd.clear();
  lcd.print(" ...loading");
  delay(500);
  menu = 41;
}
void goTo51() {
  lcd.clear();
  menu = 21;
}

void set2Hours(){
  menu = 1;               
  targetTime = 120;
  counter2 = millis(); 
  lcd.clear();
  lcd.print(" set 2 Hours");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print(" done!");
  delay(1000);
  updateMenu();
}

void set3Hours(){
  menu = 1;
  targetTime = 180;
  counter2 = millis();
  lcd.clear();
  lcd.print(" set 3 Hours");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print(" done!");
  delay(1000);
  updateMenu();
}

void set4Hours(){
  menu = 1;
  targetTime = 240;
  counter2 = millis();
  lcd.clear();
  lcd.print(" set 4 Hours");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print(" done!");
  delay(1000);
  updateMenu();
}

void set55Degrees(){
  menu = 1;
  targetTemp = 55.00;
  lcd.clear();
  lcd.print(" set 55 'C");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print(" done!");
  delay(1000);
  updateMenu();
}

void set60Degrees(){
  menu = 1;
  targetTemp = 60.00;
  lcd.clear();
  lcd.print(" set 60 'C");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print(" done!");
  delay(1000);
  updateMenu();
}

void set65Degrees(){
  menu = 1;
  targetTemp = 65.00;
  lcd.clear();
  lcd.print(" set 65 'C");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print(" done!");
  delay(1000);
  updateMenu();
}

void setAlarmON(){
  menu = 1;
  alarm = true;
  lcd.clear();
  lcd.print(" Alarm ON");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print(" done!");
  delay(1000);
  updateMenu();
}

void setAlarmOFF(){
  menu = 1;
  alarm = false;
  lcd.clear();
  lcd.print(" Alarm OFF");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print(" done!");
  delay(1000);
  updateMenu();
}

void soundAlarm(){
  
}

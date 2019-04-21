



/* !! define the proper pins for wemos d1 mini in PCD8544.h*/
/* !! remember to change back the pins for other bords than WEMOS D1 MINI or NODEMCU!!! if you need original pins restore backup of PCD8544.h*/
// set to NODEMCU 1.0 BOARD

#include <ESP8266WiFi.h>  //for WIFI
#include <PubSubClient.h> //for MQTT
#include <PCD8544.h> //for display
#include <OneWire.h> //for sensor DS18B20
#include <DallasTemperature.h> //for sensor DS18B20

// Add support for OTA***************************************
#include <ArduinoOTA.h>
// **********************************************************

#define ALARM_PIN D8

#define ONE_WIRE_PIN D3
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);

#define ADCPIN A0 //photoresistor
static PCD8544 lcd;

WiFiClient espClient;
PubSubClient client(espClient);

float sensor_temperature;
int   sensor_luminosity;
int   alert = 0;  // 0>OFF; 1> ON

long previousMillis = 0;
long send_interval = 600000; //send MQTT data interval

char temperatureData[10];
char luminosityData[10];

String tempData = "???";
String lumData = "???";
String targetTemp = "???";
String balconyTemp = "???";
String kitchenTemp = "???";
String playroomTemp = "???";

const char* ssid     = "";
const char* password = "";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *temperature_topic = "home/rooms/bedroom/sensor/temperature";
const char *luminosity_topic = "home/rooms/bedroom/sensor/luminosity";
const char *balcony_temperature = "home/rooms/balcony/sensor/temperature";
const char *kitchen_temperature = "home/rooms/kitchen/sensor/temperature";
const char *playroom_temperature = "home/rooms/playroom/sensor/temperature";
const char *target_temp = "home/rooms/kitchen/control/control_temp"; //currently target temperature for central heating
const char *alert_topic = "home/alert";

void setup(){
  
  // Add support for OTA***************************************
  //Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("BedroomSensor");
  ArduinoOTA.setPassword("bemobere");
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.begin();  /* setup the OTA server */
  // **********************************************************
  
  WiFi.hostname("BedroomSensor");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  lcd.begin(84, 48);  
  
  pinMode (ALARM_PIN, OUTPUT );
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic,"home/alert") == 0){
    char payloadMessage[length + 1];
    for (int i = 0; i < length; i++) {
      payloadMessage[i] = char(payload[i]);
    }
    alert = atoi(payloadMessage);
  }
  if (strcmp(topic,"home/rooms/kitchen/control/control_temp") == 0){
    char payloadMessage[length + 1];
    for (int i = 0; i < length; i++) {
      payloadMessage[i] = char(payload[i]);
    }
    targetTemp = String(payloadMessage);
  }
  if (strcmp(topic,"home/rooms/balcony/sensor/temperature") == 0){
    char payloadMessage[length + 1];
    for (int i = 0; i < length; i++) {
      payloadMessage[i] = char(payload[i]);
    }
    balconyTemp = String(payloadMessage);
  }
  if (strcmp(topic,"home/rooms/kitchen/sensor/temperature") == 0){
    char payloadMessage[length + 1];
    for (int i = 0; i < length; i++) {
      payloadMessage[i] = char(payload[i]);
    }
    kitchenTemp = String(payloadMessage);
  }
  if (strcmp(topic,"home/rooms/playroom/sensor/temperature") == 0){
    char payloadMessage[length + 1];
    for (int i = 0; i < length; i++) {
      payloadMessage[i] = char(payload[i]);
    }
    playroomTemp = String(payloadMessage);
  }
}

void ringAlarm(){
  digitalWrite(ALARM_PIN, HIGH);
  delay(800);
  digitalWrite(ALARM_PIN, LOW);
  delay(100);
  digitalWrite(ALARM_PIN, HIGH);
  delay(800);
  digitalWrite(ALARM_PIN, LOW);
}

void reconnectToServer() {
  while (!client.connected()) {
    if (client.connect("BedroomSensor")) {
      client.subscribe(alert_topic);
      delay(200);
      client.subscribe(target_temp);
      delay(200);
      client.subscribe(balcony_temperature);
      delay(200);
      client.subscribe(kitchen_temperature);
      delay(200);
      client.subscribe(playroom_temperature);
      delay(200);
      sendMQTTData();
    } 
    else {
      delay(2000);
    }
  }
}

void sendMQTTData(){
  client.publish(luminosity_topic,luminosityData,true);
  delay(200);
  client.publish(temperature_topic,temperatureData,true);
  delay(200);
}

void handleSensorData(){
  
  sensor_luminosity = map(analogRead(ADCPIN), 0, 1023, 0, 100);
  if (sensor_luminosity < 0) {
    sensor_luminosity = 0;    
  }

  sensors.requestTemperatures();
  sensor_temperature = sensors.getTempCByIndex(0) -1.5; //compensate for sensor heating from other components  // in testing

  if (sensor_temperature > 82) {
    return;   
  }

  lumData = String(sensor_luminosity);
  tempData = String(sensor_temperature);
  
  lumData.toCharArray(luminosityData,(lumData.length() + 1));
  tempData.toCharArray(temperatureData,(tempData.length() + 1));
}

int calculateDisplayCursor(String valueToDisplay){
  int cursorPosition = 52;
  float value = valueToDisplay.toFloat();
  
  if ((value < 0) && (value <= -10)) {
    cursorPosition = cursorPosition - 6;   
  }
  else if ((value > 0) && (value < 10)) {
    cursorPosition = cursorPosition + 6;  
  }

  return cursorPosition;  
}

void writeDisplay(){
//  lcd.setInverse(true); //inverse black/white
//  lcd.clear(); //clears all screen

  lcd.setContrast(55);

//  lcd.setCursor(0, 0);
//  lcd.clearLine();
//  lcd.print("Light");
//  lcd.setCursor(calculateDisplayCursor(String(sensor_luminosity)), 0);
//  lcd.print(sensor_luminosity);
  
  lcd.setCursor(0, 1);
  lcd.clearLine();
  lcd.print("Target");
  lcd.setCursor(calculateDisplayCursor(targetTemp), 1);
  lcd.print(targetTemp);

  lcd.setCursor(0, 2);
  lcd.clearLine();     
  lcd.print("Kitchen"); 
  lcd.setCursor(calculateDisplayCursor(kitchenTemp), 2);
  lcd.print(kitchenTemp);
  
  lcd.setCursor(0, 3);
  lcd.clearLine();
  lcd.print("Bedroom"); 
  lcd.setCursor(calculateDisplayCursor(tempData), 3);
  lcd.print(tempData);

  lcd.setCursor(0, 4);
  lcd.clearLine();
  lcd.print("Playroom"); 
  lcd.setCursor(calculateDisplayCursor(playroomTemp), 4);
  lcd.print(playroomTemp);
  
  lcd.setCursor(0, 5);
  lcd.clearLine();
  lcd.print("Balcony"); 
  lcd.setCursor(calculateDisplayCursor(balconyTemp), 5);
  lcd.print(balconyTemp);
}

void loop() {
  // Add support for OTA***************************************
  ArduinoOTA.handle();
  // **********************************************************
  unsigned long currentMillis = millis();

  if(!client.connected()){
    reconnectToServer();   
  }
  
  if(currentMillis - previousMillis > send_interval) {
    previousMillis = currentMillis;
    sendMQTTData();
  }
  
  handleSensorData();
  writeDisplay();
  
  if (alert == 1) {
    ringAlarm();  
  }
  client.loop();
}



/* define the proper pins for wemos d1 mini in PCD8544.h*/
/* remember to change the pins !!! if you need original pins restore backup*/

#include <ESP8266WiFi.h>  //for WIFI
#include <PubSubClient.h> //for MQTT
#include <PCD8544.h> //for display
#include <OneWire.h> //for sensor DS18B20
#include <DallasTemperature.h> //for sensor DS18B20

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

String targetTemp = "???";
String balconyTemp = "???";

const char* ssid     = "warz";
const char* password = "parola!derezerva";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *temperature_topic = "home/rooms/bedroom/sensor/temperature";
const char *luminosity_topic = "home/rooms/bedroom/sensor/luminosity";
const char *balcony_temperature = "home/rooms/balcony/sensor/temperature";
const char *target_temp = "home/rooms/kitchen/control/control_temp"; //currently target temperature for central heating
const char *alert_topic = "home/alert";

void setup(){
  WiFi.hostname("BedroomSensor");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  lcd.begin(84, 48);  
  Serial.begin(9600);
  
  pinMode (ALARM_PIN, OUTPUT );
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println(String(topic));
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
}

void ringAlarm(){
  Serial.println("ALARM!");
  tone(ALARM_PIN,1000);
  delay(800);
  noTone(ALARM_PIN);
  delay(100);
  tone(ALARM_PIN,1000);
  delay(800);
  noTone(ALARM_PIN);
}

void reconnectToServer() {
  Serial.println("reconnect to server");
  while (!client.connected()) {
    if (client.connect("BedroomSensor")) {
      client.subscribe(alert_topic);
      delay(200);
      client.subscribe(target_temp);
      delay(200);
      client.subscribe(balcony_temperature);
      delay(200);
      sendMQTTData();
    } 
    else {
      delay(2000);
    }
  }
}

void sendMQTTData(){
  client.publish(luminosity_topic,luminosityData);
  delay(200);
  client.publish(temperature_topic,temperatureData);
  delay(200);
  Serial.println("send mqtt data");
}

int checkAlert(int new_alert_value){
  Serial.println("check alert");
  if (new_alert_value == 1) {
    ringAlarm();  
  }
}

void handleSensorData(){
  
  sensor_luminosity = map(analogRead(ADCPIN), 100, 1023, 0, 100);
  if (sensor_luminosity < 0) {
    sensor_luminosity = 0;  
  }

  sensors.requestTemperatures();
  sensor_temperature = sensors.getTempCByIndex(0);
  
  String lumData = String(sensor_luminosity);
  String tempData = String(sensor_temperature);
  
  lumData.toCharArray(luminosityData,(lumData.length() + 1));
  tempData.toCharArray(temperatureData,(tempData.length() + 1));
  Serial.println("handle sensor data | ");
}

void writeDisplay(){
  lcd.setContrast(50);
  
  lcd.setCursor(0, 0);
  lcd.clearLine(); //or lcd.clear()
  lcd.print(sensor_luminosity);
  lcd.setCursor(0, 1);
  lcd.clearLine();
  lcd.print(sensor_temperature); 
  lcd.setCursor(0, 2);
  lcd.clearLine();
  lcd.print(balconyTemp); 
  lcd.setCursor(0, 3);
  lcd.clearLine();
  lcd.print(targetTemp);
}

void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis > send_interval) {
    previousMillis = currentMillis;
    sendMQTTData();
  }
  
  handleSensorData();
  writeDisplay();
  if(!client.connected()){
    reconnectToServer();   
  };
  
  if (alert == 1) {
    ringAlarm();  
  }
  delay(1000);
  client.loop();
  delay(1000);

}

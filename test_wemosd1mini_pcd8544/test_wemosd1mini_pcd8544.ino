

/* define the proper pins for wemos d1 mini in PCD8544.h*/

/* remember to change the pins !!! if you need original pins restore backup*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <PCD8544.h>
#include <OneWire.h>
 
 
OneWire  ds(D8);  // on pin D4 (a 4.7K resistor is necessary)

#define ADCPIN A0
static PCD8544 lcd;

WiFiClient espClient;
PubSubClient client(espClient);

float sensor_temperature;
int   sensor_luminosity;

char temperatureData[10];
char luminosityData[10];

const char* ssid     = "";
const char* password = "";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *humidity_topic = "home/rooms/bedroom/sensor/temperature";
const char *voltage_topic = "home/rooms/bedroom/sensor/luminosity";

void setup(){
  WiFi.hostname("BedroomSensor");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  lcd.begin(84, 48);  
  client.setServer(mqttServer, mqttPort);
}


void loop() {
  sensor_luminosity = map(analogRead(ADCPIN), 100, 1023, 0, 100);
  lcd.setCursor(0, 0);
  lcd.clearLine(); //or lcd.clear()
  lcd.print(sensor_luminosity);
  lcd.setCursor(0, 1);
  lcd.print("Hello, World!");
}

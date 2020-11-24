
// ESP12E barebone 
// 2MB memory no SPIFF

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Wire.h> //sensor is I2C
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define ADCPIN A0

Adafruit_BME280 bme;

WiFiClient espClient;
PubSubClient client(espClient);

float sensor_humidity,sensor_temperature,sensor_pressure,sensor_voltage;
unsigned int readingInterval = 30;  //minutes

char humidityData[10];
char temperatureData[10];
char pressureData[10];
char voltageData[10];

const char* ssid     = "warz2";
const char* password = "parola!derezerva";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *temperature_topic = "home/rooms/balcony/sensor/temperature";
const char *pressure_topic = "home/rooms/balcony/sensor/pressure";
const char *humidity_topic = "home/rooms/balcony/sensor/humidity";
const char *voltage_topic = "home/rooms/balcony/sensor/voltage";

void setup(){
  WiFi.hostname("BalconySensor");
  WiFi.begin(ssid, password);
  
  bme.begin(0x76);
  
  client.setServer(mqttServer, mqttPort);
  delay(200);
}

void SendMQTTData(){
  client.publish(humidity_topic,humidityData,true);
  delay(200);
  client.publish(temperature_topic,temperatureData,true);
  delay(200);
  client.publish(pressure_topic,pressureData,true);
  delay(200);
  client.publish(voltage_topic, voltageData,true);
  delay(200);
}

void handleSensorData(){
  sensor_humidity    = bme.readHumidity();
  sensor_pressure    = bme.readPressure() / 100.0F; 
  sensor_temperature = bme.readTemperature();
  sensor_voltage     = analogRead(ADCPIN)* 5.62 / 1023;  //5.62 is voltage divider raport ; R1/R2
  
  String humData = String(sensor_humidity);
  String pressData = String(sensor_pressure); 
  String tempData = String(sensor_temperature);
  String voltData = String(sensor_voltage);
  
  humData.toCharArray(humidityData,(humData.length() + 1));
  pressData.toCharArray(pressureData,(pressData.length() + 1));
  tempData.toCharArray(temperatureData,(tempData.length() + 1));
  voltData.toCharArray(voltageData,(voltData.length() + 1));
}

void loop() {
  client.loop();
  handleSensorData();
  delay(200);
  client.loop();
  SendMQTTData();
  delay(200);
  client.loop();
  ESP.deepSleep(readingInterval * 60000000,WAKE_RF_DEFAULT);
}

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 2 // what pin we’re connected to
#define ADCPIN A0

DHT dht(DHTPIN, DHT11, 15);

WiFiClient espClient;
PubSubClient client(espClient);

float sensor_humidity,sensor_temperature,sensor_voltage;
unsigned int readingInterval = 1;  //minutes

char humidityData[10];
char temperatureData[10];
char voltageData[10];

const char* ssid     = "";
const char* password = "";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *temperature_topic = "home/rooms/bedroom/sensor/temperature";
const char *humidity_topic = "home/rooms/bedroom/sensor/humidity";
const char *voltage_topic = "home/rooms/bedroom/sensor/voltage";

void setup(){
  WiFi.hostname("BedroomSensor");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  dht.begin();
  
  client.setServer(mqttServer, mqttPort);
}

void getAndSendMQTTDAta(){
  client.publish(humidity_topic,humidityData);
  delay(100);
  client.publish(temperature_topic,temperatureData);
  delay(100);
  client.publish(voltage_topic, voltageData);
  delay(100);
}

void reconnectToServer() {
  while (!client.connected()) {
    if (client.connect("BedroomSensor")) {
      getAndSendMQTTDAta();
    } 
    else {
      delay(2000);
    }
  }
}

void handleSensorData(){
  sensor_humidity    = dht.readHumidity(); 
  sensor_temperature = dht.readTemperature();
  sensor_voltage     = analogRead(ADCPIN) * (4.2 / 1023);
  
  String humData = String(sensor_humidity); 
  String tempData = String(sensor_temperature);
  String voltData = String(sensor_voltage);
  
  humData.toCharArray(humidityData,(humData.length() + 1));
  tempData.toCharArray(temperatureData,(tempData.length() + 1));
  voltData.toCharArray(voltageData,(voltData.length() + 1));
}

void loop() {
  delay(1000);
  handleSensorData();
  if (client.connected()) {
    getAndSendMQTTDAta();
  }
  else
    reconnectToServer();
  client.loop();
  ESP.deepSleep(readingInterval * 60000000,WAKE_RF_DEFAULT); 
}

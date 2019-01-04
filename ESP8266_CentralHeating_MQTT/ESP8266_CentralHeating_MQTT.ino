//updated for esp01

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

  //DHT Sensor Settings
#define DHTTYPE DHT11
#define DHTPIN  2 // 
  //Relay Control Pin & Settings
int RELAYPIN = 3; // 
DHT dht(DHTPIN, DHTTYPE,15);

unsigned long startMillis;
unsigned long currentMillis;
unsigned long interval = 180000;  //3 min

int sensor_humidity,sensor_temperature;
int control_temperature = 20; //we set a default value

char humidityData[10];
char temperatureData[10];
char heatingStatus[10];

const char* ssid     = "warz";
const char* password = "parola!derezerva";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *temperature_topic = "home/rooms/kitchen/sensor/temperature"; //send temp
const char *humidity_topic = "home/rooms/kitchen/sensor/humidity";     // send hum
const char *control_temp = "home/rooms/kitchen/control/control_temp"; //get desired temp
const char *heating_status = "home/rooms/kitchen/heating/status"; // heating: on/off

WiFiClient espClient;
PubSubClient client(espClient);

bool actionTime() {
  if (currentMillis - startMillis >= interval){
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
    return true;
  }
  else
    return false;
}

void getAndSendMQTTDAta(){
  client.subscribe(control_temp);
  delay(100);
  client.publish(humidity_topic,humidityData);
  delay(100);
  client.publish(temperature_topic,temperatureData);
  delay(100);
  client.publish(heating_status, heatingStatus);
}

void reconnectToServer() {
  while (!client.connected()) {
    if (client.connect("KitchenSensor")) {
      getAndSendMQTTDAta();
    } 
    else {
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  char payloadMessage[2];
  for (int i = 0; i < length; i++) {
    payloadMessage[i] = char(payload[i]);
  }
  control_temperature = atoi(payloadMessage);
  controlHeating();
}

void handleSensorData(){
  sensor_humidity    = dht.readHumidity(); 
  sensor_temperature = dht.readTemperature();
  
  String humData = String(sensor_humidity); 
  String tempData = String(sensor_temperature);
  humData.toCharArray(humidityData,(humData.length() + 1));
  tempData.toCharArray(temperatureData,(tempData.length() + 1));
}

void setup(){
  WiFi.hostname("KitchenSensor");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  pinMode(RELAYPIN, FUNCTION_3);
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW); 
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  startMillis = millis();
}

void controlHeating() {
  String buffer = "";
  if (sensor_temperature < control_temperature) {
    buffer = "ON";
    buffer.toCharArray(heatingStatus,(buffer.length() + 1));
    digitalWrite(RELAYPIN, HIGH);
  } 
  else{
    buffer = "OFF";
    buffer.toCharArray(heatingStatus,(buffer.length() + 1));
    digitalWrite(RELAYPIN, LOW);
  }
}

void loop(){
  currentMillis = millis();
  delay(5000);
  handleSensorData();
  controlHeating();
  if (actionTime() && client.connected()) {
    getAndSendMQTTDAta();
  }
  else
    reconnectToServer();
  client.loop();
}

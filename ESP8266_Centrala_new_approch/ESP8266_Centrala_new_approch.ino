//this is for bare module esp12e with adapter plate

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

  //DHT Sensor Settings
#define DHTTYPE DHT11
#define DHTPIN  13 // 

DHT dht(DHTPIN, DHT11,15);

  //Relay Control Pin & Settings
int RELAYPIN = 2; // 

unsigned long startMillis;
unsigned long currentMillis;
unsigned long interval = 120000;

float sensor_humidity,sensor_temperature;
float control_temperature = 22.0; //we set a default value

char humidityData[5];
char temperatureData[5];

const char* ssid     = "warz";
const char* password = "parola!derezerva";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *temperature_topic = "home/rooms/kitchen/sensor/temperature"; //send temp
const char *humidity_topic = "home/rooms/kitchen/sensor/humidity";     // send hum
const char *control_temp = "home/rooms/kitchen/control/control_temp"; //get desired temp

WiFiClient espClient;
PubSubClient client(espClient);



bool actionTime() {
  if (currentMillis - startMillis >= interval)  //test whether the interval has elapsed
  {
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
    return true;
  }
  else
    return false;
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("TestHeating")) {
      client.subscribe(control_temp);
      client.publish(humidity_topic,humidityData);
      client.publish(temperature_topic,temperatureData);
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
  control_temperature = atof(payloadMessage);
  triggerTempUpdate();
}

void triggerTempUpdate() {
  if (sensor_temperature < control_temperature) {
    digitalWrite(RELAYPIN, LOW); //reverse this to high
  } 
  else
    digitalWrite(RELAYPIN, HIGH);    //  reverse this to LOW
}

void handleSensorData(){
  delay(1000);
  sensor_humidity    = dht.readHumidity(); 
  sensor_temperature = dht.readTemperature();
  Serial.println(sensor_humidity);
  Serial.println(sensor_temperature);
  String humData = String(sensor_humidity); 
  String tempData = String(sensor_temperature);
  humData.toCharArray(humidityData,(humData.length() + 1));
  tempData.toCharArray(temperatureData,(tempData.length() + 1));
}

void setup(){
  WiFi.hostname("CentralHeatingUnit");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("wifi not connected");
  }
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, HIGH); //reverse this to low
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  startMillis = millis();
  Serial.begin(9600);
  Serial.println("end of setup");
}

void loop(){
  currentMillis = millis();
  handleSensorData();

  if (actionTime()) {
    client.publish(humidity_topic,humidityData);
    client.publish(temperature_topic,temperatureData);
    Serial.println("from loop>actiontime");
    Serial.println(humidityData);
    Serial.println(temperatureData);
    if (sensor_temperature < control_temperature) {
      digitalWrite(RELAYPIN, LOW); //reverse this to HIGH
    } 
    else
      digitalWrite(RELAYPIN, HIGH); //reverse this to LOW
  }
  
  if (!client.connected()){
    reconnect();
  }
  client.loop();
  Serial.println("end of loop");
}

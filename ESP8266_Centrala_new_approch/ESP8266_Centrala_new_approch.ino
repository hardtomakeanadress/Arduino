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
unsigned long interval = 120000;

float sensor_humidity,sensor_temperature;
float control_temperature = 22.0; //we set a default value

char humidityData[10];
char temperatureData[10];

const char* ssid     = "";
const char* password = "";

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

void getAndSendMQTTDAta(){
  client.subscribe(control_temp);
  delay(100);
  client.publish(humidity_topic,humidityData);
  delay(100);
  client.publish(temperature_topic,temperatureData);
  delay(100);    
}

void reconnectToServer() {
  while (!client.connected()) {
    if (client.connect("TestHeating")) {
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
  control_temperature = atof(payloadMessage);
  controlHeating();
}

void handleSensorData(){
  delay(1000);
  sensor_humidity    = dht.readHumidity(); 
  sensor_temperature = dht.readTemperature();
  
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
  }
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW); 
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  startMillis = millis();
}

void controlHeating() {
  if (sensor_temperature < control_temperature) {
    digitalWrite(RELAYPIN, HIGH); 
  } 
  else
    digitalWrite(RELAYPIN, LOW); 
}

void loop(){
  currentMillis = millis();
  handleSensorData();
  controlHeating();
  if (actionTime() && client.connected()) {
    getAndSendMQTTDAta();
  }
  else
    reconnectToServer();
    
  client.loop();
  delay(10000);
}

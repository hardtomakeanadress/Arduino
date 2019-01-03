//updated for esp01

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

  //DHT Sensor Settings
#define DHTTYPE DHT11
#define DHTPIN  13 // 
  //Relay Control Pin & Settings
int RELAYPIN = 2; // 
DHT dht(DHTPIN, DHTTYPE,15);

unsigned long startMillis;
unsigned long currentMillis;
unsigned long interval = 180000;  //3 min

int sensor_humidity,sensor_temperature;
int control_temperature = 20; //we set a default value

char humidityData[2];
char temperatureData[2];

const char* ssid     = "warz";
const char* password = "parola!derezerva";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *temperature_topic = "home/rooms/playroom/sensor/temperature"; //send temp
const char *humidity_topic = "home/rooms/playroom/sensor/humidity";     // send hum
const char *control_temp = "home/rooms/playroom/control/control_temp"; //get desired temp
const char *sensor_status = "home/rooms/playroom/sensor/status"; // heating: on/off

WiFiClient espClient;
PubSubClient client(espClient);

bool actionTime() {
  if (currentMillis - startMillis >= interval){
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
    return true;
    Serial.println("Action TIME !~");
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
  Serial.println("in GEtandsendmQTT data");    
}

void reconnectToServer() {
  while (!client.connected()) {
    if (client.connect("playroom")) {
      getAndSendMQTTDAta();
      Serial.println("Reconected ok, after getandsendmqttdata");  
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
  Serial.print("We got this temperature: ");
  Serial.println(control_temperature);
//  controlHeating();   //testing - temp control only from loop ; this is to eliminate bad connection and bad calls from topic-brocker connection
}

void handleSensorData(){
  sensor_humidity    = dht.readHumidity(); 
  sensor_temperature = dht.readTemperature();
  
  String humData = String(sensor_humidity); 
  String tempData = String(sensor_temperature);
  Serial.print(humData);
  Serial.print("|");
  Serial.print(tempData);
  humData.toCharArray(humidityData,(humData.length() + 1));
  tempData.toCharArray(temperatureData,(tempData.length() + 1));
}

void setup(){
  WiFi.hostname("PlayroomSensor");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  dht.begin();
  Serial.begin(9600);
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW); 
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  startMillis = millis();
  Serial.println("End of setup function");
}

void controlHeating() {
  if (sensor_temperature < control_temperature) {
    digitalWrite(RELAYPIN, HIGH);
    client.publish(sensor_status,"ON");
    Serial.println("Heat ON");
  } 
  else{
    digitalWrite(RELAYPIN, LOW);
    client.publish(sensor_status,"OFF");
    Serial.println("Heat OFF");
  }
}

void loop(){
  currentMillis = millis();
  delay(5000);
  handleSensorData();
  controlHeating();
  if (actionTime() && client.connected()) {
    getAndSendMQTTDAta();
    Serial.println("After getandsend mqtt data");
  }
  else
    reconnectToServer();
  client.loop();
  Serial.println("after client.loop");
}

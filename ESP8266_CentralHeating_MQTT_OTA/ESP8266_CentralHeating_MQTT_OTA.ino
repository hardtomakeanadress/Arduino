
//THIS is for Generic ESP8266 ESP01; 
//when uploading to my board set 1MB memory no SPIFF

// Add support for OTA***************************************
#include <ArduinoOTA.h>
// **********************************************************

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN  2 

int gasSensorPin = 1;
byte gasSensorState = 0;

int RELAYPIN = 3; 
DHT dht(DHTPIN, DHTTYPE,15);

unsigned long startMillis;
unsigned long currentMillis;
unsigned long interval = 180000;  //3 min

float sensor_humidity,sensor_temperature;   // we should change this to float
float control_temperature; 
float default_temperature = 20.0;//we set a default value


char humidityData[10];
char temperatureData[10];
char heatingStatus[10];

const char* ssid     = "";
const char* password = "";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *temperature_topic = "home/rooms/kitchen/sensor/temperature"; //send temp
const char *humidity_topic = "home/rooms/kitchen/sensor/humidity";     // send hum
const char *control_temp = "home/rooms/kitchen/control/control_temp"; //get desired temp
const char *heating_status = "home/rooms/kitchen/heating/status"; // heating: on/off
const char *gas_topic = "home/rooms/kitchen/sensor/gas";

WiFiClient espClient;
PubSubClient client(espClient);

void subscribeToTopic(){
  client.subscribe(control_temp);
  delay(200);
}

void publishSensorData(){
  client.publish(humidity_topic,humidityData,true);
  delay(200);
  client.publish(temperature_topic,temperatureData,true);
  delay(200);
}

void reconnectToServer() {
  while (!client.connected()) {
    if (client.connect("KitchenSensor")) {
      subscribeToTopic();
      publishSensorData();
      client.publish(heating_status, heatingStatus,true);
      delay(200);  
    } 
    else {
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  char payloadMessage[length + 1];
  float val;
  for (int i = 0; i < length; i++) {
    payloadMessage[i] = char(payload[i]);
  }
  val = atof(payloadMessage);
  
  if(val >= default_temperature){
    control_temperature = val;
  }
  else control_temperature = default_temperature;
}

void handleSensorData(){
  sensor_humidity    = dht.readHumidity(); 
  sensor_temperature = dht.readTemperature() - 1; //compensate for heating from other components ; //testing
  
  String humData = String(sensor_humidity); 
  String tempData = String(sensor_temperature);
  humData.toCharArray(humidityData,(humData.length() + 1));
  tempData.toCharArray(temperatureData,(tempData.length() + 1));
}

void setup(){
  // Add support for OTA***************************************
  ArduinoOTA.setHostname("KitchenSensor");
  ArduinoOTA.setPassword("bemobere");
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.begin();  /* setup the OTA server */
  // **********************************************************
  
  WiFi.hostname("KitchenSensor");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  pinMode(RELAYPIN, FUNCTION_3);
  pinMode(gasSensorPin, FUNCTION_3);
  
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW); 
  
  pinMode(gasSensorPin, FUNCTION_3);
  pinMode(gasSensorPin, INPUT);
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  client.subscribe(control_temp);
  
  startMillis = millis();
}

void controlHeating() {
  String buffer = "";
  if (sensor_temperature < control_temperature) {
    buffer = "ON";
    digitalWrite(RELAYPIN, HIGH);
  } 
  else{
    buffer = "OFF";
    digitalWrite(RELAYPIN, LOW);
  }
  buffer.toCharArray(heatingStatus,(buffer.length() + 1));
  client.publish(heating_status, heatingStatus, true);
  delay(200);
}

bool runTime() {
  if (currentMillis - startMillis >= interval){
    startMillis = currentMillis;  //IMPORTANT to save the start time 
    return true;
  }
  else
    return false;
}

void checkGasSensor() {
  byte gasSensorNewState = digitalRead(gasSensorPin);

  if (gasSensorNewState != gasSensorState) {
    if (gasSensorNewState == 0) {
      client.publish(gas_topic, "OK",true); 
      delay(200);
    }
    else if (gasSensorNewState == 1) {
      client.publish(gas_topic, "Gas Leak! ALERT !",true);  
      delay(200); 
    }
  }
  gasSensorState = gasSensorNewState;
}

void loop(){
  currentMillis = millis();
  
  if (runTime()) {
    client.subscribe(control_temp);
    handleSensorData();
    controlHeating();
    if(client.connected()) {
      publishSensorData();    
    }
    else {
      reconnectToServer();  
    }
  }

  checkGasSensor();
  
  // Add support for OTA***************************************
  ArduinoOTA.handle();
  // **********************************************************
  
  client.loop();
}

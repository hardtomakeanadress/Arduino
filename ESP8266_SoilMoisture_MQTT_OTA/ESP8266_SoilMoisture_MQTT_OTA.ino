
// set to NODEMCU 1.0 BOARD
// 4MB memory no SPIFF

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Add support for OTA***************************************
#include <ArduinoOTA.h>
// **********************************************************

#define ADCPIN A0
#define SENSORPWRPIN D1

WiFiClient espClient;
PubSubClient client(espClient);

int soil_humidity;
unsigned int readingInterval = 120;  //minutes

int  program_mode=3;  //   ProgramMode OFF=0; ProgramMode ON=1

char soilHumidityData[10];

char programModeStatus[5];

const char* ssid     = "";
const char* password = "";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;


const char *program_mode_topic = "home/rooms/balcony/sensor/program_mode/parsley";
const char *program_mode_status = "home/rooms/balcony/sensor/program_mode/status/parsley";
const char *humidity_topic = "home/rooms/balcony/sensor/soil/parsley";
//const char *humidity_topic = "home/debug";

void setup(){

  // Add support for OTA***************************************
  //Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("ParsleySensor");
  ArduinoOTA.setPassword("bemobere");
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.begin();  /* setup the OTA server */
  // **********************************************************
  
  WiFi.hostname("ParsleySensor");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  delay(200);
  
  pinMode(SENSORPWRPIN, OUTPUT);
}

void publishProgramModeStatus(String value){
  value.toCharArray(programModeStatus,(value.length() + 1));
  client.publish(program_mode_status, programModeStatus, true); //topic, value, retain
  delay(200);  
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic,"home/rooms/balcony/sensor/program_mode/parsley") == 0){
    char payloadMessage[length + 1];
    for (int i = 0; i < length; i++) {
      payloadMessage[i] = char(payload[i]);
    }
    program_mode = atoi(payloadMessage);
  }
}

void SendMQTTData(){
  client.publish(humidity_topic,soilHumidityData,true);
  delay(200);
}

void reconnectToServer() {
  while (!client.connected()) {
    if (client.connect("ParsleySensor")) {
      client.subscribe(program_mode_topic);
      delay(200);
      client.loop();
    } 
    else {
      delay(2000);
      client.loop();
    }
  }
}

void handleSensorData(){
  digitalWrite(SENSORPWRPIN, HIGH);
  delay(500);
  
  soil_humidity = 0;
  
  for (int i= 1; i <= 10; i++) {
    soil_humidity += map(analogRead(ADCPIN), 100, 600, 100, 0);
  }

//  for (int i= 1; i <= 10; i++) {
//    soil_humidity = soil_humidity + analogRead(ADCPIN);
//  }

  soil_humidity = soil_humidity / 10;

  digitalWrite(SENSORPWRPIN, LOW);
  
  String soilHumidity = String(soil_humidity);
  
  soilHumidity.toCharArray(soilHumidityData,(soilHumidity.length() + 1));
}

void loop() {
  if (client.connected()){
    client.subscribe(program_mode_topic);
    delay(200);
    client.loop();
  }
  else{
    reconnectToServer();
    client.loop();
  }

  if (program_mode == 0) {
    handleSensorData();
    
    if (client.connected()){
      SendMQTTData();
      client.loop();
    }
    else{
      reconnectToServer();
      SendMQTTData();
      client.loop();
    }
    publishProgramModeStatus("0");
    client.loop();
    ESP.deepSleep(readingInterval * 60000000,WAKE_RF_DEFAULT);
  }
  else if (program_mode == 1){ 
    publishProgramModeStatus("1");
    client.loop();
    ArduinoOTA.handle();
    client.loop();
  }
  client.loop();
}

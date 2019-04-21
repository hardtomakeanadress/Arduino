
// ESP12E barebone 
// 2MB memory no SPIFF

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Add support for OTA***************************************
#include <ArduinoOTA.h>
// **********************************************************

#define DHTPIN 4 
#define ADCPIN A0

DHT dht(DHTPIN, DHT22, 15);

WiFiClient espClient;
PubSubClient client(espClient);

float sensor_humidity,sensor_temperature,sensor_voltage;
unsigned int readingInterval = 10;  //minutes

int  program_mode=3;  //   ProgramMode OFF=0; ProgramMode ON=1

char humidityData[10];
char temperatureData[10];
char voltageData[10];

char programModeStatus[5];

const char* ssid     = "";
const char* password = "";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *temperature_topic = "home/rooms/balcony/sensor/temperature";
const char *humidity_topic = "home/rooms/balcony/sensor/humidity";
const char *voltage_topic = "home/rooms/balcony/sensor/voltage";
const char *program_mode_topic = "home/rooms/balcony/sensor/program_mode";
const char *program_mode_status = "home/rooms/balcony/sensor/program_mode/status";
//const char *debug = "home/debug";

void setup(){

  // Add support for OTA***************************************
  //Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("BalconySensor");
  ArduinoOTA.setPassword("bemobere");
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.begin();  /* setup the OTA server */
  // **********************************************************
  
  WiFi.hostname("BalconySensor");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  dht.begin();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  delay(200);
}

void publishProgramModeStatus(String value){
  value.toCharArray(programModeStatus,(value.length() + 1));
  client.publish(program_mode_status, programModeStatus, true); //topic, value, retain
  delay(200);  
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic,"home/rooms/balcony/sensor/program_mode") == 0){
    char payloadMessage[length + 1];
    for (int i = 0; i < length; i++) {
      payloadMessage[i] = char(payload[i]);
    }
    program_mode = atoi(payloadMessage);
  }
}

void SendMQTTData(){
  client.publish(humidity_topic,humidityData,true);
  delay(200);
  client.publish(temperature_topic,temperatureData,true);
  delay(200);
  client.publish(voltage_topic, voltageData,true);
  delay(200);
}

void reconnectToServer() {
  while (!client.connected()) {
    if (client.connect("BalconySensor")) {
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
  sensor_humidity    = dht.readHumidity(); 
  sensor_temperature = dht.readTemperature();
  sensor_voltage     = analogRead(ADCPIN)* 5.62 / 1023;  //5.62 is voltage divider raport ; R1/R2
  
  String humData = String(sensor_humidity); 
  String tempData = String(sensor_temperature);
  String voltData = String(sensor_voltage);
  
  humData.toCharArray(humidityData,(humData.length() + 1));
  tempData.toCharArray(temperatureData,(tempData.length() + 1));
  voltData.toCharArray(voltageData,(voltData.length() + 1));
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

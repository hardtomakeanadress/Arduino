
//THIS is for Generic ESP8266 ESP01; 
//when uploading to my board set 1MB memory no SPIFF

// Add support for OTA***************************************
#include <ArduinoOTA.h>
// **********************************************************

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int gasSensorPin = 1;

byte gasSensorState = 0;

unsigned long startMillis;
unsigned long currentMillis;
unsigned long interval = 10000;  //10 sec

const char* ssid     = "";
const char* password = "";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *gas_topic = "home/rooms/kitchen/sensor/gas";

WiFiClient espClient;
PubSubClient client(espClient);

void setup(){
  // Add support for OTA***************************************
  ArduinoOTA.setHostname("KitchenGasSensor");
  ArduinoOTA.setPassword("bemobere");
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.begin();  /* setup the OTA server */
  // **********************************************************
  
  WiFi.hostname("KitchenGasSensor");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  pinMode(gasSensorPin, FUNCTION_3);
  pinMode(gasSensorPin, INPUT);
  digitalWrite(gasSensorPin, LOW);
  
  client.setServer(mqttServer, mqttPort);
  startMillis = millis();
}

void checkGasSensor() {
  byte gasSensorNewState = digitalRead(gasSensorPin);

  if (gasSensorNewState != gasSensorState) {
    if (gasSensorNewState == 1){
      client.publish(gas_topic, "Gas leak",true); // third parameter is to retain message
      delay(200);
    }
    else {
      client.publish(gas_topic, "OK", true); // third parameter is to retain message
      delay(200);
    }
  gasSensorState = gasSensorNewState;
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("KitchenGasSensor")) {
      delay(200);
      checkGasSensor();
    } 
    else {
      delay(1000);
    }
  }
}

void loop(){
  currentMillis = millis();
  
  if (!client.connected()) {
    reconnect();
  }
  
  if (currentMillis - startMillis >= interval){
    startMillis = currentMillis;  //IMPORTANT to save the start time 
    gasSensorState = digitalRead(gasSensorPin);
    if (gasSensorState == 0) {
      client.publish(gas_topic, "OK",true); 
      delay(200);
    }
    else if (gasSensorState == 1) {
      client.publish(gas_topic, "Gas Leak! ALERT !",true);  
      delay(200); 
    }
    else {
      client.publish(gas_topic, "Some kind of error :(",true);
      delay(200);
    }
  }
  client.loop();
  // Add support for OTA***************************************
  ArduinoOTA.handle();
  // **********************************************************
}

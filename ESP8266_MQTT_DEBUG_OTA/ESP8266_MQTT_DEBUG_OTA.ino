

/*
this sketch was written on a esp12f barebone module
IT IS DIFFERENT FROM NODEMCU BOARD OR WEMOS D1MINI:
  my barebone esp12f > GENERIC ESP8266 module with - 2Mb ; no SPIFFS
  - you can use OTA with these settings.                
 
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Add support for OTA***************************************
#include <ArduinoOTA.h>
// **********************************************************

const char* ssid     = "";
const char* password = "";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *debug = "home/debug";  //topic that gets the debug messages


unsigned long startMillis;
unsigned long currentMillis;
unsigned long interval = 10000;

WiFiClient espClient;
PubSubClient client(espClient);

void reconnectToServer() {
  while (!client.connected()) {
    if (client.connect("ESP8266DebugTest")) {
      client.subscribe(debug);
      delay(200);
    } 
    else {
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic,"home/debug") == 0){
    Serial.print("Callback:");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  }
}

void setup(){
  // Add support for OTA***************************************
  ArduinoOTA.setHostname("ESP8266DebugTest");
  ArduinoOTA.setPassword("bemobere");
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.begin();  /* setup the OTA server */
  // **********************************************************
  
  WiFi.hostname("ESP8266DebugTest");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  client.subscribe(debug);
  
  delay(200);
  startMillis = millis();
  Serial.begin(9600);
}

bool runTime(){
  if (currentMillis - startMillis >= interval){
    startMillis = currentMillis;  //IMPORTANT to save the start time 
    return true;
  }
  else
    return false;  
}

void loop(){
  currentMillis = millis();
  // Add support for OTA***************************************
  ArduinoOTA.handle();
  // **********************************************************
  if (!client.connected()) {
    reconnectToServer();  
  }
  if (runTime()) {
    Serial.println("10 sec passed");  
  }
  client.loop();
}

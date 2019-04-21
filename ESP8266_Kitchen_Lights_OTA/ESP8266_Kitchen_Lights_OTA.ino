

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

const char *control_light = "home/rooms/kitchen/control/control_light";  //topic that controles the light
const char *light_status = "home/rooms/kitchen/light/status"; // topic where we publish the actual light status
const char *alert_topic = "home/alert";

char lightStatus[5];

int ledPin = 4;
int onBoardLed = 2;

int newLedState = 0; // from mqtt topic
int currentLedState = 0;
int alert = 0;          // 0>OFF; 1> ON

WiFiClient espClient;
PubSubClient client(espClient);

void reconnectToServer() {
  while (!client.connected()) {
    if (client.connect("KitchenLightControl")) {
      client.subscribe(control_light);
      delay(200);
      client.subscribe(alert_topic);
      delay(200);
      publishLightStatus();
    } 
    else {
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic,"home/alert") == 0){
    char payloadMessage[length + 1];
    for (int i = 0; i < length; i++) {
      payloadMessage[i] = char(payload[i]);
    }
    alert = atoi(payloadMessage);
  }
  if (strcmp(topic,"home/rooms/kitchen/control/control_light") == 0){
    char payloadMessage[length + 1];
    for (int i = 0; i < length; i++) {
      payloadMessage[i] = char(payload[i]);
    }
    newLedState = atoi(payloadMessage);
  }
}

void publishLightStatus(){
  String buffer = String(currentLedState);
  buffer.toCharArray(lightStatus,(buffer.length() + 1));
  client.publish(light_status, lightStatus); //topic, value
  delay(200);  
}

void setup(){
  // Add support for OTA***************************************
  //Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("KitchenLightControl");
  ArduinoOTA.setPassword("bemobere");
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.begin();  /* setup the OTA server */
  // **********************************************************
  
  WiFi.hostname("KitchenLightControl");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  pinMode(ledPin, OUTPUT);
  pinMode(onBoardLed, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(onBoardLed, HIGH); //on esp12, if you want to turn off onboard led - set to high
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  client.subscribe(control_light);
  delay(200);
  client.subscribe(alert_topic);
  delay(200);
}

void blinkAlert() {
 digitalWrite(ledPin, HIGH);
 delay(500);
 digitalWrite(ledPin, LOW);
 delay(200);
 digitalWrite(ledPin, HIGH);
 delay(500);
 digitalWrite(ledPin, LOW);
 delay(1500);
}

void controlLight() {
    if ((newLedState == 1) && (currentLedState == 0)) {
      digitalWrite(ledPin, HIGH); 
      currentLedState = 1;
      publishLightStatus(); 
    }
    else if ((newLedState == 1) && (currentLedState == 1)) {
      return;
    }
    else if ((newLedState == 0) && (currentLedState == 0)) {
      return;
    }
    else if ((newLedState == 0) && (currentLedState == 1)) {
      digitalWrite(ledPin, LOW); 
      currentLedState = 0;
      publishLightStatus(); 
    }
}

void loop(){
  // Add support for OTA***************************************
  ArduinoOTA.handle();
  // **********************************************************
  if (!client.connected()) {
    reconnectToServer();  
  }
  
  if (alert == 1) {
    blinkAlert();  
  }

  controlLight();

  client.loop();
}

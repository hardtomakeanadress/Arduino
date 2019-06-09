

/*
this sketch was written on a SONOFF module
IT IS DIFFERENT FROM NODEMCU BOARD OR WEMOS D1MINI:
  I think this is  > GENERIC ESP8266 module with - 1Mb ; no SPIFFS
  - you CAN use OTA with these settings.                
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

const char *control_fan = "home/rooms/bedroom/control/control_fan";  //topic that controles the fan
const char *fan_status = "home/rooms/bedroom/fan/status"; // topic where we publish the actual fan status

char fanStatus[5];

//int ledPin = 4;
//int onBoardLed = 2;

int gpio_13_led = 13;
int gpio_12_relay = 12;

int newFanState = 0; // from mqtt topic
int currentFanState = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void reconnectToServer() {
  while (!client.connected()) {
    if (client.connect("BedroomFanControl")) {
      client.subscribe(control_fan);
      delay(200);
      publishFanStatus();
    } 
    else {
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic,"home/rooms/bedroom/control/control_fan") == 0){
    char payloadMessage[length + 1];
    for (int i = 0; i < length; i++) {
      payloadMessage[i] = char(payload[i]);
    }
    newFanState = atoi(payloadMessage);
  }
}

void publishFanStatus(){
  String buffer = String(currentFanState);
  buffer.toCharArray(fanStatus,(buffer.length() + 1));
  client.publish(fan_status, fanStatus); //topic, value
  delay(200);  
}

void setup(){
  // Add support for OTA***************************************
  //Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("BedroomFanControl");
  ArduinoOTA.setPassword("bemobere");
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.begin();  /* setup the OTA server */
  // **********************************************************
  
  WiFi.hostname("BedroomFanControl");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  pinMode(gpio_12_relay, OUTPUT);
  pinMode(gpio_13_led, OUTPUT);
  digitalWrite(gpio_12_relay, LOW);
  digitalWrite(gpio_13_led, HIGH); //on esp12, if you want to turn off onboard led - set to high
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  client.subscribe(control_fan);
  delay(200);
}

void controlFan() {
    if ((newFanState == 1) && (currentFanState == 0)) {
      digitalWrite(gpio_12_relay, HIGH); 
      currentFanState = 1;
      publishFanStatus(); 
    }
    else if ((newFanState == 1) && (currentFanState == 1)) {
      return;
    }
    else if ((newFanState == 0) && (currentFanState == 0)) {
      return;
    }
    else if ((newFanState == 0) && (currentFanState == 1)) {
      digitalWrite(gpio_12_relay, LOW); 
      currentFanState = 0;
      publishFanStatus(); 
    }
}

void loop(){
  // Add support for OTA***************************************
  ArduinoOTA.handle();
  // **********************************************************
  if (!client.connected()) {
    reconnectToServer();  
  }

  controlFan();

  client.loop();
}



//!!
//This is on a Generic ESP8266 module; when programing use 1MB memory, no SPIFF
//!!

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// Add support for OTA***************************************
#include <ArduinoOTA.h>
// **********************************************************

const char* ssid        = "";
const char* password    = "";
const char* mqtt_server = "192.168.0.107";
const int mqtt_port     = 1883;

const char *mqtt_topic    = "home/doors/front_door";
const byte sensorPin = 3; //esp01 would not boot proper is pin2 is connected to ground at boot time, meaning when the door is closed ...change pin? //we used pin3 but needs to change pin function in setup

byte sensorNewState;
byte sensorState;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi(){
  // Add support for OTA***************************************
  //Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("FrontdoorSensor");
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.begin();  /* setup the OTA server */
  // **********************************************************
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
  }
}
 
void setup() {
  WiFi.hostname("FrontdoorSensor");
  setup_wifi();
  
  pinMode(sensorPin, FUNCTION_3); //here we change the pin function to GPIO from Serial RX
  pinMode(sensorPin, INPUT);
  
  client.setServer(mqtt_server, mqtt_port);
}

void reconnect() {
  while (!client.connected()) {
    client.connect("FrontdoorSensor");
    delay(200);
  }
}

void loop() {
  // Add support for OTA***************************************
  ArduinoOTA.handle();
  // **********************************************************
  
  if (!client.connected()) {
    reconnect();
  }

  sensorNewState = digitalRead(sensorPin);  // 1=opened; 0= closed
  if (sensorNewState != sensorState) {
    if (sensorNewState == 1){
      client.publish(mqtt_topic, "Opened",true); // third parameter is to retain message
    }
    else {
      client.publish(mqtt_topic, "Closed", true); // third parameter is to retain message
    }
    sensorState =  sensorNewState;
  }
  
  client.loop();
}

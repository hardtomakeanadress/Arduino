

//!!
//This is on a Generic ESP8266 module; when programing use 1MB memory, no SPIFF
//!!

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// Add support for OTA***************************************
#include <ArduinoOTA.h>
// **********************************************************

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "192.168.0.107";

const int mqtt_port = 1883;

const char *mqtt_topic = "home/doors/front_door";
const char *alert_topic = "home/alert";

const byte sensorPin = 3; //esp01 would not boot proper is pin2 is connected to ground at boot time, meaning when the door is closed ...change pin? //we used pin3 but needs to change pin function in setup
const byte ALARM_PIN = 0;
byte sensorState;

long previousMillis = 0;
long send_interval  = 100; //check interval

int alert = 0;  // 0>OFF; 1> ON

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi(){
  // Add support for OTA***************************************
  //Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("FrontdoorSensor");
  ArduinoOTA.setPassword("bemobere");
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
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode (ALARM_PIN, OUTPUT );
  sensorState = digitalRead(sensorPin);
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("FrontdoorSensor")) {
      client.subscribe(alert_topic);
      delay(200);
      sendDoorState();
    } 
    else {
      delay(1000);
    }
  }
}

void sendDoorState() {
  byte sensorNewState = digitalRead(sensorPin);  

  if (sensorState != sensorNewState) {
    if (sensorNewState == 1){
      client.publish(mqtt_topic, "Opened",true); // third parameter is to retain message
    }
    else {
      client.publish(mqtt_topic, "Closed", true); // third parameter is to retain message
    }
  sensorState =  sensorNewState;
  }
}

void ringAlarm(){
  tone(ALARM_PIN,1000);
  delay(800);
  noTone(ALARM_PIN);
  delay(100);
  tone(ALARM_PIN,1000);
  delay(800);
  noTone(ALARM_PIN);
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic,"home/alert") == 0){
    char payloadMessage[length + 1];
    for (int i = 0; i < length; i++) {
      payloadMessage[i] = char(payload[i]);
    }
    alert = atoi(payloadMessage);
  }
}
 
void loop() {
  // Add support for OTA***************************************
  ArduinoOTA.handle();
  // **********************************************************
  unsigned long currentMillis = millis();
  
  if (!client.connected()) {
    reconnect();
  }

  if(currentMillis - previousMillis > send_interval) {
    previousMillis = currentMillis;
    sendDoorState();
  }
  
  if (alert == 1) {
    ringAlarm();  
  }
  
  client.loop();
}

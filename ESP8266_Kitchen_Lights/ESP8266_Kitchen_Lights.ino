//updated for esp12e

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "";
const char* password = "";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *control_light = "home/rooms/kitchen/control/control_light";  //topic that controles the light
const char *light_status = "home/rooms/kitchen/light/status"; // topic where we publish the actual light status

char lightStatus[5];

int ledPin = 4;
int onBoardLed = 2;
int received_value = 0; // from mqtt topic

WiFiClient espClient;
PubSubClient client(espClient);

void reconnectToServer() {
  while (!client.connected()) {
    if (client.connect("KitchenLightControl")) {
      client.subscribe(control_light);
      delay(200);
      publishLightStatus();
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
  int new_received_value = atoi(payloadMessage);
  controlLight(new_received_value);
  publishLightStatus();
}

void publishLightStatus(){
  String buffer = String(received_value);
  buffer.toCharArray(lightStatus,(buffer.length() + 1));
  client.publish(light_status, lightStatus); //topic, value
  delay(200);  
}

void setup(){
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
}

void blinkError() {
 digitalWrite(ledPin, HIGH);
 delay(500);
 digitalWrite(ledPin, LOW);
 delay(200);
 digitalWrite(ledPin, HIGH);
 delay(500);
 digitalWrite(ledPin, LOW);
 delay(1500);
}

void controlLight(int new_value) {

  if (new_value != received_value) {
    if (new_value == 1) {
      digitalWrite(ledPin, HIGH);  
    }
    else if (new_value == 0) {
      digitalWrite(ledPin, LOW);  
    }
  }
  received_value = new_value;
}

void loop(){
  if (!client.connected()) {
    reconnectToServer();  
  }
  delay(2000);
  if ((received_value != 0)&&(received_value != 1)) {
    blinkError();  
  }
  client.loop();
}

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

//USER CONFIGURED SECTION START//
const char* ssid = "warz";
const char* password = "parola!derezerva";
const char* mqtt_server = "192.168.0.107";
const int mqtt_port = 1883;
const char *mqtt_user = "";
const char *mqtt_pass = "";
const char *mqtt_client_name = "front_door"; // Client connections cant have the same connection name
const char *mqtt_topic = "door";
//USER CONFIGURED SECTION END//

const byte interruptPin = 13;  ??? maybe not 13 

WiFiClient espClient;
PubSubClient client(espClient); 

void setup_wifi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
  }
}

void handleInterrupt() {
  //aici punem codul care se executa la interrupt
  // trimitem mesajul mqtt
}

void setup(){
  setup_wifi();
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, CHANGE); //change to CHANGE
  client.setServer(mqtt_server, mqtt_port);
}

void loop(){
 // in loop nu cred ca se face ceva, doar pe interrupt
}



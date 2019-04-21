#include <PubSubClient.h>
#include <ESP8266WiFi.h>

//USER CONFIGURED SECTION START//
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "192.168.0.107";
const int mqtt_port = 1883;
const char *mqtt_user = "";
const char *mqtt_pass = "";
const char *mqtt_client_name = "front_door"; // Client connections cant have the same connection name
const char *mqtt_topic = "doors/front_door";
//USER CONFIGURED SECTION END//

const byte sensorPin = 3; //esp01 would not boot proper is pin2 is connected to ground at boot time, meaning when the door is closed ...change pin? //we used pin3 but needs to change pin function in setup
byte sensorState;

WiFiClient espClient;
PubSubClient client(espClient);

//take the wifi setup in a separate method
void setup_wifi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
  }
}
 
void setup() {
  WiFi.hostname("FrontDoorSensor");
  pinMode(sensorPin, FUNCTION_3); //here we change the pin function to GPIO from Serial RX
  setup_wifi();
  pinMode(sensorPin, INPUT_PULLUP);
  sensorState = digitalRead(sensorPin);
  client.setServer(mqtt_server, mqtt_port);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect(mqtt_client_name, mqtt_user, mqtt_pass)) {
    } 
    else {
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}
 
void loop() {
  byte sensorNewState = digitalRead(sensorPin);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if (sensorState != sensorNewState) {
    if (sensorNewState == 1){
      client.publish(mqtt_topic, "opened",true); // third parameter is to retain message
    }
    else {
      client.publish(mqtt_topic, "closed", true); // third parameter is to retain message
    }
  sensorState =  sensorNewState;
  }
  delay(500);
}

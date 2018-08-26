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
const char *mqtt_topic = "doors/front_door";
//USER CONFIGURED SECTION END//

const byte sensorPin = 13;
byte sensorState;

WiFiClient espClient;
PubSubClient client(espClient); 

//taking the wifi setup in a separate method
void setup_wifi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
  }
}
 
void setup() {
  WiFi.hostname("FrontDoorSensor");
  setup_wifi();
  pinMode(sensorPin, INPUT_PULLUP);
  sensorState = digitalRead(sensorPin);
  client.setServer(mqtt_server, mqtt_port);
  Serial.begin(115200);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect(mqtt_client_name, mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
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
      client.publish(mqtt_topic, "opened");
      Serial.println("Opened");
    }
    else {
      client.publish(mqtt_topic, "closed");
      Serial.println("Closed");
    }
  sensorState =  sensorNewState;
  }
  delay(500);
}

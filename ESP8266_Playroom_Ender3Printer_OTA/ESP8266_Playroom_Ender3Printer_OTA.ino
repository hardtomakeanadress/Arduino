

/*
this sketch was written on a SONOFF module
IT IS DIFFERENT FROM NODEMCU BOARD OR WEMOS D1 MINI:
  I think this is  > GENERIC ESP8266 module with - 1Mb ; no SPIFFS
  - you CAN use OTA with these settings.                
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Add support for OTA***************************************
#include <ArduinoOTA.h>
// **********************************************************

const char* ssid     = "warz";
const char* password = "parola!derezerva";

const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;

const char *control_Ender3DPrinter = "home/rooms/playroom/control/Ender3DPrinter";  //topic that controles the 3D printer
const char *Ender3DPrinter_status = "home/rooms/playroom/Ender3DPrinter/status"; // topic where we publish the actual 3d printer status

char Ender3DPrinterStatus[5];

//int ledPin = 4;
//int onBoardLed = 2;

int gpio_13_led = 13;
int gpio_12_relay = 12;

int newEnder3DPrinterState = 0; // from mqtt topic
int currentEnder3DPrinterState = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void reconnectToServer() {
  while (!client.connected()) {
    if (client.connect("PlayroomEnder3DPrinterControl")) {
      client.subscribe(control_Ender3DPrinter);
      delay(200);
      publishEnder3DPrinterStatus();
    } 
    else {
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic,"home/rooms/playroom/control/control_Ender3DPrinter") == 0){
    char payloadMessage[length + 1];
    for (int i = 0; i < length; i++) {
      payloadMessage[i] = char(payload[i]);
    }
    newEnder3DPrinterState = atoi(payloadMessage);
  }
}

void publishEnder3DPrinterStatus(){
  String buffer = String(currentEnder3DPrinterState);
  buffer.toCharArray(Ender3DPrinterStatus,(buffer.length() + 1));
  client.publish(Ender3DPrinter_status, Ender3DPrinterStatus); //topic, value
  delay(200);  
}

void setup(){
  // Add support for OTA***************************************
  //Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("PlayroomEnder3DPrinterControl");
  ArduinoOTA.setPassword("bemobere");
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.begin();  /* setup the OTA server */
  // **********************************************************
  
  WiFi.hostname("PlayroomEnder3DPrinterControl");
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
  client.subscribe(control_Ender3DPrinter);
  delay(200);
}

void controlEnder3DPrinter() {
    if ((newEnder3DPrinterState == 1) && (currentEnder3DPrinterState == 0)) {
      digitalWrite(gpio_12_relay, HIGH);
      digitalWrite(gpio_13_led, LOW);
      currentEnder3DPrinterState = 1;
      publishEnder3DPrinterStatus(); 
    }
    else if ((newEnder3DPrinterState == 1) && (currentEnder3DPrinterState == 1)) {
      return;
    }
    else if ((newEnder3DPrinterState == 0) && (currentEnder3DPrinterState == 0)) {
      return;
    }
    else if ((newEnder3DPrinterState == 0) && (currentEnder3DPrinterState == 1)) {
      digitalWrite(gpio_12_relay, LOW);
      digitalWrite(gpio_13_led, HIGH);
      currentEnder3DPrinterState = 0;
      publishEnder3DPrinterStatus(); 
    }
}

void loop(){
  // Add support for OTA***************************************
  ArduinoOTA.handle();
  // **********************************************************
  if (!client.connected()) {
    reconnectToServer();  
  }

  controlEnder3DPrinter();

  client.loop();
}

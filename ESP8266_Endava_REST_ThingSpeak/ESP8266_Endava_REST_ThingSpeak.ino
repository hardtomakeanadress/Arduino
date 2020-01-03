#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 2

String apiKey        = "";
const char* ssid     = "";
const char* password = "";
const char* server   = "";

unsigned long startMillis = millis();
unsigned long currentMillis;
unsigned long interval = 30000;  //30 sec

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  WiFi.begin(ssid, password);
  dht.begin();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

bool runTime() {
  currentMillis = millis();
  if (currentMillis - startMillis >= interval){
    startMillis = currentMillis;  //IMPORTANT to save the start time 
    return true;
  }
  else
    return false;
}

void loop() {
  
  if (runTime()&&(client.connect(server,80))) {  // "184.106.153.149" or api.thingspeak.com
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    delay(2000);
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(t);
    postStr +="&field2=";
    postStr += String(h);
  
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
client.stop();
}

#include <ESP8266WiFi.h>

//this is the "camera mica" channel

String apiKey        = "7QOUBJXWOOUMQGN4";
const char* ssid     = "warz";
const char* password = "paroladerezerva";
const char* server   = "api.thingspeak.com";

uint32_t getVcc;

//#define ADCPIN A0

ADC_MODE(ADC_VCC);

WiFiClient client;

void setup() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {

//  float v = analogRead(ADCPIN); // * 2.99 / 1024.0;
  getVcc = ESP.getVcc();
  
//  if (isnan(h) || isnan(t)) {
//    delay(1000);
//    return;
//  }
  
if (client.connect(server,80)) {  // "184.106.153.149" or api.thingspeak.com
  String postStr = apiKey;
  postStr +="&field1=";
  postStr += String(getVcc);
  postStr += "\r\n\r\n";

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

// thingspeak needs minimum 15 sec delay between updates
//waiting for 10 minutes between readings
//delay(30000);
ESP.deepSleep(600000000,WAKE_RF_DEFAULT);
}

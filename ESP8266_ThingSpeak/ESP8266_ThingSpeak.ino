#include <DHT.h>
#include <ESP8266WiFi.h>
//this is the camera mica channel
// replace with your channel’s thingspeak API key,
String apiKey = "7QOUBJXWOOUMQGN4";
const char* ssid = "warz";
const char* password = "paroladerezerva";

const char* server = "api.thingspeak.com";
#define DHTPIN 2 // what pin we’re connected to
int LEDPIN  = 0;

DHT dht(DHTPIN, DHT11,15);
WiFiClient client;

void setup() {
  
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  delay(10);
  dht.begin();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    delay(1000);
    return;
  }
  if (t > 24) {
    digitalWrite(LEDPIN, HIGH);
  }
  else {
    digitalWrite(LEDPIN, LOW);
  }
Serial.print(t);
if (client.connect(server,80)) {  // "184.106.153.149" or api.thingspeak.com
  String postStr = apiKey;
  postStr +="&field1=";
  postStr += String(t);
  postStr +="&field2=";
  postStr += String(h);
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
delay(60000);
}

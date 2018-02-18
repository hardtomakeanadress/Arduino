#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN 4 // what pin we’re connected to; on WemosD1 Mini pin is called D4

char ssid[] = "warz";
char pass[] = "paroladerezerva";

DHT dht(DHTPIN, DHT11, 15);
WiFiClient client;

void setup() {
  
  WiFi.hostname("OfficeSensor");
  WiFi.begin(ssid,pass);
  
  Serial.begin(9600);
  
  dht.begin();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Not connected yet");
  }
}

void loop() {
  float   h = dht.readHumidity();
  float   t = dht.readTemperature();
  float   v = analogRead(A0) * (4.1 / 1023);
  String baseUrl = "http://192.168.0.120:3003/get?room=office";
  
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin(baseUrl + "&humidity=" + h + "&temperature=" + t + "&voltage=" + v);   //Specify request destination
 
   int httpCode = http.GET();   //Send the request
 
   http.end();  //Close connection
  }
  ESP.deepSleep(600000000,WAKE_RF_DEFAULT); 
}

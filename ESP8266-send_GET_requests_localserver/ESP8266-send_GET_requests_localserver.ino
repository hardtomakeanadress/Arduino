#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//!!!!!!
#define DHTPIN D4 // what pin we’re connected to; only on WemosD1 Mini pin is called D4; on NodeMcu is 4
//!!!!!!

char ssid[] = "";
char pass[] = "";

DHT dht(DHTPIN, DHT22, 15);
WiFiClient client;

void setup() {
  
  WiFi.hostname("BalconySensor");
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
  String baseUrl = "http://192.168.0.107:3003/get?room=balcony";
  
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin(baseUrl + "&humidity=" + h + "&temperature=" + t + "&voltage=" + v);   //Specify request destination
 
   int httpCode = http.GET();   //Send the request
 
   http.end();  //Close connection
  }
  ESP.deepSleep(3600000000,WAKE_RF_DEFAULT); //sleep 60min
}

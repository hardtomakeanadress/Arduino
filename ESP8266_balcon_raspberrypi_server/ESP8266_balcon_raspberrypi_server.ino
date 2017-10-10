#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define DHTPIN 13 // what pin we’re connected to
#define ADCPIN A0

DHT dht(DHTPIN, DHT11, 15);
WiFiClient client;

void setup() {
  dht.begin();
  WiFi.hostname("balconySensor");
  WiFi.begin("warz", "paroladerezerva");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float v = analogRead(ADCPIN) * 4.33 / 1024.0;

// could interfere with the wifi / sleep /conect cycle, more testing need to be done
// for now, I wil validate the data from the backend.

//  if (isnan(h) || isnan(t)) {
//    delay(1000);
//    return;
//  }

  if (WiFi.status()== WL_CONNECTED) { 
    
    HTTPClient http;    //Declare object of class HTTPClient
    http.begin("http://192.168.0.120:80/post");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header

 // String postString format should be = "{\"temperature\": 25,\"humidity\": 65,\"voltage\": 3.17}";
    String postString = "{\"temperature\":";
           postString += t;
           postString += ", \"humidity\":";
           postString += h;
           postString += ", \"voltage\":";
           postString += v;
           postString += "}";
    
    int httpCode = http.POST(postString);   //Send the request
    
    String responseCode = http.getString();       //Get the response payload
    http.end();  //Close connection
  }
  ESP.deepSleep(600000000,WAKE_RF_DEFAULT); 
}

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define DHTPIN D4 // what pin we’re connected to
#define ADCPIN A0

char ssid[] = "";
char pass[] = "";

DHT dht(DHTPIN, DHT22, 15);
WiFiClient client;

void setup() {
//  Serial.begin(115200);
  WiFi.hostname("BalconySensor");
  WiFi.begin(ssid,pass);
  dht.begin();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
//    Serial.println("Not connected yet");
  }
}

void loop() {
  float   h = dht.readHumidity();
  float   t = dht.readTemperature();
  float   v = analogRead(ADCPIN) * (4.45 / 1023);
  String id = "balcony";
//  Serial.println(h);
//  Serial.println(t);
//  Serial.println(v);
  
// is interfering with the sketch; we better validate the data on the server side  
//  if (isnan(h) || isnan(t)) {
//    delay(1000);
//    Serial.println("Error reading DHT sensor values!");
//    return;
//  }

  if (WiFi.status()== WL_CONNECTED) { 
//    Serial.println("Wifi connected !");
    HTTPClient http;    //Declare object of class HTTPClient
    http.begin("http://192.168.0.122:8888/post");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header

 // String postString format should be = "{\"temperature\": 25,\"humidity\": 65,\"voltage\": 3.17}";
    String postString  = "{\"temperature\":";
           postString += t;
           postString += ", \"humidity\":";
           postString += h;
           postString += ", \"voltage\":";
           postString += v;
           postString += ", \"id\":";
           postString += "\"";
           postString += id;
           postString += "\"";
           postString += "}";
//    Serial.println(postString);
    int httpCode = http.POST(postString);   //Send the request
    
    //String responseCode = http.getString();       //Get the response payload
    http.end();  //Close connection
  }
//  delay(20000);
  ESP.deepSleep(600000000,WAKE_RF_DEFAULT); 
}

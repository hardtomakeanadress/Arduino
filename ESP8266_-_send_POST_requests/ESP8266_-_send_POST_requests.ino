#include <ESP8266WiFi.h>
#include <DHT.h>
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>
 
/* Set these to your desired credentials. */
const char *ssid = "warz";  //ENTER YOUR WIFI SETTINGS
const char *password = "parola!derezerva";

#define DHTPIN D4 // what pin we’re connected to
#define ADCPIN A0
 
//Web/Server address to read/write from 
const char *host = "192.168.0.122";   // website or IP address of server

DHT dht(DHTPIN, DHT22,15);
WiFiClient client;
 
//=======================================================================
//                    Power on setup
//=======================================================================
 
void setup() {
  WiFi.begin(ssid, password);
  dht.begin();
  Serial.begin(9600);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("not connected to wifi...");
  }
}
 
//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  Serial.println("Here we go!");
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float v = analogRead(ADCPIN) * (4.1 / 1023);
  
  HTTPClient http;    //Declare object of class HTTPClient
 
  String postData;
 
  //Post Data
  postData = "temp=50";
//  postData = "temp= " + String(t) + " & hum= " + String(h) + " & volt= "  + String(v);
  Serial.println(postData);
  http.begin(host);              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
 
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection
  delay(30000);
//ESP.deepSleep(60000000,WAKE_RF_DEFAULT); //sleep 1 minute
}
//=======================================================================

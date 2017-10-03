#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

float h,t,v; 

void setup() {
  WiFi.hostname("postrequester");
  Serial.begin(115200);                                 
 
  while (WiFi.status() != WL_CONNECTED) { 
 
    delay(1000);
    Serial.println("Waiting for connection");
 
  }
 
}
 
void loop() {
 
 if (WiFi.status()== WL_CONNECTED) {
   Serial.println("Network is connected");
   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin("http://192.168.0.120:80/post");      //Specify request destination
   http.addHeader("Content-Type", "application/json");  //Specify content-type header
   String postString = "{\"temperature\":";
           postString += t;
           postString += ", \"humidity\":";
           postString += h;
           postString += ", \"voltage\":";
           postString += v;
           postString += "}"; 
   int httpCode = http.POST(postString);   //Send the request
   String payload = http.getString();                  //Get the response payload
   Serial.println(postString);
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
 
 }
 else{
 
    Serial.println("Error in WiFi connection");   
 
 }
 
  delay(30000);  //Send a request every 30 seconds
 
}

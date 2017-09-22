//
//#include <ESP8266WiFi.h>
//
//const char* ssid     = "warz";
//const char* password = "paroladerezerva";
//const char* server   = "192.168.0.103";
//
//uint32_t getVcc;
//ADC_MODE(ADC_VCC);
//
////#define ADCPIN A0
//
//WiFiClient client;
//
//void setup() {
//  Serial.begin(9600);
//  WiFi.hostname("postrequester");
//  WiFi.begin(ssid, password);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//  }
//}
//
//void loop() {
//  getVcc = ESP.getVcc();
//  
//    client.print("POST /update HTTP/1.1\n");
//    client.print(String(getVcc));
//    Serial.print(String(getVcc));
//    client.stop();
//    delay(60000);
//  
//}
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
 
void setup() {
  WiFi.hostname("postrequester");
  Serial.begin(115200);                                  //Serial connection
  WiFi.begin("warz", "paroladerezerva");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
 
}
 
void loop() {
 
 if (WiFi.status()== WL_CONNECTED) {   //Check WiFi connection status
 
   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin("http://192.168.0.103:80/post");      //Specify request destination
   http.addHeader("Content-Type", "application/json");  //Specify content-type header
   char PostData[] = "{\"temperature\": \"Fred\", \"age\": 31}"; 
   int httpCode = http.POST(PostData);   //Send the request
   String payload = http.getString();                  //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
 
 }
 else{
 
    Serial.println("Error in WiFi connection");   
 
 }
 
  delay(30000);  //Send a request every 30 seconds
 
}

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//!!!!!!
#define sensor 2 // what pin we’re connected to; only on WemosD1 Mini pin is called D4; on NodeMcu is 4
//!!!!!!

char ssid[] = "warz";
char pass[] = "parola!derezerva";
String baseUrl = "http://192.168.0.201:4003/state?room=ITRoomSensor";
int sensorDefaultState;

WiFiClient client;

void setup() {
  WiFi.hostname("ITRoomSensor");
  WiFi.begin(ssid,pass);

  //getting the default state values for vcc and door
  int sensorDefaultState = digitalRead(sensor);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  int sensorStateNow = digitalRead(sensor);

  //check if the door has opened or closed
  


  if (sensorStateNow != sensorDefaultState) {
    sendAlert(sensorStateNow);
    //update the default values;
    sensorDefaultState = sensorStateNow;
  }
}
  
void sendAlert( int sensorStateNow) {
  //set the correct door status 
  String doorState;
  if (sensorStateNow == HIGH) {
    doorState = "Opened";  
  }
  else {
    doorState = "Closed";  
  }

  //wifi & HTTP stuff
  if (WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    HTTPClient http;    //Declare object of class HTTPClient
    http.begin(baseUrl + "&doorState=" + doorState);   //Specify request destination
    int httpCode = http.GET();   //Send the request
    http.end();  //Close connection
  }
  delay(10000); //10 sec delay  
}

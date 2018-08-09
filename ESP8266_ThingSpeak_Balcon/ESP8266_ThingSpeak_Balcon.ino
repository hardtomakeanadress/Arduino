#include <DHT.h>
#include <ESP8266WiFi.h>

//this is the "Weatherstation" channel; replace with your channel’s thingspeak API WRITE key

// Wi-Fi Settings
const char* ssid = ""; // your wireless network name (SSID)
const char* password = ""; // your Wi-Fi network password

//ThingSpeak settings
const char* server = "192.168.0.122";

#define DHTPIN D4 // what pin we’re connected to
#define ADCPIN A0

DHT dht(DHTPIN, DHT22,15);
WiFiClient client;

void setup() {
  WiFi.begin(ssid, password);
  dht.begin();
  Serial.begin(9600);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("not connected to wifi...");
  }
}

void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float v = analogRead(ADCPIN) * (4.1 / 1023);
  
  Serial.print(h);
  Serial.print(" | ");
  Serial.print(t);
  Serial.print(" | ");
  Serial.println(v);
  
  if (client.connect(server, 80)) {  
    
    String dataString = "temp=" + String(t) + "&hum=" + String(h) + "&volt=" + String(v);
  
    client.print(dataString);
    Serial.println(dataString);
  }
  client.stop();
  Serial.println("Data was sent, going to sleep(delay)");
  
// thingspeak needs minimum 15 sec delay between updates
//waiting for 1 minutes between readings
delay(60000);
//  ESP.deepSleep(60000000,WAKE_RF_DEFAULT); 
}

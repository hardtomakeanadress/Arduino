
#include <ESP8266WiFi.h>

const char* ssid     = "warz";
const char* password = "paroladerezerva";
const char* server   = "192.168.0.120";

uint32_t getVcc;
ADC_MODE(ADC_VCC);

//#define ADCPIN A0

WiFiClient client;

void setup() {
  WiFi.hostname("postrequester");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  getVcc = ESP.getVcc();
  if (client.connect(server,80)) {
    client.print(String(getVcc));
    client.stop();
    delay(60000);
  }
}

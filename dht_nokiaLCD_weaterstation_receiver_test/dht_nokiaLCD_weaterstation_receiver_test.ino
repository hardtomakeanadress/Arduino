
#include <VirtualWire.h>

const int receive_pin = 11;

char temperatureChar[10];
char humidityChar[10];
char lightChar[10];
char voltChar[10];

struct package
{
  float humidity ;
  float temperature ;
  float light;
  float volt;
};

typedef struct package Package;
Package data;

void setup()
{
    Serial.begin(9600);	// Debugging only

    // Initialise the IO and ISR
    vw_set_rx_pin(receive_pin);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(500);	 // Bits per sec

    vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
    uint8_t buf[sizeof(data)];
    uint8_t buflen = sizeof(data);

if (vw_have_message())  // Is there a packet for us? 
  {
    vw_get_message(buf, &buflen);
    memcpy(&data,&buf,buflen);

    String temperatureString = String(data.temperature,1);
    temperatureString.toCharArray(temperatureChar,10);

    String humidityString = String(data.humidity,1);
    humidityString.toCharArray(humidityChar,10);

    String lightString = String(data.light,1);
    lightString.toCharArray(lightChar,10);

    String voltString = String(data.volt,1);
    voltString.toCharArray(voltChar,10);

    Serial.print("Temperature: ");
    Serial.print(data.temperature);
    Serial.print("\n");
    Serial.print("Humidity: ");
    Serial.print(data.humidity);
    Serial.print("\n");
    Serial.print("Light: ");
    Serial.print(data.light);
    Serial.print("\n");
    Serial.print("Volts: ");
    Serial.print(data.volt);
    Serial.print("\n");
  }
}

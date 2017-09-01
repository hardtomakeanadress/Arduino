
#include <VirtualWire.h>

const int receive_pin = 9;

char temperatureChar[10];
char pressureChar[10];
char altitudeChar[10];
char voltageChar[10];

struct package
  {
    float temperature;
    float pressure;
    float altitude;
    float voltage;
  };

typedef struct package Package;
Package data;

void setup()
{
    Serial.begin(9600);	// Debugging only

    // Initialise the IO and ISR
    vw_set_rx_pin(9);
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

    String pressureString = String(data.pressure,1);
    pressureString.toCharArray(pressureChar,10);

    String altitudeString = String(data.altitude,1);
    altitudeString.toCharArray(altitudeChar,10);

    String voltageString = String(data.voltage,1);
    voltageString.toCharArray(voltageChar,10);

    Serial.print("Temperatura: ");
    Serial.print(data.temperature);
    Serial.print("\n");
    Serial.print("Presiune: ");
    Serial.print(data.pressure);
    Serial.print("\n");
    Serial.print("Altitudine: ");
    Serial.print(data.altitude);
    Serial.print("\n");
    Serial.print("Voltaj baterie: ");
    Serial.print(data.voltage);
    Serial.print("\n");
  }
}

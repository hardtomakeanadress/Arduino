#include <Wire.h>
#include <VirtualWire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include "LowPower.h"

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10

//Adafruit_BMP280 bme; // I2C
//Adafruit_BMP280 bme(BMP_CS); // hardware SPI
Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

const int transmitterPower = 2;

struct package
  {
    float temperature;
    float pressure;
    float altitude;
    float batteryVoltage;
  };

typedef struct package Package;
Package data;
  
void setup() {
  Serial.begin(9600);

   
  vw_set_tx_pin(9);
  vw_set_ptt_inverted(true);
  vw_setup(500); // Bits per sec
  Serial.println(F("BMP280 test"));
         
  if (!bme.begin()) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}
  
void loop() {
  readSensor();
  readVcc();
  pinMode(transmitterPower, OUTPUT);
  digitalWrite(transmitterPower, HIGH);
  delay(10);
  vw_send((uint8_t *)&data, sizeof(data));
  vw_wait_tx();
  digitalWrite(transmitterPower, LOW);
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
                SPI_OFF, USART0_OFF, TWI_OFF);

  }

void readSensor() {
  data.temperature = bme.readTemperature();
  data.pressure = bme.readPressure() / 100;
  data.altitude = bme.readAltitude(1013.25);
  }

void readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  data.batteryVoltage = float(result) / 1000;
}

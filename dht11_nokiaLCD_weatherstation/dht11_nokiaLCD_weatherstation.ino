#include "U8glib.h"
#include "DHT.h"
#include "LowPower.h"
#include <VirtualWire.h>
 
#define DHTPIN 2
#define LIGHTPIN A0
#define BATERRYPIN A1
#define DHTTYPE DHT11

const int transmit_pin = 12;

float h,t,light,volt;
//#define backlight_pin 11

struct package
{
  float humidity ;
  float temperature ;
  float light;
  float volt;
};

typedef struct package Package;
Package data;
 
DHT dht(DHTPIN, DHTTYPE);
 
U8GLIB_PCD8544 u8g(3, 4, 7, 5, 6);  // CLK=8, DIN=4, CE=7, DC=5, RST=6

void draw(void) {
  int cursor_position = 50;

  if (light < 10) {
    cursor_position = 55;
  }
  
  u8g.setFont(u8g_font_profont11); 
  
  u8g.drawStr(0, 10, "Temp:");  
  u8g.drawStr(0, 21, "Hum:");
  u8g.drawStr(0, 32, "Light:");
  u8g.drawStr(0, 43, "Volt:");
  
  u8g.setPrintPos(50, 10 ); 
  u8g.print(t, 0);  
  u8g.drawStr(70, 10, "C");
  
  u8g.setPrintPos(50, 21);
  u8g.print(h, 0);  
  u8g.drawStr(70, 21, "%");
  
  u8g.setPrintPos(cursor_position, 32);
  u8g.print(light, 0);
  u8g.drawStr(70, 32, "%");

  u8g.setPrintPos(43, 43);
  u8g.print(volt, 1);  
  u8g.drawStr(70, 43, "V");
}
 
void setup(void) {
  
  // Initialise the IO and ISR
  vw_set_tx_pin(transmit_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(500);       // Bits per sec
  
  dht.begin();
  pinMode(LIGHTPIN, INPUT);
  pinMode(BATERRYPIN, INPUT);
}

void run_asleep() {
  u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_profont11);
    u8g.drawStr(0, 20, "Zzz...sleeping");
  } 
  while( u8g.nextPage() );
}
  
void run_awake() {
  delay(1000);
  h = dht.readHumidity(); 
  t = dht.readTemperature();

  //change analog reference to 1.1V internal
  analogReference(INTERNAL);                

  // make a "dry read" that we are not using
  analogRead(BATERRYPIN);    

  //wait for voltage to stabilize                        
  delay(100);    

  //start using the reading value
  volt = (analogRead(BATERRYPIN) * 2.65 ) / 1023.0; 

  //get the struct data rady for transmission
  data.humidity = h;
  data.temperature = t;
  data.light = light;
  data.volt= volt;
  
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  vw_send((uint8_t *)&data, sizeof(data)); //send the data
  vw_wait_tx(); // Wait until the whole message is gone
  
//  delay(500);  
}
 
void loop(void) {
  //change back to default analog referance
  analogReference(DEFAULT); 

  //make a "dry" read that we are not using
  analogRead(LIGHTPIN); 

  //wait for voltage to stabilize
  delay(100); 
  
  light = map(analogRead(LIGHTPIN), 0, 1023, 99, 0); 
  
  if (light > 2) {
    run_awake() ;
  }
  else{
    run_asleep();
  }

  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}

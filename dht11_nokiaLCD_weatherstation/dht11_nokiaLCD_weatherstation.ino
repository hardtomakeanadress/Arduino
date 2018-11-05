
#include "U8glib.h"
#include "DHT.h"
#include "LowPower.h"
 
#define DHTPIN 2
#define DHTTYPE DHT11

float h,t;

//#define backlight_pin 11
 
DHT dht(DHTPIN, DHTTYPE);
 
U8GLIB_PCD8544 u8g(3, 4, 7, 5, 6);  // CLK=8, DIN=4, CE=7, DC=5, RST=6

void draw(void) {
  u8g.setFont(u8g_font_profont11); 
  u8g.drawStr(0, 15, "Temperat: ");  
  u8g.drawStr(0, 35, "Humidity: ");
  u8g.setPrintPos(55, 15 ); 
  u8g.print(t, 0);  
  u8g.drawStr(70, 15, "C ");
  u8g.setPrintPos(55, 35);
  u8g.print(h, 0);  
  u8g.drawStr(70, 35, "% ");
}
 
void setup(void) {
  dht.begin();
//  analogWrite(backlight_pin, 50);  
}
 
void loop(void) {
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  delay(1000);
  h = dht.readHumidity(); 
  t = dht.readTemperature();
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
//  delay(500);  
}

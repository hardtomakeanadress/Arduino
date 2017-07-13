#include <PCD8544.h>
#include <notele_de_stoc.h>

int note1 = NOTE_A6;
int alarma = 8;
int led = 9;

static const byte sensorPin = 0;
static const byte ledPin = 13;
// The dimensions of the LCD (in pixels)...
static const byte LCD_WIDTH = 84;
static const byte LCD_HEIGHT = 48;
// The number of lines for the temperature chart...
static const byte CHART_HEIGHT = 5;
// A custom "degrees" symbol...
static const byte DEGREES_CHAR = 1;
static const byte degrees_glyph[] = { 0x00, 0x07, 0x05, 0x07, 0x00 };
// A bitmap graphic (10x2) of a thermometer...
static const byte THERMO_WIDTH = 10;
static const byte THERMO_HEIGHT = 2;
static const byte thermometer[] = { 0x00, 0x00, 0x48, 0xfe, 0x01, 0xfe, 0x00, 0x02, 0x05, 0x02,
0x00, 0x00, 0x62, 0xff, 0xfe, 0xff, 0x60, 0x00, 0x00, 0x00};
static PCD8544 lcd;
void setup() {
lcd.begin(LCD_WIDTH, LCD_HEIGHT);
// Register the custom symbol...
lcd.createChar(DEGREES_CHAR, degrees_glyph);
pinMode(ledPin, OUTPUT);
// The internal 1.1V reference provides for better
// resolution from the LM35, and is also more stable
// when powered from either a battery or USB...
pinMode(9, OUTPUT);
 digitalWrite(9, LOW);
}
void loop() {
// Start beyond the edge of the screen...
static byte xChart = LCD_WIDTH;
digitalWrite(ledPin, HIGH);
// Read the temperature (in celsius)...
float temp = (analogRead(sensorPin) * 0.4887585532746823);
// Print the temperature (using the custom "degrees" symbol)...
lcd.setCursor(0, 0);
lcd.print("Temp: ");
lcd.print(temp, 1);
lcd.print(" \001C ");
// Draw the thermometer bitmap at the bottom left corner...
lcd.setCursor(0, LCD_HEIGHT/8 - THERMO_HEIGHT);
lcd.drawBitmap(thermometer, THERMO_WIDTH, THERMO_HEIGHT);
// Wrap the chart's current position...
if (xChart >= LCD_WIDTH) {
xChart = THERMO_WIDTH + 2;
}
// Update the temperature chart...
lcd.setCursor(xChart, 1);
lcd.drawColumn(CHART_HEIGHT, map(temp, 0, 45, 0, CHART_HEIGHT*8)); // ...clipped to the 0-45C range.
lcd.drawColumn(CHART_HEIGHT, 0); // ...with a clear marker to see the current chart position.
xChart++;
digitalWrite(ledPin, LOW);
delay(3000); 
{
  // Conditia de Sunet de ALARMA
if (temp >27) {
// sound tone
tone(8, note1);
}
else {
//opreste alarma
noTone(8);
}
// pana aici

//control releu incalzire
if (temp >26) {
digitalWrite(led,LOW);
} 
else 
{
digitalWrite(led,HIGH); //pana aici
}
}
}

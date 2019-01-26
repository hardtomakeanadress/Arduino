/* define de proper pins for wemos d1 mini in PCD8544.h*/

/* remember to change the pins !!! if you need original pins restore backup*/

#include <PCD8544.h>


static PCD8544 lcd;


void setup() {
  // PCD8544-compatible displays may have a different resolution...
  lcd.begin(84, 48);

  // Add the smiley to position "0" of the ASCII table...
//  lcd.createChar(0, glyph);
}


void loop() {
//  // Just to show the program is alive...
//  static int counter = 0;
int level = map(analogRead(A0), 0, 1023, 0, 100);
//  // Write a piece of text on the first line...
//  lcd.setCursor(0, 0);
//  lcd.print("Hello, World!");
//
//  // Write the counter on the second line...
  lcd.setCursor(0, 1);
  lcd.print(level, DEC);
//  lcd.write(' ');
//  lcd.write(0);  // write the smiley
//
//  // Use a potentiometer to set the LCD contrast...
  
//  // lcd.setContrast(level);
//
//  delay(200);
//  counter++;
}


/* EOF - HelloWorld.ino */

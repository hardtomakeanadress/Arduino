#include "pitches.h"

int lightPin = A5;  //define a pin for Photo resistor

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

int valoareCitita;

void setup()
{
    Serial.begin(9600);  //Begin serial communcation

}

void loop()
{
    valoareCitita = analogRead(lightPin);

    if (valoareCitita < 700) {
      playSound();
      }
    Serial.println(valoareCitita); //Write the value of the photoresistor to the serial monitor.
    
   delay(100); //short delay for faster response to light.
}

void playSound() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}

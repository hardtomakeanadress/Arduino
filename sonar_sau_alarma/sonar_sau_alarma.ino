#include <MelodyUtils.h>
#include <pitches.h>

void setup()
{
  
}

void loop()
{
  
  
 MelodyUtils mel(8);
 mel.Glis(NOTE_DS8, NOTE_D8, 0.9);
  
  delay(200);
  mel.Glis(NOTE_DS8, NOTE_D8, 0.9);
  delay(2500);
  
  
  
}

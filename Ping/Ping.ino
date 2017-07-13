#define trigpin 3
#define echopin 2

void setup(){
  Serial.begin (9600);
  pinMode(trigpin, OUTPUT);  
  pinMode(echopin, INPUT);
}

void loop(){
  int duration, distance;
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigpin, LOW);
  duration = pulseIn (echopin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print(distance);
  Serial.println();
  delay(1000);
}



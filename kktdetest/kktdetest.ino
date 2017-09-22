    

void setup() {
  Serial.begin(9600);

    
}

void loop() {

  float h = 50.0;
  float t = 23.5;
  float v = 4.15;


String postString = "{\"temp\": 25,\"hum\": 65,\"volt\": 3.17}";
    
    String postString2 = "{\"temperature\":";
           postString2 += t;
           postString2 += ", \"humidity\":";
           postString2 += h;
           postString2 += ", \"voltage\":";
           postString2 += v;
           postString2 += "}";
  
  Serial.println("----------------------------");
  Serial.println(postString);
  Serial.println("\n");
  Serial.println(postString2);
  Serial.println("----------------------------");
  delay(10000);

}

//char PostData[] = "{\"temperature\": \"Fred\", \"age\": 31}";

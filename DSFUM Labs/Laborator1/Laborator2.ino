 void setup()
{ 
  pinMode(13, OUTPUT); 
}

void loop(){
  static int luminozitate = 0, dir = 1;                
  static unsigned long last = 0;
  unsigned long s = micros();                 
  unsigned long on = (4000 * (unsigned long)luminozitate) / 255; 

  digitalWrite(13, HIGH);                
  while (micros() - s < on) {}
  digitalWrite(13, LOW);                 
  while (micros() - s < 4000) {}              

  unsigned long now = millis();              
  if (now - last >= 8) {                      
    luminozitate += dir;
    if (luminozitate >= 255) 
    { 
      luminozitate = 255; 
      dir = -1; 
    }
    if (luminozitate <= 0)   
    { 
      luminozitate = 0;   
      dir =  1; 
    }
    last = now;
  }
}

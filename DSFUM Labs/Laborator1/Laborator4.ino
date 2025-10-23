void setup ()
{
  pinMode(4,OUTPUT); // pinul pentru latch (blocare)
  pinMode(7,OUTPUT); // pin pentru sincronizarea ceasului
  pinMode(8,OUTPUT); // pin pentru transmiterea datelor
}

void loop()
{ 
    afiseazaValoare(0b00000010, 31);
    delay(1000);
    afiseazaValoare(0b11110010, 31); 
    delay(1000);
    afiseazaValoare(0b00100100, 31); 
    delay(1000);
    afiseazaValoare(0b01100000, 31); 
    delay(1000);
    afiseazaValoare(0b11010000, 31); 
    delay(1000);
    afiseazaValoare(0b01001000, 31); 
    delay(1000);
    afiseazaValoare(0b00001000, 31); 
    delay(1000);
    afiseazaValoare(0b11100010, 31); 
    delay(1000);
    afiseazaValoare(0b00000000, 31); 
    delay(1000);
    afiseazaValoare(0b01000000, 31); 
    delay(1000);
 
}
 
void afiseazaValoare(byte valoare, byte segment)
{
  digitalWrite(4,LOW);
  shiftOut(8, 7, LSBFIRST, valoare);
  shiftOut(8, 7, LSBFIRST, segment);
  digitalWrite(4,HIGH);
}
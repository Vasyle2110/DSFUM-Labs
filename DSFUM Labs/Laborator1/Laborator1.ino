void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  for(int i=8 ;i<14;i++)
    pinMode(i, OUTPUT);
}

// the loop function runs over and over again forever
/*void loop() {
  for(int i=8;i<=14;i++)
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);
  for(int i=8;i<=14;i++)                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
//Sa facem sa se aprinda ledurile secvential de la 8 la 13 la niste leduri
//Solutia mea
void loop() {
  for(int i=8;i<14;i++){
    digitalWrite(i, HIGH);
    delay(300);
    digitalWrite(i, LOW);  // turn the LED on (HIGH is the voltage level)
    delay(300);
  }
}
void loop(){
  digitalWrite((millis()%1800)%300+8,1);
  for(int i = 8 ;i<14;i++)
    digitalWrite(i,0);

}
*/
//un numar de la 1 pana la 63 si sa il reprezentam binar pe leduri
int binary(int k){
  for(int i=0;i<6;i++){
    digitalWrite(i+8,(k>>i)&1);
  }
}

void loop(){
  for(int i=0;i<64;i++){
    binary(i);
    delay(100);
  }
}

//soltie fara sa folosesc functia delay
void loop(){
  binary((millis()%6400)/100);
}

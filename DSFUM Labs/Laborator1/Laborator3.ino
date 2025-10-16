const int FIRST_PIN = 8;
const int LAST_PIN  = 13;
const unsigned long FRAME_US = 4000UL;    
enum Mode { OFF_M, ON_M, BLINK_M, FADEIN_M, FADEOUT_M, FADEINOUT_M };
Mode mode[14];                
unsigned long intervalMs[14],lastMs[14];      
int value255[14],blinkState[14];            

bool isPwmPin(int p) {
  return (p==3 || p==5 || p==6 || p==9 || p==10 || p==11);
}

void setPin(int pin, int on) { digitalWrite(pin, on ? HIGH : LOW); }

void pwmFrameAll() {
  unsigned long onTime[14];
  bool hiNow[14];

  for (int p = FIRST_PIN; p <= LAST_PIN; p++) {
    unsigned long on = (FRAME_US * (unsigned long)value255[p]) / 255UL;
    onTime[p] = on;
    hiNow[p]  = (on > 0);
    setPin(p, hiNow[p]);           
  }
  unsigned long start = micros();
  while (micros() - start < FRAME_US) {
    unsigned long e = micros() - start;
    for (int p = FIRST_PIN; p <= LAST_PIN; p++) {
      if (hiNow[p] && e >= onTime[p]) {
        setPin(p, 0);
        hiNow[p] = false;
      }
    }
  }
}

void applyCommand(const String& line) {
  int c1 = line.indexOf(',');
  int c2 = line.indexOf(',', c1 + 1);
  if (c1 < 0 || c2 < 0) return;

  String ledPart = line.substring(0, c1);
  String action= line.substring(c1 + 1, c2);
  String sInt = line.substring(c2 + 1);

  ledPart.trim(); action.trim(); sInt.trim();

  int startPin = 0, endPin = 0;
  int dash = ledPart.indexOf('-');
  if (dash >= 0) {
    startPin = ledPart.substring(0, dash).toInt();
    endPin = ledPart.substring(dash + 1).toInt();
  } else {
    startPin = endPin = ledPart.toInt();
  }
  if (startPin < FIRST_PIN) startPin = FIRST_PIN;
  if (endPin > LAST_PIN)  endPin   = LAST_PIN;
  if (startPin > endPin) { int t = startPin; startPin = endPin; endPin = t; }

  unsigned long ms = (unsigned long) sInt.toInt();
  if (ms == 0) ms = 500;

  for (int p = startPin; p <= endPin; p++) {
    if (action == "on")  { mode[p] = ON_M; value255[p] = 255; }
    else if (action == "off") { mode[p] = OFF_M; value255[p] = 0;   }
    else if (action == "blink"){ mode[p] = BLINK_M; blinkState[p] = 0; lastMs[p] = 0; }
    intervalMs[p] = ms;
  }

  Serial.print("OK: "); Serial.print(startPin);
  Serial.print('-'); Serial.print(endPin);
  Serial.print(" -> "); Serial.print(action);
  Serial.print(" la intervalul de timp "); Serial.print(ms); Serial.println(" ms");
}

bool readLine(String &out) {
  static String buf = "";
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\r') continue;
    if (c == '\n') { out = buf; buf = ""; return true; }
    buf += c;
  }
  return false;
}
void setup() {
  Serial.begin(9600);
  for (int p = FIRST_PIN; p <= LAST_PIN; p++) {
    pinMode(p, OUTPUT);
    mode[p] = OFF_M; value255[p] = 0; blinkState[p] = 0;
    setPin(p, 0);
  }
  Serial.println("Comenzi posibile: on ,off,blink,fadein,fadeout");
  Serial.println("Exemple:");
  Serial.println("8-10,blink,500");
  Serial.println("12,fadein,2000");
  Serial.println("11-13,fadeout,1000");
}

void loop() {
  String line;
  if (readLine(line)) applyCommand(line);
  unsigned long now = millis();
  for (int p = FIRST_PIN; p <= LAST_PIN; p++) {
    switch (mode[p]) {
      case OFF_M:{
        value255[p] = 0;   
        break;
      }
      case ON_M:{
        value255[p] = 255; 
        break;
      }
      case BLINK_M: {
        unsigned long half = (intervalMs[p] > 1) ? (intervalMs[p] / 2) : 1;
        if (now - lastMs[p] >= half) {
          blinkState[p] = !blinkState[p];
          value255[p] = blinkState[p] ? 255 : 0;
          lastMs[p] = now;
        }
      } break;
    }
  }
  pwmFrameAll();
}

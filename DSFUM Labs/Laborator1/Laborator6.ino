
int MOSI_pin = 11;
int SlaveSelect_pin = 10;
int Clock_pin = 13;


byte REG_DIGIT0      = 0x01;  
byte REG_DECODEMODE  = 0x09;
byte REG_INTENSITY   = 0x0A;
byte REG_SCANLIMIT   = 0x0B;
byte REG_SHUTDOWN    = 0x0C;
byte REG_DISPLAYTEST = 0x0F;

byte A[8][8];  
byte B[8][8];   


int frameDelayMs = 120; 
int initialLiveCells = 18; 


void putByte(byte data) {
  for (int i = 7; i >= 0; i--) {
    digitalWrite(Clock_pin, LOW);
    digitalWrite(MOSI_pin, data & (1 << i));
    digitalWrite(Clock_pin, HIGH);
  }
}
void toMax(byte reg, byte val) {
  digitalWrite(SlaveSelect_pin, LOW);
  putByte(reg);
  putByte(val);
  digitalWrite(SlaveSelect_pin, HIGH);
}

void displayMatrix(byte m[8][8]) {
  for (int i = 0; i < 8; i++) {
    byte val = 0;
    for (int j = 0; j < 8; j++) {
      val |= (m[i][7 - j] << j);
    }
    toMax(REG_DIGIT0 + i, val);
  }
}


void clearMatrix(byte m[8][8]) {
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      m[i][j] = 0;
}
void randomSeedMatrix(byte m[8][8], int aliveCount) {
  clearMatrix(m);
  for (int k = 0; k < aliveCount; k++) {
    int i = random(0, 8);
    int j = random(0, 8);
    m[i][j] = 1;
  }
}


int neighbors(byte m[8][8], int r, int c) {
  int cnt = 0;
  for (int dr = -1; dr <= 1; dr++) {
    for (int dc = -1; dc <= 1; dc++) {
      if (dr == 0 && dc == 0) continue;    
      int rr = r + dr;
      int cc = c + dc;

      if (rr >= 0 && rr < 8 && cc >= 0 && cc < 8) {
        cnt += m[rr][cc];
      }
    }
  }
  return cnt;
}


void stepLife() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      int n = neighbors(A, i, j);
      if (A[i][j] == 1) {
  
        B[i][j] = (n == 2 || n == 3) ? 1 : 0;
      } else {
       
        B[i][j] = (n == 3) ? 1 : 0;
      }
    }
  }
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      A[i][j] = B[i][j];
}


uint64_t hashBoard(byte m[8][8]) {
  uint64_t h = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      h <<= 1;
      h |= (uint64_t)(m[i][j] & 1);
    }
  }
  return h;
}
int population(byte m[8][8]) {
  int s = 0;
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      s += m[i][j];
  return s;
}

void initMax() {
  pinMode(MOSI_pin, OUTPUT);
  pinMode(Clock_pin, OUTPUT);
  pinMode(SlaveSelect_pin, OUTPUT);
  digitalWrite(Clock_pin, HIGH);

  toMax(REG_SCANLIMIT, 0x07);    
  toMax(REG_DECODEMODE, 0x00);    
  toMax(REG_DISPLAYTEST, 0x00);   
  toMax(REG_SHUTDOWN, 0x01);      
  toMax(REG_INTENSITY, 0x06);    
  for (int i = 0; i < 8; i++) toMax(REG_DIGIT0 + i, 0x00);
}

void setup() {
  randomSeed(analogRead(A0));  
  initMax();
  randomSeedMatrix(A, initialLiveCells);
}

void maybeAutoReset(uint64_t h, uint64_t hPrev, uint64_t hPrev2) {
  if (h == 0ULL || h == hPrev || h == hPrev2 || population(A) == 0) {
    randomSeedMatrix(A, initialLiveCells);
    toMax(REG_INTENSITY, 0x0F); delay(40);  
    toMax(REG_INTENSITY, 0x06);
  }
}

void loop() {
  static uint64_t hPrev = 0, hPrev2 = 0;

  displayMatrix(A);
  delay(frameDelayMs);


  uint64_t hNow = hashBoard(A);
  maybeAutoReset(hNow, hPrev, hPrev2);
  hPrev2 = hPrev;
  hPrev = hNow;

  stepLife();
}

#define Do   262
#define Re   294
#define Mi   330
#define Fa   349
#define Sol  392
#define dSol 415
#define La   440
#define Si   494
#define DO   523
#define RE   587

const byte LATCH  = 4;
const byte SRCLK  = 7;
const byte DATA   = 8;
const byte BUZZER = 3;

const byte CIFRE[10] = {
  0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001,
  0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000
};

const int MELODIE0[] = {
  Re,3,  Mi,3,  Fa,3,  Sol,3, La,3,  Si,3,  DO,3,  RE,3,
  RE,3,  DO,3,  Si,3,  La,3,  Sol,3, Fa,3,  Mi,3,  Re,3
};

const int MELODIE1[] = {
  Do,6,   Do,2,   Fa,12,  Fa,4,
  Sol,6,  Do,2,   La,8,   Fa,8,
  Fa,6,   Mi,2,   Re,8,   Sol,8,
  Fa,6,   Mi,2,   Fa,16,

  Do,6,   Do,2,   Fa,12,  Fa,4,
  Sol,6,  Do,2,   La,8,   Fa,8,
  Fa,6,   Mi,2,   Re,8,   La,8,
  La,6,   dSol,2, La,16,

  La,6,   Si,2,   DO,12,  La,4,
  Si,6,   DO,2,   RE,8,   Si,4,   Sol,4,
  Fa,6,   Re,2,   Do,8,   Fa,8,
  La,6,   Sol,2,  Fa,18
};

const int* MELODII[2] = { MELODIE0, MELODIE1 };
const byte NR_NOTE[2] = { 16, 46 };

byte mod = 0;

bool oraSetata = false;
long oraSec = 0;

bool swSetat = false;
bool swActiv = false;
long swSec = 0;

int  alarmaMinut = -1;
byte melodieClock = 0;
byte melodieSw = 0;

unsigned long ultimaSecunda = 0;

bool canta = false;
byte melodie = 0;
byte nota = 0;
unsigned long urmatoareaNota = 0;

char cmd[32];
byte cmdLen = 0;

void setup() {
  pinMode(LATCH, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);
  ultimaSecunda = millis();
}

void loop() {
  citesteSerial();
  tick();
  ruleazaMelodia();
  afiseaza(valoare());
}

void trimite(byte segmente, byte pozitie) {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, SRCLK, MSBFIRST, segmente);
  shiftOut(DATA, SRCLK, MSBFIRST, pozitie);
  digitalWrite(LATCH, HIGH);
}

void afiseaza(int v) {
  int div = 1000;
  for (byte i = 0; i < 4; i++) {
    trimite(CIFRE[(v / div) % 10], 1 << i);
    delay(3);
    trimite(0xFF, 0x00);
    div /= 10;
  }
}

int valoare() {
  if (mod == 0) return (oraSec / 3600) * 100 + (oraSec / 60) % 60;
  return (swSec / 60) * 100 + swSec % 60;
}

void tick() {
  if (millis() - ultimaSecunda < 1000) return;
  ultimaSecunda += 1000;

  if (oraSetata) {
    oraSec = (oraSec + 1) % 86400L;
    if (oraSec % 60 == 0 && oraSec / 60 == alarmaMinut)
      porneste(melodieClock);
  }

  if (swActiv) {
    swSec--;
    if (swSec <= 0) {
      swSec = 0;
      swActiv = false;
      porneste(melodieSw);
    }
  }
}

void porneste(byte index) {
  canta = true;
  melodie = index;
  nota = 0;
  urmatoareaNota = millis();
}

void ruleazaMelodia() {
  if (!canta || millis() < urmatoareaNota) return;

  if (nota >= NR_NOTE[melodie]) {
    canta = false;
    return;
  }

  int durata = MELODII[melodie][nota * 2 + 1] * 100;
  tone(BUZZER, MELODII[melodie][nota * 2], durata);
  urmatoareaNota = millis() + durata + 100;
  nota++;
}

void citesteSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (cmdLen > 0) {
        cmd[cmdLen] = '\0';
        executa(cmd);
        cmdLen = 0;
      }
    } else if (cmdLen < 31) {
      cmd[cmdLen++] = c;
    }
  }
}

void executa(char* s) {
  int a, b, c;

  if (strcmp(s, "clock") == 0) {
    mod = 0;
  }
  else if (strcmp(s, "stopwatch") == 0) {
    mod = 1;
  }
  else if (sscanf(s, "alarm clock %d:%d %d", &a, &b, &c) == 3) {
    alarmaMinut = a * 60 + b;
    melodieClock = c % 2;
  }
  else if (sscanf(s, "alarm stopwatch %d", &a) == 1) {
    melodieSw = a % 2;
  }
  else if (!oraSetata && sscanf(s, "clock %d:%d", &a, &b) == 2) {
    oraSec = (long)a * 3600 + (long)b * 60;
    oraSetata = true;
    mod = 0;
    ultimaSecunda = millis();
  }
  else if (!swSetat && sscanf(s, "stopwatch %d:%d", &a, &b) == 2) {
    swSec = (long)a * 60 + b;
    swSetat = true;
    swActiv = true;
    mod = 1;
    ultimaSecunda = millis();
  }
}
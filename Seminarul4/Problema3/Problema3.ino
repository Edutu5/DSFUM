const byte LATCH = 4;
const byte CLOCK = 7;
const byte DATA  = 8;

const byte BTN_PLUS  = A1;
const byte BTN_MINUS = A2;
const byte BTN_RESET = A3;

const byte CIFRE[10] = {
  0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001,
  0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000
};

const byte STINS = 0b11111111;
const int DIVIZOR[4] = {1000, 100, 10, 1};

int contor = 0;
bool prevPlus = HIGH, prevMinus = HIGH, prevReset = HIGH;

void setup() {
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(BTN_PLUS, INPUT);
  pinMode(BTN_MINUS, INPUT);
  pinMode(BTN_RESET, INPUT);
}

void loop() {
  afiseazaNumar(contor);
  citesteButoane();
}

void trimite(byte valoare, byte pozitie) {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, valoare);
  shiftOut(DATA, CLOCK, MSBFIRST, pozitie);
  digitalWrite(LATCH, HIGH);
}

void stingeDisplay() {
  trimite(STINS, 0x00);
}

void afiseazaNumar(int numar) {
  for (byte i = 0; i < 4; i++) {
    if (i < 3 && numar < DIVIZOR[i])
      trimite(STINS, 1 << i);
    else
      trimite(CIFRE[(numar / DIVIZOR[i]) % 10], 1 << i);
    delay(3);
    stingeDisplay();
  }
}

void citesteButoane() {
  bool curentPlus  = digitalRead(BTN_PLUS);
  bool curentMinus = digitalRead(BTN_MINUS);
  bool curentReset = digitalRead(BTN_RESET);

  if (prevPlus == HIGH && curentPlus == LOW && contor < 9999) contor++;
  if (prevMinus == HIGH && curentMinus == LOW && contor > 0) contor--;
  if (prevReset == HIGH && curentReset == LOW) contor = 0;

  prevPlus = curentPlus;
  prevMinus = curentMinus;
  prevReset = curentReset;
}
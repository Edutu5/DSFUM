const byte LATCH = 4;
const byte CLOCK = 7;
const byte DATA  = 8;

const byte BTN1 = A1;
const byte BTN2 = A2;
const byte BTN3 = A3;

const byte STINS = 0b11111111;
const byte POZITII[4] = {0b10000000, 0b01000000, 0b00100000, 0b00010000};

const char TEXT[] = "    dECEbAL_Si_FiUL_SAU_LOCUiESC_PE_PLAiUL_dACiEi    ";
const int LUNGIME = sizeof(TEXT) - 1;

const unsigned long INTERVAL = 400;

int offset = 0;
bool pauzat = false;
bool prev1 = HIGH, prev2 = HIGH, prev3 = HIGH;
unsigned long ultimulShift = 0;

void setup() {
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT);
  ultimulShift = millis();
}

void loop() {
  afiseazaFereastra();
  citesteButoane();

  if (pauzat) {
    ultimulShift = millis();
  } else if (millis() - ultimulShift >= INTERVAL) {
    ultimulShift = millis();
    offset++;
    if (offset > LUNGIME - 4) offset = 0;
  }
}

void trimite(byte valoare, byte pozitie) {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, valoare);
  shiftOut(DATA, CLOCK, LSBFIRST, pozitie);
  digitalWrite(LATCH, HIGH);
}

void stingeDisplay() {
  trimite(STINS, 0x00);
}

void afiseazaFereastra() {
  for (byte i = 0; i < 4; i++) {
    trimite(codChar(TEXT[offset + i]), POZITII[i]);
    delay(3);
    stingeDisplay();
  }
}

void citesteButoane() {
  bool c1 = digitalRead(BTN1);
  bool c2 = digitalRead(BTN2);
  bool c3 = digitalRead(BTN3);

  if ((prev1 == HIGH && c1 == LOW) ||
      (prev2 == HIGH && c2 == LOW) ||
      (prev3 == HIGH && c3 == LOW))
    pauzat = !pauzat;

  prev1 = c1;
  prev2 = c2;
  prev3 = c3;
}

byte codChar(char c) {
  switch (c) {
    case '0': return 0b00000011;
    case '1': return 0b11110011;
    case '2': return 0b00100101;
    case '3': return 0b00001101;
    case '4': return 0b10011001;
    case '5': return 0b01001001;
    case '6': return 0b01000001;
    case '7': return 0b00011111;
    case '8': return 0b00000001;
    case '9': return 0b00001001;
    case 'A': return 0b00010001;
    case 'b': return 0b11000001;
    case 'C': return 0b01100011;
    case 'd': return 0b10000101;
    case 'E': return 0b01100001;
    case 'F': return 0b01110001;
    case '_': return 0b11101111;
    case 'L': return 0b11100011;
    case 'i': return 0b11011111;
    case 'J': return 0b10000111;
    case 'O': return 0b00000011;
    case 'P': return 0b00110001;
    case 'S': return 0b01001001;
    case 'U': return 0b10000011;
    default:  return STINS;
  }
}
const byte LATCH = 4;
const byte CLOCK = 7;
const byte DATA  = 8;

const byte POZITIE = 0b10000000;

const byte LITERE[6] = {
  0b00010001, 0b11000001, 0b01100011, 0b10000101, 0b01100001, 0b01110001
};

void setup() {
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
}

void loop() {
  for (byte i = 0; i < 6; i++) {
    afiseazaValoare(LITERE[i], POZITIE);
    delay(5000);
  }
}

void afiseazaValoare(byte valoare, byte segment) {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, valoare);
  shiftOut(DATA, CLOCK, LSBFIRST, segment);
  digitalWrite(LATCH, HIGH);
}
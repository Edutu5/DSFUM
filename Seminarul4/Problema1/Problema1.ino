const byte LATCH = 4;
const byte CLOCK = 7;
const byte DATA  = 8;

const byte POZITIE = 0b10000000;

const byte CIFRE[10] = {
  0b00000011, 0b11110011, 0b00100101, 0b01100001, 0b11010001,
  0b01001001, 0b00001001, 0b11100011, 0b00000001, 0b01000001
};

void setup() {
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
}

void loop() {
  for (byte i = 0; i <= 9; i++) {
    afiseazaValoare(CIFRE[i], POZITIE);
    delay(1000);
  }
}

void afiseazaValoare(byte valoare, byte segment) {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, valoare);
  shiftOut(DATA, CLOCK, LSBFIRST, segment);
  digitalWrite(LATCH, HIGH);
}
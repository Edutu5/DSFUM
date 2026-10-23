const byte LATCH = 4;
const byte CLOCK = 7;
const byte DATA  = 8;
const byte RUNDE = 10;

const byte CIFRE[10] = {
  0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001,
  0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000
};

const int DIVIZOR[4] = {1000, 100, 10, 1};

unsigned long timpi[RUNDE];
int valori[RUNDE];

void setup() {
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  Serial.begin(9600);
  stingeDisplay();

  Serial.println("Apasa Enter pentru a incepe.");
  while (Serial.available() == 0) {}
  goleste();
  randomSeed(micros());

  for (byte runda = 0; runda < RUNDE; runda++) {
    int numar = random(1000, 10000);
    goleste();

    Serial.print("Runda ");
    Serial.println(runda + 1);

    unsigned long start = millis();
    int raspuns = citesteRaspuns(numar);
    unsigned long timp = millis() - start;
    stingeDisplay();

    timpi[runda] = timp;
    valori[runda] = numar;

    Serial.print(numar);
    Serial.print(" -> ");
    Serial.print(raspuns);
    Serial.print(" : ");
    Serial.print(timp);
    Serial.println(" ms");

    delay(700);
  }

  byte iMin = 0, iMax = 0;
  unsigned long suma = 0;
  for (byte i = 0; i < RUNDE; i++) {
    if (timpi[i] < timpi[iMin]) iMin = i;
    if (timpi[i] > timpi[iMax]) iMax = i;
    suma += timpi[i];
  }

  Serial.print("Timp minim: ");
  Serial.print(timpi[iMin]);
  Serial.print(" ms pentru valoarea ");
  Serial.println(valori[iMin]);

  Serial.print("Timp maxim: ");
  Serial.print(timpi[iMax]);
  Serial.print(" ms pentru valoarea ");
  Serial.println(valori[iMax]);

  Serial.print("Media timpilor: ");
  Serial.print((float)suma / RUNDE, 1);
  Serial.println(" ms");
}

void loop() {}

void afiseazaCifra(byte cifra, byte pozitie) {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, CIFRE[cifra]);
  shiftOut(DATA, CLOCK, MSBFIRST, pozitie);
  digitalWrite(LATCH, HIGH);
}

void stingeDisplay() {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, 0xFF);
  shiftOut(DATA, CLOCK, MSBFIRST, 0x00);
  digitalWrite(LATCH, HIGH);
}

void afiseazaNumar(int numar) {
  for (byte i = 0; i < 4; i++) {
    afiseazaCifra((numar / DIVIZOR[i]) % 10, 1 << i);
    delay(3);
    stingeDisplay();
  }
}

void goleste() {
  unsigned long t = millis();
  while (millis() - t < 60)
    while (Serial.available() > 0)
      Serial.read();
}

int citesteRaspuns(int numarAfisat) {
  char buffer[5];
  byte n = 0;
  while (n < 4) {
    afiseazaNumar(numarAfisat);
    while (Serial.available() > 0 && n < 4) {
      char c = Serial.read();
      if (c >= '0' && c <= '9') buffer[n++] = c;
    }
  }
  buffer[4] = '\0';
  return atoi(buffer);
}
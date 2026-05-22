// define portas sensor
static const int sD8 = A0;
static const int sD7 = A1;
static const int sD6 = A2;
static const int sD5 = A3;
static const int sD4 = A4;
static const int sD3 = A5;
static const int sD2 = 2;
static const int sD1 = 4;

// define portas saida ponte
static const int pSB = 9;
static const int pA1 = 8;
static const int pA2 = 7;
static const int pB1 = 12;
static const int pB2 = 13;
static const int pPA = 3;
static const int pPB = 11;






void setup() {
  // define modo pinos sensor
  int pinosSensor[] = {sD1, sD2, sD3, sD4, sD5, sD6, sD7, sD8};
  for (int pino : pinosSensor) {
    pinMode(pino, INPUT);
  }

  // define modo pinos ponte
  int pinosPonte[] = {pSB, pA1, pA2, pB1, pB2, pPA, pPB};
  for (int pino : pinosPonte) {
    pinMode(pino, OUTPUT);
  }

  // standby e PWMs high no setup
  digitalWrite(pSB, HIGH);
  digitalWrite(pPA, HIGH);
  digitalWrite(pPB, HIGH);


  // define serial para saída
  Serial.begin(9600);
}


void loop() {
  // define saídas 
  int s1 = digitalRead(sD1);
  int s2 = digitalRead(sD2);
  int s3 = digitalRead(sD3);
  int s4 = digitalRead(sD4);
  int s5 = digitalRead(sD5);
  int s6 = digitalRead(sD6);
  int s7 = digitalRead(sD7);
  int s8 = digitalRead(sD8);
  
  // imprime saídas no serial
  int sensoresOut[] = {s1, s2, s3, s4, s5, s6, s7, s8};
  for (int sensor : sensoresOut) {
    Serial.print(sensor);
    Serial.print(" ");
  }
  Serial.println();

  frente();
  delay(100);
}


void frente() {
  // cada
  digitalWrite(pA1, HIGH);
  digitalWrite(pA2, LOW);
  digitalWrite(pB1, HIGH);
  digitalWrite(pB2, LOW);
}
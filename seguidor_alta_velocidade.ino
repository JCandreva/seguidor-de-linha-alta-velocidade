// define portas sensor
static const int sD8 = A0;
static const int sD7 = A1;
static const int sD6 = A2;
static const int sD3 = A5;
static const int sD2 = A4;
static const int sD1 = A3;
static const int sIR = 5;

// define portas saida ponte
static const int pSB = 9;
static const int pA1 = 8;
static const int pA2 = 7;
static const int pB1 = 12;
static const int pB2 = 13;
static const int pPA = 3;
static const int pPB = 11;

// --- CONTROLE AUTOMÁTICO DE VELOCIDADE ---
static const int VELOCIDADE_RETO = 200; // <- MEXA APENAS AQUI para mudar a velocidade geral!

// O Arduino calcula o resto sozinho usando as proporções equilibradas:
static const int VELOCIDADE_CURVA_MAIOR = constrain(VELOCIDADE_RETO * 1.4, 0, 255); 
static const int VELOCIDADE_CURVA_MENOR = constrain(VELOCIDADE_RETO * 0.2, 0, 255);

// Variável de memória: guarda a última direção decidida (0 = frente, 1 = esquerda, 2 = direita)
int ultimaDirecao = 0; 

void setup() {
  int pinosSensor[] = {sD1, sD2, sD3, sD6, sD7, sD8};
  for (int pino : pinosSensor) {
    pinMode(pino, INPUT);
  }
  pinMode(sIR, OUTPUT);

  int pinosPonte[] = {pSB, pA1, pA2, pB1, pB2, pPA, pPB};
  for (int pino : pinosPonte) {
    pinMode(pino, OUTPUT);
  }

  digitalWrite(pSB, HIGH);
  analogWrite(pPA, 50);
  analogWrite(pPB, 50);
  digitalWrite(sIR, HIGH);

  // Aumentado para velocidade máxima estável do Arduino Uno/Nano
  Serial.begin(115200); 
}

void loop() {
  int s1 = analogRead(sD1);
  int s2 = analogRead(sD2);
  int s3 = analogRead(sD3);
  int s6 = analogRead(sD6);
  int s7 = analogRead(sD7);
  int s8 = analogRead(sD8);
  
  // Agrupamento dos setores
  int somaEsquerda = s1 + s2;
  int somaCentro   = s3 + s6;
  int somaDireita  = s7 + s8;
  
  // Margem de erro para entender o que é linha e o que é fundo do cenário
  // Se sua pista for preta no branco, ajuste esse valor baseado nos testes
  const int margemErro = 150; 
  const int limiarPistaVazia = 300; // Se a soma de tudo for menor que isso, ele perdeu a pista

  // CASO ESPECIAL: O robô perdeu a linha (entrou no ponto cego)
  if ((somaEsquerda + somaCentro + somaDireita) < limiarPistaVazia) {
    if (ultimaDirecao == 1) {
      esquerda(); // Força a curva para a esquerda para tentar achar a linha de volta
      Serial.println("RECUPERANDO ESQUERDA");
    } else if (ultimaDirecao == 2) {
      direita(); // Força a curva para a direita para tentar achar a linha de volta
      Serial.println("RECUPERANDO DIREITA");
    } else {
      frente();
    }
  }
  // TOMADA DE DECISÃO NORMAL
  else if ((somaDireita > somaEsquerda) && (somaDireita > somaCentro + margemErro)) {
    direita();
    ultimaDirecao = 2; // Salva na memória que virou para a direita
    Serial.println("-> DIREITA");
  }
  else if ((somaEsquerda > somaDireita) && (somaEsquerda > somaCentro + margemErro)) {
    esquerda();
    ultimaDirecao = 1; // Salva na memória que virou para a esquerda
    Serial.println("<- ESQUERDA");
  }
  else {
    frente(); 
    ultimaDirecao = 0; // Linha está no centro
    Serial.println("|| FRENTE");
  }
  
  // O delay foi REMOVIDO daqui. O Arduino precisa rodar na velocidade máxima do processador!
}

void avanco() {
  digitalWrite(pA1, HIGH);
  digitalWrite(pA2, LOW);
  digitalWrite(pB1, LOW);
  digitalWrite(pB2, HIGH);
}

void frente(){
  avanco();
  analogWrite(pPA, VELOCIDADE_RETO);
  analogWrite(pPB, VELOCIDADE_RETO);
}

void esquerda() {
  avanco();
  analogWrite(pPA, VELOCIDADE_CURVA_MAIOR);
  analogWrite(pPB, VELOCIDADE_CURVA_MENOR);
}

void direita() {
  avanco();
  analogWrite(pPA, VELOCIDADE_CURVA_MENOR);
  analogWrite(pPB, VELOCIDADE_CURVA_MAIOR);
}

void parar() {
  digitalWrite(pA1, LOW);
  digitalWrite(pA2, LOW);
  digitalWrite(pB1, LOW);
  digitalWrite(pB2, LOW);
}

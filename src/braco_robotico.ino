// ============================================================
//  Braço Robótico de Coleta de Amostras — Docking & Retrieval
//  Global Solution 2026 · FIAP · Project-Based Maker Lab
// ============================================================
//  Componentes:
//    - Arduino Uno
//    - Servo 1 (Ombro): pino 9  → comandos U (Up) e D (Down)
//    - Servo 2 (Garra) : pino 10 → comandos O (Open) e C (Close)
//    - LED de status   : pino 13
//
//  Alimentação: fonte de bancada 5V ou 6V nos servos
//  Comunicação: Monitor Serial a 9600 baud
//
//  Comandos disponíveis (letras maiúsculas ou minúsculas):
//    U → Ombro sobe   (+15°, máx 150°)
//    D → Ombro desce  (-15°, mín  30°)
//    O → Garra abre   (move para 120°)
//    C → Garra fecha  (move para  20°)
// ============================================================

#include <Servo.h>

// --- Pinos ---
const int PINO_SERVO_OMBRO = 9;
const int PINO_SERVO_GARRA = 10;
const int PINO_LED         = 13;

// --- Ângulos iniciais ---
const int ANGULO_OMBRO_INICIAL = 90;   // posição de repouso
const int ANGULO_GARRA_INICIAL = 20;   // garra fechada no início

// --- Limites do ombro ---
const int OMBRO_MIN  = 30;
const int OMBRO_MAX  = 150;
const int PASSO_OMBRO = 15;            // graus por comando

// --- Posições da garra ---
const int GARRA_ABERTA  = 120;
const int GARRA_FECHADA = 20;

// --- Objetos ---
Servo servoOmbro;
Servo servoGarra;

// --- Estado atual ---
int anguloOmbro = ANGULO_OMBRO_INICIAL;
int anguloGarra = ANGULO_GARRA_INICIAL;

// ============================================================
void setup() {
  Serial.begin(9600);

  servoOmbro.attach(PINO_SERVO_OMBRO);
  servoGarra.attach(PINO_SERVO_GARRA);

  pinMode(PINO_LED, OUTPUT);

  // Posição inicial
  servoOmbro.write(anguloOmbro);
  servoGarra.write(anguloGarra);

  // Pisca LED 3x para indicar que o sistema iniciou
  piscarLED(3);

  Serial.println("========================================");
  Serial.println(" BRACO ROBOTICO — Docking & Retrieval  ");
  Serial.println("========================================");
  Serial.println(" Comandos:");
  Serial.println("  U = Ombro SOBE    (+15 graus)");
  Serial.println("  D = Ombro DESCE   (-15 graus)");
  Serial.println("  O = Garra ABRE");
  Serial.println("  C = Garra FECHA");
  Serial.println("========================================");
  imprimirStatus();
}

// ============================================================
void loop() {
  if (Serial.available() > 0) {
    char comando = Serial.read();
    comando = toupper(comando);  // aceita maiúscula ou minúscula

    // Ignora Enter e espaços
    if (comando == '\n' || comando == '\r' || comando == ' ') {
      return;
    }

    executarComando(comando);
  }
}

// ============================================================
void executarComando(char cmd) {
  switch (cmd) {

    case 'U':  // Ombro sobe
      if (anguloOmbro + PASSO_OMBRO <= OMBRO_MAX) {
        anguloOmbro += PASSO_OMBRO;
        moverServoSuave(servoOmbro, anguloOmbro);
        Serial.print("[U] Ombro subiu -> ");
        Serial.print(anguloOmbro);
        Serial.println(" graus");
      } else {
        Serial.println("[U] LIMITE maximo do ombro atingido!");
      }
      break;

    case 'D':  // Ombro desce
      if (anguloOmbro - PASSO_OMBRO >= OMBRO_MIN) {
        anguloOmbro -= PASSO_OMBRO;
        moverServoSuave(servoOmbro, anguloOmbro);
        Serial.print("[D] Ombro desceu -> ");
        Serial.print(anguloOmbro);
        Serial.println(" graus");
      } else {
        Serial.println("[D] LIMITE minimo do ombro atingido!");
      }
      break;

    case 'O':  // Garra abre
      anguloGarra = GARRA_ABERTA;
      moverServoSuave(servoGarra, anguloGarra);
      Serial.println("[O] Garra ABERTA");
      break;

    case 'C':  // Garra fecha
      anguloGarra = GARRA_FECHADA;
      moverServoSuave(servoGarra, anguloGarra);
      Serial.println("[C] Garra FECHADA — amostra capturada!");
      piscarLED(2);  // confirmação visual de captura
      break;

    default:
      Serial.print("[?] Comando desconhecido: '");
      Serial.print(cmd);
      Serial.println("' — Use U, D, O ou C");
      break;
  }

  imprimirStatus();
}

// ============================================================
// Move o servo gradualmente para suavizar o movimento
void moverServoSuave(Servo &servo, int anguloAlvo) {
  int anguloAtual = servo.read();
  int passo = (anguloAlvo > anguloAtual) ? 1 : -1;

  digitalWrite(PINO_LED, HIGH);  // LED aceso durante movimento

  while (anguloAtual != anguloAlvo) {
    anguloAtual += passo;
    servo.write(anguloAtual);
    delay(15);  // 15ms por grau = movimento suave
  }

  digitalWrite(PINO_LED, LOW);   // LED apaga ao terminar
}

// ============================================================
// Pisca o LED n vezes
void piscarLED(int vezes) {
  for (int i = 0; i < vezes; i++) {
    digitalWrite(PINO_LED, HIGH);
    delay(200);
    digitalWrite(PINO_LED, LOW);
    delay(200);
  }
}

// ============================================================
// Imprime o estado atual no Monitor Serial
void imprimirStatus() {
  Serial.println("----------------------------------------");
  Serial.print("  Ombro: ");
  Serial.print(anguloOmbro);
  Serial.print(" graus | Garra: ");
  Serial.print(anguloGarra == GARRA_ABERTA ? "ABERTA" : "FECHADA");
  Serial.println();
  Serial.println("----------------------------------------");
}

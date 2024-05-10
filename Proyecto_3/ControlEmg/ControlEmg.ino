#include <Servo.h>

Servo SERVO_1;
#define SERVO_PIN 8
#define UMBRAL 6000

// Definir los coeficientes del filtro IIR pasa altas
float b_iir[5] = {0.0048, -0.0193, 0.0289, -0.0193, 0.0048};  // Ajusta los coeficientes según tu filtro pasa altas
float a_iir[5] = {1.0, -2.3695, 2.3140, -1.0547, 0.1874};

const int numDatos = 100;
int datos[numDatos];
int ultpos = 0;

int outByte = 0;
int flexion = 0;  // Variable para indicar movimiento de flexión

void setup() {
  SERVO_1.attach(SERVO_PIN);
  Serial.begin(9600);
  for (int i = 0; i < numDatos; i++) {
    datos[i] = analogRead(A0);
    delay(100);
  }
}

void loop() {
  datos[ultpos] = analogRead(A0);  // Leer dato analógico
  ultpos++;
  if (ultpos >= numDatos) ultpos = 0;

  // Aplicar filtro IIR pasa altas
  applyIIRFilter();

  float varianza = calcularVarianza();

  // Verificar tendencia de la señal para clasificar el movimiento
  if (varianza > UMBRAL) {
    if (flexion == 0) {
      Serial.println("Flexion detectada");
      flexion = 1;  // Marcar que se detectó flexión para evitar repeticiones
    }
    SERVO_1.write(120);  // Mover servomotor a posición alta
  } else {
    flexion = 0;  // Reiniciar la detección de flexión
    SERVO_1.write(10);   // Mover servomotor a posición baja
  }

  Serial.println(varianza);  // Enviar dato por puerto serie

  delay (50);
}

void applyIIRFilter() {
  static float filterData[numDatos] = {0};  // Inicializar la variable estática
  for (int i = 0; i < numDatos; i++) {
    filterData[i] = 0;
    for (int j = 0; j < 5; j++) {
      int index = (i + j) % numDatos;
      filterData[i] += b_iir[j] * datos[index] - a_iir[j+1] * filterData[index];
    }
  }
  outByte = (int)filterData[numDatos - 1];  // Actualizar el valor filtrado
}

float calcularVarianza() {
  float media = 0;
  for (int i = 0; i < numDatos; i++) {
    media += datos[i];
  }
  media /= numDatos;

  float varianza = 0;
  for (int i = 0; i < numDatos; i++) {
    varianza += (datos[i] - media) * (datos[i] - media);
  }
  varianza /= (numDatos - 1);

  return varianza;
}



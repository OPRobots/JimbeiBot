#include "pines.h"

void inicializar_pines() {
  pinMode(S_RIVAL_1, INPUT);
  pinMode(S_RIVAL_2, INPUT);

  pinMode(S_LINEA_I, INPUT);
  pinMode(S_LINEA_D, INPUT);

  pinMode(MOTOR_RIGHT_A, OUTPUT);
  pinMode(MOTOR_RIGHT_B, OUTPUT);
  pinMode(MOTOR_LEFT_A, OUTPUT);
  pinMode(MOTOR_LEFT_B, OUTPUT);
  digitalWrite(MOTOR_RIGHT_A, HIGH);
  digitalWrite(MOTOR_RIGHT_B, HIGH);
  digitalWrite(MOTOR_LEFT_A, HIGH);
  digitalWrite(MOTOR_LEFT_B, HIGH);

  pinMode(MOTOR_SUCTION, OUTPUT);

  pinMode(NEOPIXEL, OUTPUT);


  pinMode(BOTON, INPUT_PULLDOWN);
  pinMode(MOD_START, INPUT);
  attachInterrupt(digitalPinToInterrupt(MOD_START), rc5_isr, CHANGE);
}
#include "pines.h"

void inicializar_pines(){
  pinMode(S_RIVAL_1, INPUT);
  pinMode(S_RIVAL_2, INPUT);
  pinMode(S_RIVAL_3, INPUT);
  pinMode(S_RIVAL_4, INPUT);

  pinMode(S_LINEA_I, INPUT);
  pinMode(S_LINEA_D, INPUT);

  pinMode(PWM_MD, OUTPUT);
  pinMode(PWM_MI, OUTPUT);

  pinMode(LED_ADELANTE, OUTPUT);
  pinMode(LED_ATRAS, OUTPUT);
  pinMode(LED_DERECHA, OUTPUT);
  pinMode(LED_IZQUIERDA, OUTPUT);

  pinMode(BOTON, INPUT_PULLUP);
}
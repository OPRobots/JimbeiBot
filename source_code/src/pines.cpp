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

  pinMode(NEOPIXEL, OUTPUT);

  pinMode(BOTON, INPUT_PULLDOWN);
}
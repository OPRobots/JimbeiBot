#ifndef __PINES_H
#define __PINES_H
#include "Arduino.h"

// PINES
// Sensores numerados por orden de colocación en el robot mirandolo desde arriba con la cuchilla hacia adelante

#define S_RIVAL_1 2
#define S_RIVAL_2 4
#define S_RIVAL_3 0
#define S_RIVAL_4 1

#define S_LINEA_I 3
#define S_LINEA_D 21

#define PWM_MD 6
#define PWM_MI 9

#define LED_ADELANTE 20
#define LED_ATRAS 8
#define LED_DERECHA 7
#define LED_IZQUIERDA 10

#define BOTON 5

void inicializar_pines();

#endif
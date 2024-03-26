#ifndef __PINES_H
#define __PINES_H
#include "Arduino.h"

// PINES
// Sensores numerados por orden de colocación en el robot mirandolo desde arriba con la cuchilla hacia adelante

#define S_RIVAL_1 0
#define S_RIVAL_2 1
#define S_RIVAL_3 3
#define S_RIVAL_4 4

#define S_LINEA_I 7
#define S_LINEA_D 2

#define PWM_MD 8
#define PWM_MI 5

#define NEOPIXEL 20

#define BOTON 6

void inicializar_pines();

#endif
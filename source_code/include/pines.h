#ifndef __PINES_H
#define __PINES_H
#include "Arduino.h"
#include "rc5.h"

// PINES
// Sensores numerados por orden de colocación en el robot mirandolo desde arriba con la cuchilla hacia adelante

#define S_RIVAL_1 0
#define S_RIVAL_2 1

#define S_LINEA_I 3
#define S_LINEA_D 4

#define SERVO 9

#define MOTOR_RIGHT_A 2
#define MOTOR_RIGHT_B 8
#define MOTOR_LEFT_A 7
#define MOTOR_LEFT_B 10
#define MOTOR_SUCTION 5

#define NEOPIXEL 20

#define BOTON 6
#define MOD_START 21

void inicializar_pines();

#endif
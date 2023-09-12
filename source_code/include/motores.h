#ifndef __MOTORES_H
#define __MOTORES_H
#include <Arduino.h>
#include "pines.h"
#include "sensores.h"

void inicializar_motores();
void calculo_vel_motores(int,int);
void asignacion_vel_motores();
void parar_motores();

#endif
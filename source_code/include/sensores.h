#ifndef __SENSORES_H
#define __SENSORES_H
#include <Arduino.h>
#include "pines.h"

void filtro_sensores();
bool sensor1();
bool sensor2();
bool sensor3();
bool sensor4();
int sensor1_analog();
int sensor2_analog();
int sensor3_analog();
int sensor4_analog();
bool sensor_linea_I();
bool sensor_linea_I();
bool sensor_linea_D();
bool boton();
int posicion_rival();
int posicion_rival_chusta();

#endif
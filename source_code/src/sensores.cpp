#include "sensores.h"

const int MAGNITUD_FILTRO = 20;
const int UMBRAL = 2000;
const int UMBRAL_HISTERESIS = 1500;

int posicion = 0;

int s_rival_1 = 0;
int s_rival_2 = 0;
int s_rival_3 = 0;
int s_rival_4 = 0;

bool s_rival_1_bool = false;
bool s_rival_2_bool = false;
bool s_rival_3_bool = false;
bool s_rival_4_bool = false;

int Filtro_s1[MAGNITUD_FILTRO];
int Filtro_s2[MAGNITUD_FILTRO];
int Filtro_s3[MAGNITUD_FILTRO];
int Filtro_s4[MAGNITUD_FILTRO];

int i_s;

void filtro_sensores() {

  Filtro_s1[i_s] = analogRead(S_RIVAL_1);
  Filtro_s2[i_s] = analogRead(S_RIVAL_2);
  Filtro_s3[i_s] = analogRead(S_RIVAL_3);
  Filtro_s4[i_s] = analogRead(S_RIVAL_4);
  i_s = (i_s + 1) % MAGNITUD_FILTRO; // Avanza el índice circularmente cuando supera MAGNITUD FILTRO vuelve a ser 0

  for (int i = 0; i < MAGNITUD_FILTRO; i++) {
    s_rival_1 += Filtro_s1[i];
    s_rival_2 += Filtro_s2[i];
    s_rival_3 += Filtro_s3[i];
    s_rival_4 += Filtro_s4[i];
  }

  s_rival_1 = s_rival_1 / MAGNITUD_FILTRO;
  s_rival_2 = s_rival_2 / MAGNITUD_FILTRO;
  s_rival_3 = s_rival_3 / MAGNITUD_FILTRO;
  s_rival_4 = s_rival_4 / MAGNITUD_FILTRO;
  filtro_sensores_histeresis();
  // s_rival_1_bool = s_rival_1 > UMBRAL;
  // s_rival_2_bool = s_rival_2 > UMBRAL;
  // s_rival_3_bool = s_rival_3 > UMBRAL;
  // s_rival_4_bool = s_rival_4 > UMBRAL;
}

void filtro_sensores_histeresis() {

  // s_rival_1 = analogRead(S_RIVAL_1);
  // s_rival_2 = analogRead(S_RIVAL_2);
  // s_rival_3 = analogRead(S_RIVAL_3);
  // s_rival_4 = analogRead(S_RIVAL_4);

  if (s_rival_1 > UMBRAL) {
    s_rival_1_bool = true;
  } else if (s_rival_1 < UMBRAL_HISTERESIS) {
    s_rival_1_bool = false;
  }
  if (s_rival_2 > UMBRAL) {
    s_rival_2_bool = true;
  } else if (s_rival_2 < UMBRAL_HISTERESIS) {
    s_rival_2_bool = false;
  }
  if (s_rival_3 > UMBRAL) {
    s_rival_3_bool = true;
  } else if (s_rival_3 < UMBRAL_HISTERESIS) {
    s_rival_3_bool = false;
  }
  if (s_rival_4 > UMBRAL) {
    s_rival_4_bool = true;
  } else if (s_rival_4 < UMBRAL_HISTERESIS) {
    s_rival_4_bool = false;
  }
}

bool sensor1() {
  return s_rival_1_bool;
}
bool sensor2() {
  return s_rival_2_bool;
}
bool sensor3() {
  return s_rival_3_bool;
}
bool sensor4() {
  return s_rival_4_bool;
}

int sensor1_analog() {
  return s_rival_1;
}
int sensor2_analog() {
  return s_rival_2;
}
int sensor3_analog() {
  return s_rival_3;
}
int sensor4_analog() {
  return s_rival_4;
}

bool sensor_linea_I() {
  return digitalRead(S_LINEA_I);
}
bool sensor_linea_D() {
  return digitalRead(S_LINEA_D);
}
bool boton() {
  return !digitalRead(BOTON);
}

int posicion_rival_chusta() {
  if (sensor1() && !sensor2() && !sensor3() && !sensor4()) {
    posicion = -50;
  } else if (!sensor1() && sensor2() && !sensor3() && !sensor4()) {
    posicion = -100;
  } else if (!sensor1() && !sensor2() && sensor3() && !sensor4()) {
    posicion = 100;
  } else if (!sensor1() && !sensor2() && !sensor3() && sensor4()) {
    posicion = 50;
  } else if (sensor1() && sensor4()) {
    posicion = 0;
  } else if (sensor1() && sensor2() && !sensor3() && !sensor4()) {
    posicion = -75;
  } else if (!sensor1() && !sensor2() && sensor3() && sensor4()) {
    posicion = 75;
  } else if (!sensor1() && !sensor2() && !sensor3() && !sensor4()) {
    if (posicion > 0) {
      posicion = 150;
    } else {
      posicion = -150;
    }
  }
  return posicion;
}

int posicion_rival() {
  int sumaSensoresPonderados = 0;
  int sumaSensores = 0;
  int sensoresDetectando = 0;

  sumaSensoresPonderados = sumaSensoresPonderados + ((2 + 1) * sensor1() * 1000);
  sumaSensores = sumaSensores + sensor1();
  if (sensor1()) {
    sensoresDetectando = sensoresDetectando + 1;
  }
  sumaSensoresPonderados = sumaSensoresPonderados + ((1 + 1) * sensor2() * 1000);
  sumaSensores = sumaSensores + sensor2();
  if (sensor2()) {
    sensoresDetectando = sensoresDetectando + 1;
  }
  sumaSensoresPonderados = sumaSensoresPonderados + ((4 + 1) * sensor3() * 1000);
  sumaSensores = sumaSensores + sensor3();
  if (sensor3()) {
    sensoresDetectando = sensoresDetectando + 1;
  }
  sumaSensoresPonderados = sumaSensoresPonderados + ((3 + 1) * sensor4() * 1000);
  sumaSensores = sumaSensores + sensor4();
  if (sensor4()) {
    sensoresDetectando = sensoresDetectando + 1;
  }

  if (sensoresDetectando > 0) {

    posicion = ((sumaSensoresPonderados / sumaSensores) - 2500); // 2500 = (4 + 1) * (float)(1000 / 2))

  } else if (posicion > 0) {
    posicion = 2500;
    // TODO: Contemplar parada con 0 sensores detectando.
  } else {
    posicion = -2500;
    // TODO: Contemplar parada con 0 sensores detectando.
  }
  return posicion;
}

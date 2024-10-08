#include "sensores.h"

const int MAGNITUD_FILTRO = 20;
const int MAGNITUD_FILTRO_TEMPORAL = 10; // Tiempo en ms para cambiar de estado el sensor
const int UMBRAL = 1300;
const int UMBRAL_HISTERESIS = 850;
const int CONTADOR_LINEA = 6;

int posicion = 0;
bool rival_encima = false;

int contador_boton = 0;
int contador_linea_D = 0;
int contador_linea_I = 0;

int s_rival_1 = 0;
int s_rival_2 = 0;
int s_rival_3 = 0;
int s_rival_4 = 0;

bool s_rival_1_bool = false;
bool s_rival_2_bool = false;
bool s_rival_3_bool = false;
bool s_rival_4_bool = false;

bool last_s_rival_1_bool = false;
bool last_s_rival_2_bool = false;
bool last_s_rival_3_bool = false;
bool last_s_rival_4_bool = false;

long s_rival_1_ms_true = 0;
long s_rival_1_ms_false = 0;
long s_rival_2_ms_true = 0;
long s_rival_2_ms_false = 0;
long s_rival_3_ms_true = 0;
long s_rival_3_ms_false = 0;
long s_rival_4_ms_true = 0;
long s_rival_4_ms_false = 0;

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
    if (!s_rival_1_bool) {
      s_rival_1_ms_true = millis();
    }
    s_rival_1_bool = true;
  } else if (s_rival_1 < UMBRAL_HISTERESIS) {
    if (s_rival_1_bool) {
      s_rival_1_ms_false = millis();
    }
    s_rival_1_bool = false;
  }
  if (s_rival_2 > UMBRAL) {
    if (!s_rival_2_bool) {
      s_rival_2_ms_true = millis();
    }
    s_rival_2_bool = true;
  } else if (s_rival_2 < UMBRAL_HISTERESIS) {
    if (s_rival_2_bool) {
      s_rival_2_ms_false = millis();
    }
    s_rival_2_bool = false;
  }
  if (s_rival_3 > UMBRAL) {
    if (!s_rival_3_bool) {
      s_rival_3_ms_true = millis();
    }
    s_rival_3_bool = true;
  } else if (s_rival_3 < UMBRAL_HISTERESIS) {
    if (s_rival_3_bool) {
      s_rival_3_ms_false = millis();
    }
    s_rival_3_bool = false;
  }
  if (s_rival_4 > UMBRAL) {
    if (!s_rival_4_bool) {
      s_rival_4_ms_true = millis();
    }
    s_rival_4_bool = true;
  } else if (s_rival_4 < UMBRAL_HISTERESIS) {
    if (s_rival_4_bool) {
      s_rival_4_ms_false = millis();
    }
    s_rival_4_bool = false;
  }

  // Filtro para intentar evitar la perdida de deteccion cuando el robot rival esta mas cerca de 4cm (limite de los sensores)
  if (!s_rival_1_bool && !s_rival_2_bool && !s_rival_3_bool && !s_rival_4_bool) {
    rival_encima = false;
  } else if ((s_rival_3 > 3000) || rival_encima) {
    if (!s_rival_2_bool) {
      s_rival_2_ms_true = millis();
    }
    s_rival_2_bool = true;
    if (!s_rival_3_bool) {
      s_rival_3_ms_true = millis();
    }
    s_rival_3_bool = true;
    rival_encima = true;
  }
}

bool sensor1() {
  // Serial.printf("s_rival_1_bool: %d - last_s_rival_1_bool: %d - s_rival_1_ms_true: %ld - s_rival_1_ms_false: %ld - millis: %ld\n", s_rival_1_bool, last_s_rival_1_bool, s_rival_1_ms_true, s_rival_1_ms_false, millis());
  if (s_rival_1_bool && (millis() - s_rival_1_ms_true) > MAGNITUD_FILTRO_TEMPORAL) {
    last_s_rival_1_bool = true;
    return last_s_rival_1_bool;
  } else if (!s_rival_1_bool && (millis() - s_rival_1_ms_false) > MAGNITUD_FILTRO_TEMPORAL) {
    last_s_rival_1_bool = false;
    return last_s_rival_1_bool;
  } else {
    return last_s_rival_1_bool;
  }
}
bool sensor2() {
  if (s_rival_2_bool && (millis() - s_rival_2_ms_true) > MAGNITUD_FILTRO_TEMPORAL) {
    last_s_rival_2_bool = true;
    return last_s_rival_2_bool;
  } else if (!s_rival_2_bool && (millis() - s_rival_2_ms_false) > MAGNITUD_FILTRO_TEMPORAL) {
    last_s_rival_2_bool = false;
    return last_s_rival_2_bool;
  } else {
    return last_s_rival_2_bool;
  }
}
bool sensor3() {
  if (s_rival_3_bool && (millis() - s_rival_3_ms_true) > MAGNITUD_FILTRO_TEMPORAL) {
    last_s_rival_3_bool = true;
    return last_s_rival_3_bool;
  } else if (!s_rival_3_bool && (millis() - s_rival_3_ms_false) > MAGNITUD_FILTRO_TEMPORAL) {
    last_s_rival_3_bool = false;
    return last_s_rival_3_bool;
  } else {
    return last_s_rival_3_bool;
  }
}
bool sensor4() {
  if (s_rival_4_bool && (millis() - s_rival_4_ms_true) > MAGNITUD_FILTRO_TEMPORAL) {
    last_s_rival_4_bool = true;
    return last_s_rival_4_bool;
  } else if (!s_rival_4_bool && (millis() - s_rival_4_ms_false) > MAGNITUD_FILTRO_TEMPORAL) {
    last_s_rival_4_bool = false;
    return last_s_rival_4_bool;
  } else {
    return last_s_rival_4_bool;
  }
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
  if (!digitalRead(S_LINEA_I)) {
    contador_linea_I++;
  } else {
    contador_linea_I = 0;
  }
  if (contador_linea_I >= CONTADOR_LINEA) {
    return false;
  } else {
    return true;
  }
}
bool sensor_linea_D() {
  if (!digitalRead(S_LINEA_D)) {
    contador_linea_D++;
  } else {
    contador_linea_D = 0;
  }
  if (contador_linea_D >= CONTADOR_LINEA) {
    return false;
  } else {
    return true;
  }
}
bool boton() {
  if (digitalRead(BOTON)) {
    contador_boton++;
  } else {
    contador_boton = 0;
  }
  if (contador_boton >= 4000) {
    return true;
  } else {
    return false;
  }
}

int posicion_rival_chusta() {
  if (sensor3()) {
    posicion = 0;
  } else if (sensor1()) {
    posicion = -50;
  } else if (sensor4()) {
    posicion = 50;
  } else if (!sensor1() && !sensor3() && !sensor4()) {
    if (posicion > 0) {
      posicion = 120;
    } else if (posicion < 0) {
      posicion = -120;
    }
  }
  return posicion;
}

// No se usa actualmente
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

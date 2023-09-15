#include "debug.h"

#define MODOS_DEBUG 3
int modo_debug = 0;

void debug_inicio(){
  if (boton()) {
      modo_debug = (modo_debug + 1) % MODOS_DEBUG;
      while (boton()) {
      }
    }
    if (modo_debug == 0) {
      for (int i = 0; i < 20; i++) {
        filtro_sensores();
      }
      if (sensor1()) {
        digitalWrite(LED_IZQUIERDA, true);
      } else {
        digitalWrite(LED_IZQUIERDA, false);
      }
      if (sensor2()) {
        digitalWrite(LED_ADELANTE, true);
      } else {
        digitalWrite(LED_ADELANTE, false);
      }
      if (sensor3()) {
        digitalWrite(LED_ATRAS, true);
      } else {
        digitalWrite(LED_ATRAS, false);
      }
      if (sensor4()) {
        digitalWrite(LED_DERECHA, true);
      } else {
        digitalWrite(LED_DERECHA, false);
      }
    }
    if (modo_debug == 1) {
      if (!sensor_linea_D()) {
        digitalWrite(LED_ADELANTE, true);
        digitalWrite(LED_DERECHA, true);
        digitalWrite(LED_ATRAS, true);
        digitalWrite(LED_IZQUIERDA, false);
      } else {
        digitalWrite(LED_ADELANTE, false);
        digitalWrite(LED_DERECHA, false);
        digitalWrite(LED_ATRAS, false);
        digitalWrite(LED_IZQUIERDA, false);
      }
      if (!sensor_linea_I()) {
        digitalWrite(LED_ADELANTE, true);
        digitalWrite(LED_DERECHA, false);
        digitalWrite(LED_ATRAS, true);
        digitalWrite(LED_IZQUIERDA, true);
      } else {
        digitalWrite(LED_ADELANTE, false);
        digitalWrite(LED_DERECHA, false);
        digitalWrite(LED_ATRAS, false);
        digitalWrite(LED_IZQUIERDA, false);
      }
    }
    if (modo_debug == 2) {
      imprimir_sensores_raw();
    }
}

void imprimir_sensores_raw() {
  Serial.print(analogRead(S_RIVAL_1));
  Serial.print(" ");
  Serial.print(analogRead(S_RIVAL_2));
  Serial.print(" ");
  Serial.print(analogRead(S_RIVAL_3));
  Serial.print(" ");
  Serial.print(analogRead(S_RIVAL_4));
  Serial.print(" ");

  Serial.print(digitalRead(S_LINEA_I));
  Serial.print(" ");
  Serial.println(digitalRead(S_LINEA_D));
  delay(125);
}

void imprimir_sensores_filtrados() {
  Serial.print(sensor1());
  Serial.print(" ");
  Serial.print(sensor2());
  Serial.print(" ");
  Serial.print(sensor3());
  Serial.print(" ");
  Serial.println(sensor4());

  //delay(200);
}
void imprimir_sensores_filtrados_analog() {
  Serial.print(sensor1_analog());
  Serial.print(" ");
  Serial.print(sensor2_analog());
  Serial.print(" ");
  Serial.print(sensor3_analog());
  Serial.print(" ");
  Serial.println(sensor4_analog());

  //delay(200);
}
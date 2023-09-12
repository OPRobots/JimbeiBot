#include "debug.h"


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
  delay(200);
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
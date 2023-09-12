#include "debug.h"
#include "motores.h"
#include "pines.h"
#include "sensores.h"
#include <Arduino.h>

// PINES

// #define S_RIVAL_1 2
// #define S_RIVAL_2 4
// #define S_RIVAL_3 0
// #define S_RIVAL_4 1

// #define S_LINEA_I 3
// #define S_LINEA_D 21

// #define PWM_MD 6
// #define PWM_MI 9

// #define LED_ADELANTE 20
// #define LED_ATRAS 8
// #define LED_DERECHA 7
// #define LED_IZQUIERDA 10

// #define BOTON 5

#define NUM_ESTRATEGIAS 5

#define ESTRAT_ADELANTE 0
#define ESTRAT_DERECHA 1
#define ESTRAT_IZQUIERDA 2
#define ESTRAT_ATRAS 3
#define ESTRAT_PID 4

// Variables PID
#define TIEMPO_PID 10
#define KP 1.2
#define KD 0.8
#define KI 0
int proporcional = 0;
int derivada = 0;
int posicion_anterior = 0;
int correccion = 0;

int vel = 150;

// Otras variables

int estrategia = 0;
float millisInicio = 0;
int tiempoPulsado = 0;
int pulsa = 0;
bool inicio = false;
long millisPID = 0;
int contador = 0;

void setup() {

  inicializar_pines();
  inicializar_motores();

  // Seteamos los leds en la estrategia 0 para que de el feedback nada mas arrancar
  digitalWrite(LED_ADELANTE, true);
  digitalWrite(LED_DERECHA, true);
  digitalWrite(LED_ATRAS, false);
  digitalWrite(LED_IZQUIERDA, true);

  Serial.begin(115200);
}

void loop() {
  // imprimir_sensores_raw();

  if (boton()) {
    pulsa = millis();
    while (boton()) {
    }
    tiempoPulsado = millis() - pulsa;
  } else {
    tiempoPulsado = 0;
  }

  if (tiempoPulsado > 0) {
    if (tiempoPulsado < 350) {
      estrategia = (estrategia + 1) % NUM_ESTRATEGIAS;
      switch (estrategia) {
        case ESTRAT_ADELANTE:
          digitalWrite(LED_ADELANTE, true);
          digitalWrite(LED_DERECHA, true);
          digitalWrite(LED_ATRAS, false);
          digitalWrite(LED_IZQUIERDA, true);
          break;
        case ESTRAT_DERECHA:
          digitalWrite(LED_ADELANTE, true);
          digitalWrite(LED_DERECHA, true);
          digitalWrite(LED_ATRAS, true);
          digitalWrite(LED_IZQUIERDA, false);
          break;
        case ESTRAT_IZQUIERDA:
          digitalWrite(LED_ADELANTE, true);
          digitalWrite(LED_DERECHA, false);
          digitalWrite(LED_ATRAS, true);
          digitalWrite(LED_IZQUIERDA, true);
          break;
        case ESTRAT_ATRAS:
          digitalWrite(LED_ADELANTE, false);
          digitalWrite(LED_DERECHA, true);
          digitalWrite(LED_ATRAS, true);
          digitalWrite(LED_IZQUIERDA, true);
          break;
        case ESTRAT_PID:
          digitalWrite(LED_ADELANTE, true);
          digitalWrite(LED_DERECHA, true);
          digitalWrite(LED_ATRAS, true);
          digitalWrite(LED_IZQUIERDA, true);
          break;

        default:
          break;
      }
    } else {
      inicio = true;
      millisInicio = millis();
    }
  }

  if (inicio && (millis() - millisInicio > 5000)) {

    if (millis() >= millisPID + 1) {
      filtro_sensores();
      contador = (contador + 1) % TIEMPO_PID; // Avanza el índice circularmente cuando supera TIEMPO_PID vuelve a ser 0

      if (contador == 0) { // Este if se ejecuta una vez cada tiempo establecido en TIEMPO_PID en ms
        digitalWrite(LED_ADELANTE, false);
        // imprimir_sensores_filtrados();
        // Serial.println(posicion_rival_chusta());
        // delay(100);
        //////////////////////////////
        ////    Calculo del PID   ////
        //////////////////////////////

        proporcional = posicion_rival_chusta();
        derivada = proporcional - posicion_anterior;
        // integral = integral + (posicion / 10);
        // integral= constrain(integral,-500,500);

        correccion = ((KP * proporcional) + (KD * derivada)); // + (ki * integral));
        // Serial.println(correccion);
        posicion_anterior = proporcional;

        // aqui aplicamos la correccion del pid a las velocidades de los motores
        asignacion_vel_motores();
      }
      calculo_vel_motores(vel, correccion);

      millisPID = millis();
    }

  } else if (inicio && (millis() - millisInicio > 4000)) {

    digitalWrite(LED_ADELANTE, true);

  } else if (inicio && (millis() - millisInicio > 3000)) {

    digitalWrite(LED_ADELANTE, true);
    digitalWrite(LED_IZQUIERDA, true);

  } else if (inicio && (millis() - millisInicio > 2000)) {

    digitalWrite(LED_ADELANTE, true);
    digitalWrite(LED_ATRAS, true);
    digitalWrite(LED_IZQUIERDA, true);

  } else if (inicio && (millis() - millisInicio > 1000)) {

    digitalWrite(LED_ADELANTE, true);
    digitalWrite(LED_DERECHA, true);
    digitalWrite(LED_ATRAS, true);
    digitalWrite(LED_IZQUIERDA, true);

  } else {
    parar_motores();
  }
}

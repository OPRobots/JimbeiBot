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
#define ESTRAT_DERECHA 2
#define ESTRAT_IZQUIERDA 3
#define ESTRAT_ATRAS 1
#define ESTRAT_PID 4

// Variables PID
#define VEL_BASE 170
#define TIEMPO_PID 10
#define KP 1.2
#define KD 0.8
#define KI 0

#define COLOR 1 // 1 = NEGRO / 0 = BLANCO (BLANCO PARA COMPETIR, NEGRO PARA PRUEBAS SIN DOYHO)

int proporcional = 0;
int derivada = 0;
int posicion_anterior = 0;
int correccion = 0;

int vel = 0;

// Otras variables

int estrategia = 0;
float millisInicio = 0;
int tiempoPulsado = 0;
long pulsa = 0;
long parpadeo = 0;
bool estado = false;
bool inicio = false;
long millisPID = 0;
int contador = 0;
bool usar_PID = 0;
bool debug = false;

void setup() {

  inicializar_pines();
  if (boton()) {
    debug = true;
  } else {
    inicializar_motores();

    // Seteamos los leds en la estrategia 0 para que de el feedback nada mas arrancar
    digitalWrite(LED_ADELANTE, true);
    digitalWrite(LED_DERECHA, true);
    digitalWrite(LED_ATRAS, false);
    digitalWrite(LED_IZQUIERDA, true);
  }
  Serial.begin(115200);
}

void loop() {

  if (debug) {
    if (boton()) {
      estrategia = (estrategia + 1) % 3;
      while (boton()) {
      }
    }
    if (estrategia == 0) {
      for (int i = 0; i < 20; i++) {
        filtro_sensores_histeresis();
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
    if (estrategia == 1) {
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
    if (estrategia == 2) {
      imprimir_sensores_raw();
    }
    return;
  }

  if (boton()) {
    pulsa = millis();
    parpadeo = millis();
    while (boton()) {
      if ((millis() - pulsa) > 350) {
        if ((millis() - parpadeo) > 75) {
          parpadeo = millis();
          estado = !estado;
          digitalWrite(LED_ADELANTE, estado);
          digitalWrite(LED_DERECHA, estado);
          digitalWrite(LED_ATRAS, estado);
          digitalWrite(LED_IZQUIERDA, estado);
        }
      }
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
          vel = VEL_BASE + 100;
          digitalWrite(LED_ADELANTE, true);
          digitalWrite(LED_DERECHA, true);
          digitalWrite(LED_ATRAS, false);
          digitalWrite(LED_IZQUIERDA, true);
          break;
        case ESTRAT_DERECHA:
          vel = 0;
          correccion = 300;
          digitalWrite(LED_ADELANTE, true);
          digitalWrite(LED_DERECHA, true);
          digitalWrite(LED_ATRAS, true);
          digitalWrite(LED_IZQUIERDA, false);
          break;
        case ESTRAT_IZQUIERDA:
          vel = 0;
          correccion = -300;
          digitalWrite(LED_ADELANTE, true);
          digitalWrite(LED_DERECHA, false);
          digitalWrite(LED_ATRAS, true);
          digitalWrite(LED_IZQUIERDA, true);
          break;
        case ESTRAT_ATRAS:
          vel = 0;
          correccion = 300;
          digitalWrite(LED_ADELANTE, false);
          digitalWrite(LED_DERECHA, true);
          digitalWrite(LED_ATRAS, true);
          digitalWrite(LED_IZQUIERDA, true);
          break;
        case ESTRAT_PID:
          vel = 0;
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

      if (sensor_linea_D() == COLOR) {
        secuencia_linea_D();
        usar_PID = false;
        vel = 0;
      } else if (sensor_linea_I() == COLOR) {
        secuencia_linea_I();
        usar_PID = false;
        vel = 0;
      } else if (sensor1() || sensor2() || sensor3() || sensor4()) {
        usar_PID = true;
        vel = VEL_BASE;
      }
      contador = (contador + 1) % TIEMPO_PID; // Avanza el índice circularmente cuando supera TIEMPO_PID vuelve a ser 0

      if (contador == 0) { // Este if se ejecuta una vez cada tiempo establecido en TIEMPO_PID en ms
        digitalWrite(LED_ADELANTE, true);
        digitalWrite(LED_DERECHA, true);
        digitalWrite(LED_ATRAS, true);
        digitalWrite(LED_IZQUIERDA, true);
        // imprimir_sensores_raw();
        // imprimir_sensores_filtrados();
        // Serial.println(posicion_rival_chusta());
        // delay(100);
        //////////////////////////////
        ////    Calculo del PID   ////
        //////////////////////////////
        // Serial.println(usar_PID);
        if (usar_PID) {
          proporcional = posicion_rival_chusta();
          derivada = proporcional - posicion_anterior;

          if (proporcional == 0) {
            vel += 5;
          } else if (proporcional > 75) {
            vel = VEL_BASE;
          }

          correccion = ((KP * proporcional) + (KD * derivada)); // + (ki * integral));
          // Serial.println(correccion);
          posicion_anterior = proporcional;
        }
        // aqui aplicamos la correccion del pid a las velocidades de los motores
        asignacion_vel_motores();
      }
      calculo_vel_motores(vel, correccion);

      millisPID = millis();
    }

  } else if (inicio && (millis() - millisInicio > 4000)) {

    digitalWrite(LED_ATRAS, false);
    digitalWrite(LED_IZQUIERDA, true);

  } else if (inicio && (millis() - millisInicio > 3000)) {

    digitalWrite(LED_DERECHA, false);
    digitalWrite(LED_ATRAS, true);

  } else if (inicio && (millis() - millisInicio > 2000)) {

    digitalWrite(LED_ADELANTE, false);
    digitalWrite(LED_DERECHA, true);

  } else if (inicio && (millis() - millisInicio > 1000)) {

    digitalWrite(LED_ADELANTE, true);
    digitalWrite(LED_DERECHA, false);
    digitalWrite(LED_ATRAS, false);
    digitalWrite(LED_IZQUIERDA, false);

  } else {
    parar_motores();
  }
}

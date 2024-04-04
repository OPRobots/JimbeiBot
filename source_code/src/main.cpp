#include "debug.h"
#include "luces.h"
#include "motores.h"
#include "pines.h"
#include "sensores.h"
#include <Arduino.h>

#define NUM_ESTRATEGIAS 5

#define ESTRAT_ADELANTE 0
#define ESTRAT_DERECHA 1
#define ESTRAT_IZQUIERDA 2
#define ESTRAT_ATRAS 3
#define ESTRAT_PID 4

// Variables PID
#define VEL_BASE 250
#define TIEMPO_PID 10
#define KP 1.2
#define KD 0.8
#define KI 0

#define ANULAR_LINEA 1 // 0 = ANULADO / 1 = NO ANULDO (1 PARA COMPETIR, 0 PARA PRUEBAS SIN DOYHO)

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
  if (digitalRead(BOTON)) {
    debug = true;
    while (digitalRead(BOTON)) {
    }
  } else {
    inicializar_motores();

    // Seteamos los leds en la estrategia 0 para que de el feedback nada mas arrancar
    set_led(RGB_TOP, true);
    set_led(RGB_RIGHT, false);
    set_led(RGB_LEFT, false);
  }
  Serial.begin(115200);
}

void loop() {

  if (debug) {
    debug_inicio();
    return;
  }

  if (inicio && (millis() - millisInicio > 5000)) {

    if (millis() >= millisPID + 1) {
      filtro_sensores();

      if (!sensor_linea_D() && ANULAR_LINEA) {
        secuencia_linea_D();
        usar_PID = true;
        vel = 0;
        correccion = 0;
        posicion_anterior = 0;
      } else if (!sensor_linea_I() && ANULAR_LINEA) {
        secuencia_linea_I();
        usar_PID = true;
        vel = 0;
        correccion = 0;
        posicion_anterior = 0;
      } else if (sensor1() || sensor2() || sensor3() || sensor4()) {
        usar_PID = true;
        if (!(sensor1() && sensor4())) {
          vel = VEL_BASE;
        }
      }
      contador = (contador + 1) % TIEMPO_PID; // Avanza el índice circularmente cuando supera TIEMPO_PID vuelve a ser 0

      if (contador == 0) { // Este if se ejecuta una vez cada tiempo establecido en TIEMPO_PID en ms

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
            vel += 2;
          } else if (proporcional > 75) {
            vel = VEL_BASE;
          }

          correccion = ((KP * proporcional) + (KD * derivada)); // + (ki * integral));
          // Serial.println(correccion);
          posicion_anterior = proporcional;
        }
        // aqui aplicamos la correccion del pid a las velocidades de los motores
        if (estrategia == ESTRAT_PID) {
          vel = 0;
        }
        asignacion_vel_motores();
      }
      calculo_vel_motores(vel, correccion);

      millisPID = millis();
    }

  } else if (inicio && (millis() - millisInicio > 4000)) {

    set_led(RGB_TOP, true);
    set_led(RGB_RIGHT, true);
    set_led(RGB_LEFT, true);

  } else if (inicio && (millis() - millisInicio > 3000)) {

    set_led(RGB_TOP, true);
    set_led(RGB_RIGHT, true);
    set_led(RGB_LEFT, false);

  } else if (inicio && (millis() - millisInicio > 2000)) {

    set_led(RGB_TOP, true);
    set_led(RGB_RIGHT, false);
    set_led(RGB_LEFT, false);

  } else if (inicio && (millis() - millisInicio > 1000)) {

    set_led(RGB_TOP, false);
    set_led(RGB_RIGHT, false);
    set_led(RGB_LEFT, false);

  } else {
    parar_motores();

    if (boton()) {
      pulsa = millis();
      parpadeo = millis();
      while (boton()) {
        if ((millis() - pulsa) > 350) {
          rainbow_led(RGB_TOP);
          rainbow_led(RGB_RIGHT);
          rainbow_led(RGB_LEFT);
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
            vel = VEL_BASE + 65;
            set_led(RGB_TOP, true);
            set_led(RGB_RIGHT, false);
            set_led(RGB_LEFT, false);
            break;
          case ESTRAT_DERECHA:
            vel = 0;
            correccion = 180;
            set_led(RGB_TOP, false);
            set_led(RGB_RIGHT, true);
            set_led(RGB_LEFT, false);
            break;
          case ESTRAT_IZQUIERDA:
            vel = 0;
            correccion = -180;
            set_led(RGB_TOP, false);
            set_led(RGB_RIGHT, false);
            set_led(RGB_LEFT, true);
            break;
          case ESTRAT_ATRAS:
            vel = 0;
            correccion = 280;
            set_led(RGB_TOP, false);
            set_led(RGB_RIGHT, true);
            set_led(RGB_LEFT, true);
            break;
          case ESTRAT_PID:
            vel = 0;
            set_led(RGB_TOP, true);
            set_led(RGB_RIGHT, true);
            set_led(RGB_LEFT, true);
            break;

          default:
            break;
        }
      } else {
        inicio = true;
        millisInicio = millis();
      }
    }
  }
}

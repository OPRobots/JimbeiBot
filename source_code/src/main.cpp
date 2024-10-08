#include "control.h"
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
#define VEL_BASE 300
#define TIEMPO_PID 4
#define KP 3
#define KD 6.0
#define KI 0

#define ANULAR_LINEA 1 // 0 = ANULADO / 1 = NO ANULADO (1 PARA COMPETIR, 0 PARA PRUEBAS SIN DOYHO)

int proporcional = 0;
int derivada = 0;
int posicion_anterior = 0;
int correccion = 0;

int vel = 0;

// Otras variables

int estrategia = 0;
int tiempoPulsado = 0;
long pulsa = 0;
long parpadeo = 0;
bool estado = false;
bool inicio = false;
long millisPID = 0;
int contador = 0;
bool usar_PID = 0;
bool debug = false;
bool estrategiaRealizada = false;
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
  //test mando y leds
  

  if (debug) {
    filtro_sensores();
    debug_inicio();
    return;
  }

  if ((is_started() && (millis() - ms_started() > 5000)) && is_started()) {

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
        if (!sensor3()) {
          vel = VEL_BASE;
        }
      }
      if (!estrategiaRealizada) {
        estrategiaRealizada = true;
        switch (estrategia) {
          case ESTRAT_ADELANTE:
            vel = VEL_BASE + 65;
            delay(20);
            break;
          case ESTRAT_DERECHA:
            arranque_derecha();
            break;
          case ESTRAT_IZQUIERDA:
            arranque_izquierda();
            break;
          case ESTRAT_ATRAS:
            arranque_espaldas();
            break;
        }
      }
      contador = (contador + 1) % TIEMPO_PID; // Avanza el índice circularmente cuando supera TIEMPO_PID vuelve a ser 0

      if (contador == 0) { // Este if se ejecuta una vez cada tiempo establecido en TIEMPO_PID en ms

        // imprimir_sensores_raw();
        // imprimir_sensores_filtrados();
        // Serial.println(posicion_rival_chusta());
        // delay(100);
        // return;
        //////////////////////////////
        ////    Calculo del PID   ////
        //////////////////////////////
        // Serial.println(usar_PID);
        if (usar_PID) {
          proporcional = posicion_rival_chusta();
          derivada = proporcional - posicion_anterior;

          if (proporcional == 0) {
            //vel += 2;
            if (vel > 750) {
              vel = VEL_BASE;
            }
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
        //Serial.println(correccion);
        //return;
        asignacion_vel_motores();
      }
      calculo_vel_motores(vel, correccion);

      millisPID = millis();
    }

  } else if (is_started() && (millis() - ms_started() > 4000)) {

    set_led(RGB_TOP, true);
    set_led(RGB_RIGHT, true);
    set_led(RGB_LEFT, true);

  } else if (is_started() && (millis() - ms_started() > 3000)) {

    set_led(RGB_TOP, true);
    set_led(RGB_RIGHT, true);
    set_led(RGB_LEFT, false);

  } else if (is_started() && (millis() - ms_started() > 2000)) {

    set_led(RGB_TOP, true);
    set_led(RGB_RIGHT, false);
    set_led(RGB_LEFT, false);

  } else if (is_started() && (millis() - ms_started() > 1000)) {

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
           switch (estrategia) {
          case ESTRAT_ADELANTE:
            set_led_confirmado(RGB_TOP, true);
            set_led_confirmado(RGB_RIGHT, false);
            set_led_confirmado(RGB_LEFT, false);
            break;
          case ESTRAT_DERECHA:
            set_led_confirmado(RGB_TOP, false);
            set_led_confirmado(RGB_RIGHT, true);
            set_led_confirmado(RGB_LEFT, false);
            break;
          case ESTRAT_IZQUIERDA:
            set_led_confirmado(RGB_TOP, false);
            set_led_confirmado(RGB_RIGHT, false);
            set_led_confirmado(RGB_LEFT, true);
            break;
          case ESTRAT_ATRAS:
            set_led_confirmado(RGB_TOP, false);
            set_led_confirmado(RGB_RIGHT, true);
            set_led_confirmado(RGB_LEFT, true);
            break;
          case ESTRAT_PID:
            vel = VEL_BASE / 2.0;
            set_led_confirmado(RGB_TOP, true);
            set_led_confirmado(RGB_RIGHT, true);
            set_led_confirmado(RGB_LEFT, true);
            break;

          default:
            break;
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
            vel = VEL_BASE + 65;
            set_led(RGB_TOP, true);
            set_led(RGB_RIGHT, false);
            set_led(RGB_LEFT, false);
            break;
          case ESTRAT_DERECHA:
            set_led(RGB_TOP, false);
            set_led(RGB_RIGHT, true);
            set_led(RGB_LEFT, false);
            break;
          case ESTRAT_IZQUIERDA:
            set_led(RGB_TOP, false);
            set_led(RGB_RIGHT, false);
            set_led(RGB_LEFT, true);
            break;
          case ESTRAT_ATRAS:
            set_led(RGB_TOP, false);
            set_led(RGB_RIGHT, true);
            set_led(RGB_LEFT, true);
            break;
          case ESTRAT_PID:
            vel = VEL_BASE / 2.0;
            set_led(RGB_TOP, true);
            set_led(RGB_RIGHT, true);
            set_led(RGB_LEFT, true);
            break;

          default:
            break;
        }
      } else if (tiempoPulsado >= 350 || is_started()){
        set_starting(true);
        start(millis());
      }
    }
  }
}

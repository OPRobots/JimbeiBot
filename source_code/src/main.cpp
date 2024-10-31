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
#define VEL_BASE 0
#define VEL_SUCCION 100
#define TIEMPO_PID 4
#define KP 3
#define KD 6.0
#define KI 0
#define ANULAR_LINEA 0 // 1 = ANULADO / 0 = NO ANULADO (0 PARA COMPETIR, 1 PARA PRUEBAS SIN DOYHO)

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
  // test mando y leds

  if (debug) {
    filtro_sensores();
    debug_inicio();
    return;
  }
  if ((is_started() && (millis() - ms_started() > 0)) && is_started()) {

    // ledcWrite(PWM_SERVO, 190);
    // set_fan_speed(VEL_SUCCION);
    // set_motors_speed(100, 100);

    if (millis() >= millisPID + 1) {
      filtro_sensores();

      ledcWrite(PWM_SERVO, 190);
      set_fan_speed(VEL_SUCCION);

      if (!sensor_linea_D() && !ANULAR_LINEA) {
        set_motors_speed(-100, -100);
        delay(500);
        set_motors_speed(-100, 100);
        delay(500);
      } else if (!sensor_linea_I() && !ANULAR_LINEA) {
        set_motors_speed(-100, -100);
        delay(500);
        set_motors_speed(100, -100);
        delay(500);
      } else if (sensor1() && sensor2()) {
        set_motors_speed(100, 100);
      } else if (sensor1()) {
        set_motors_speed(-100, 100);
      } else if (sensor2()) {
        set_motors_speed(100, -100);
      } else {
        set_motors_speed(100, -100);
      }

      millisPID = millis();
    }

    return;

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
    set_fan_speed(10);
  }
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
    } else if (tiempoPulsado >= 350 || is_started()) {
      set_starting(true);
      start(millis());
    }
  }
}

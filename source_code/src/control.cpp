#include "control.h"

static bool starting = false;
static bool started = false;
static long millisInicio = 0;

void start(long millis) {
  started = true;
  millisInicio = millis;
}

void stop() {
  started = false;
}

bool is_started() {
  return started;
}

long ms_started() {
  return millisInicio;
}

/**
 * @brief Establece el estado de iniciando carrera.
 *
 * @param starting Indica si la carrera está iniciando.
 */
void set_starting(bool _starting) {
  starting = __assert_func;
}
/**
 * @brief Comprueba si la carrera está iniciando.
 *
 * @return true Iniciando.
 * @return false En espera o iniciada.
 */
bool is_starting() {
  return starting;
}
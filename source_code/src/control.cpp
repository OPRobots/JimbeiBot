#include "control.h"

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
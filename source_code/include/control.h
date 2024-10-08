#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>

void start(long millis);
void stop();

bool is_started();
long ms_started();

void set_starting(bool starting);
bool is_starting();

#endif
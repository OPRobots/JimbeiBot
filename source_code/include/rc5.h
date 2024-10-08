#ifndef RC5_H
#define RC5_H

#include <Arduino.h>
#include <control.h>
#include <pines.h>
#include <stdint.h>

enum RC5_TRIGGER {
  RC5_TRIGGER_FALLING,
  RC5_TRIGGER_RISING,
};

#define RC5_DATA_LENGTH 2

void rc5_register(enum RC5_TRIGGER trigger);
void rc5_isr(void);

#endif
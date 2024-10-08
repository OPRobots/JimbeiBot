#ifndef __LUCES_H
#define __LUCES_H
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <pines.h>

enum LEDS {
  RGB_LEFT = 2,
  RGB_TOP = 1,
  RGB_RIGHT = 0
};

void set_led(enum LEDS led, bool on);
void set_led_confirmado(enum LEDS led, bool on);
void clear_led(enum LEDS led);

void blink_led(enum LEDS led, int r, int g, int b, int ms);
void rainbow_led(enum LEDS led);

#endif
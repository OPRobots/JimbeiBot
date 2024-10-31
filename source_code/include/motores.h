#ifndef __MOTORES_H
#define __MOTORES_H
#include <Arduino.h>
#include "pines.h"
#include "sensores.h"



/**
 * @brief Configuración de los motores de tracción.
 * Canales: 0 a 3
 * Frecuencia: 1 kHz
 * Resolución: 10 bits
 * Rango: 0 a 1023
 *
 */
#define PWM_MOTOR_RIGHT_A 0
#define PWM_MOTOR_RIGHT_B 1
#define PWM_MOTOR_LEFT_A 2
#define PWM_MOTOR_LEFT_B 3
#define PWM_MOTORS_HZ 1000
#define PWM_MOTORS_RESOLUTION 10
#define PWM_MOTORS_MAX 1023
#define PWM_MOTORS_MIN 0

/**
 * @brief Configuración del motor de succión.
 * Canal: 4
 * Frecuencia: 50 Hz
 * Resolución: 11 bits
 * Rango: 102 a 204 (1000us a 2000us del Protocolo Servo)
 *
 */
#define PWM_SUCTION 4
#define PWM_SUCTION_HZ 50
#define PWM_SUCTION_RESOLUTION 11
#define PWM_SUCTION_MAX 204
#define PWM_SUCTION_MIN 102
/**
 * @brief Configuración del motor de succión.
 * Canal: 4
 * Frecuencia: 50 Hz
 * Resolución: 11 bits
 * Rango: 102 a 204 (1000us a 2000us del Protocolo Servo)
 *
 */
#define PWM_SERVO 5
#define PWM_SERVO_HZ 50
#define PWM_SERVO_RESOLUTION 11
#define PWM_SERVO_MAX 204
#define PWM_SERVO_MIN 102



void inicializar_motores();
void set_motors_speed(float, float);
void set_fan_speed(int);
void asignacion_vel_motores();
void parar_motores();
void secuencia_linea_D();
void secuencia_linea_I();
void arranque_derecha();
void arranque_izquierda();
void arranque_espaldas();

#endif
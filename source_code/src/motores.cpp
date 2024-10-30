#include "motores.h"

#define UMBRAL_COGGIN 20

const int MAGNITUD_FILTRO = 10;

int Mot_D = 0;
int Mot_I = 1;

int Vel_D = 0;
int Vel_I = 0;

int Vel_D_anterior = 0;
int Vel_I_anterior = 0;

int Filtro_MD[MAGNITUD_FILTRO];
int Filtro_MI[MAGNITUD_FILTRO];

int i_m = 0;

void inicializar_motores() {
  // Configuración de los canales PWM del Timer
  ledcSetup(PWM_MOTOR_RIGHT_A, PWM_MOTORS_HZ, PWM_MOTORS_RESOLUTION);
  ledcSetup(PWM_MOTOR_RIGHT_B, PWM_MOTORS_HZ, PWM_MOTORS_RESOLUTION);
  ledcSetup(PWM_MOTOR_LEFT_A, PWM_MOTORS_HZ, PWM_MOTORS_RESOLUTION);
  ledcSetup(PWM_MOTOR_LEFT_B, PWM_MOTORS_HZ, PWM_MOTORS_RESOLUTION);
  ledcSetup(PWM_SUCTION, PWM_SUCTION_HZ, PWM_SUCTION_RESOLUTION);

  // Asignación de los pines a los canales PWM
  ledcAttachPin(MOTOR_RIGHT_A, PWM_MOTOR_RIGHT_A);
  ledcAttachPin(MOTOR_RIGHT_B, PWM_MOTOR_RIGHT_B);
  ledcAttachPin(MOTOR_LEFT_A, PWM_MOTOR_LEFT_A);
  ledcAttachPin(MOTOR_LEFT_B, PWM_MOTOR_LEFT_B);
  ledcAttachPin(MOTOR_SUCTION, PWM_SUCTION);

  // Establece el valor inicial de los canales PWM
  ledcWrite(PWM_MOTOR_RIGHT_A, PWM_MOTORS_MIN);
  ledcWrite(PWM_MOTOR_RIGHT_B, PWM_MOTORS_MIN);
  ledcWrite(PWM_MOTOR_LEFT_A, PWM_MOTORS_MIN);
  ledcWrite(PWM_MOTOR_LEFT_B, PWM_MOTORS_MIN);
  ledcWrite(PWM_SUCTION, PWM_SUCTION_MIN);

  // Tiempo de espera para inicialización del ESC; se puede comentar si se espera manualmente (calibrando, ajustando velocidad, etc.)
   delay(10000);
}

void calculo_vel_motores(int vel, int correccion) {

  int aux_Vel_D = 0;
  int aux_Vel_I = 0;

  if (vel == 0 && correccion == 0) {
    Vel_D = 0;
    Vel_I = 0;
    ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
    ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  } else {

    Vel_D = vel - correccion;
    Vel_I = vel + correccion;
  }
}

/**
 * @brief Establece la velocidad de los motores
 *
 * @param velI Velocidad del motor izquierdo 0-100%
 * @param velD Velocidad del motor derecho 0-100%
 */
void set_motors_speed(float velI, float velD) {
  if (velI > 100) {
    velI = 100;
  } else if (velI < -100) {
    velI = -100;
  }

  if (velI > 0) {
    ledcWrite(PWM_MOTOR_LEFT_A, PWM_MOTORS_MAX - (PWM_MOTORS_MAX * velI / 100));
    ledcWrite(PWM_MOTOR_LEFT_B, PWM_MOTORS_MAX);
  } else {
    ledcWrite(PWM_MOTOR_LEFT_A, PWM_MOTORS_MAX);
    ledcWrite(PWM_MOTOR_LEFT_B, PWM_MOTORS_MAX - (PWM_MOTORS_MAX * abs(velI) / 100));
  }

  if (velD > 100) {
    velD = 100;
  } else if (velD < -100) {
    velD = -100;
  }

  if (velD > 0) {
    ledcWrite(PWM_MOTOR_RIGHT_A, PWM_MOTORS_MAX);
    ledcWrite(PWM_MOTOR_RIGHT_B, PWM_MOTORS_MAX - (PWM_MOTORS_MAX * velD / 100));
  } else {
    ledcWrite(PWM_MOTOR_RIGHT_A, PWM_MOTORS_MAX - (PWM_MOTORS_MAX * abs(velD) / 100));
    ledcWrite(PWM_MOTOR_RIGHT_B, PWM_MOTORS_MAX);
  }
}
void parar_motores() {

  digitalWrite(MOTOR_RIGHT_A, HIGH);
  digitalWrite(MOTOR_RIGHT_B, HIGH);
  digitalWrite(MOTOR_LEFT_A, HIGH);
  digitalWrite(MOTOR_LEFT_B, HIGH);
}

/**
 * @brief Establece la velocidad del ventilador
 *
 * @param vel Velocidad del ventilador 0-100%
 */
void set_fan_speed(int vel) {
  if (vel != 0) {
    ledcWrite(PWM_SUCTION, map(vel, 0, 100, PWM_SUCTION_MIN, PWM_SUCTION_MAX));
  } else {
    ledcWrite(PWM_SUCTION, PWM_SUCTION_MIN);
  }
}

void secuencia_linea_D() {
  // Vel_D = 0;
  // Vel_I = 0;
  // ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  // ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  // delay(20);
  Vel_D = -750;
  Vel_I = -750;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(175);
  Vel_D = 750;
  Vel_I = -750;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(175);
  Vel_D = 250;
  Vel_I = 250;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
}
void secuencia_linea_I() {
  // Vel_D = 0;
  // Vel_I = 0;
  // ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  // ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  // delay(20);
  Vel_D = -750;
  Vel_I = -750;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(175);
  Vel_D = -750;
  Vel_I = 750;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(175);
  Vel_D = 250;
  Vel_I = 250;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
}
void arranque_derecha() {
  Vel_D = -750;
  Vel_I = 750;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(225);
  Vel_D = 500;
  Vel_I = 500;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(200);
  Vel_D = 250;
  Vel_I = 250;
  // ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  // ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  // delay(1000);
}
void arranque_izquierda() {
  Vel_D = 750;
  Vel_I = -750;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(225);
  Vel_D = 500;
  Vel_I = 500;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(200);
  Vel_D = 250;
  Vel_I = 250;
  // ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  // ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  // delay(1000);
}
void arranque_espaldas() {
  Vel_D = -750;
  Vel_I = 750;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(325);
  Vel_D = 500;
  Vel_I = 500;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(200);
  Vel_D = 250;
  Vel_I = 250;
  // ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  // ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  // delay(1000);
}
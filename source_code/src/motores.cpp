#include "motores.h"

#define UMBRAL_COGGIN 120

const int MAGNITUD_FILTRO = 30;

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
  ledcSetup(0, 4000, 11); // Canal de PWM (0 - 16),Frecuencia, Resolucion (nº bits)
  ledcAttachPin(PWM_MD, Mot_D);
  ledcAttachPin(PWM_MI, Mot_I);

  ledcWrite(Mot_D, 1536); // 1536 es 75% PWM, la mitad entre 1024(50% pwm) y 2048(100% pwm)
  ledcWrite(Mot_I, 1536);
  delay(2000);
}

void calculo_vel_motores(int vel, int correccion) {

  int aux_Vel_D = 0;
  int aux_Vel_I = 0;

  if (vel == 0) {
    Vel_D = 0;
    Vel_I = 0;
    ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
    ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  } else {

    Vel_D = vel - correccion;
    Vel_I = vel + correccion;

    Filtro_MD[i_m] = Vel_D;
    Filtro_MI[i_m] = Vel_I;
    i_m = (i_m + 1) % MAGNITUD_FILTRO; // Avanza el índice circularmente cuando supera MAGNITUD FILTRO vuelve a ser 0

    for (int i = 0; i < MAGNITUD_FILTRO; i++) {
      aux_Vel_D += Filtro_MD[i];
      aux_Vel_I += Filtro_MI[i];
    }

    Vel_D = aux_Vel_D / MAGNITUD_FILTRO;
    Vel_I = aux_Vel_I / MAGNITUD_FILTRO;
  }
}

void asignacion_vel_motores() {

Vel_D = (Vel_D + Vel_D_anterior)/2;
Vel_I = (Vel_I + Vel_I_anterior)/2;


  // de tal modo que si la correccion es muy baja le aplique el minimo de velocidad al motor para evitar coggin
  if (Vel_D > (UMBRAL_COGGIN / 2) && Vel_D < UMBRAL_COGGIN) {
    Vel_D = UMBRAL_COGGIN;
  } else if (Vel_D < -(UMBRAL_COGGIN / 2) && Vel_D > -(UMBRAL_COGGIN + 15)) {
    Vel_D = -(UMBRAL_COGGIN + 15);
  } else if (Vel_D > -(UMBRAL_COGGIN / 2) && Vel_D < (UMBRAL_COGGIN / 2)) {
    Vel_D = 0;
  }

  if (Vel_I > (UMBRAL_COGGIN / 2) && Vel_I < UMBRAL_COGGIN) {
    Vel_I = UMBRAL_COGGIN;
  } else if (Vel_I < -(UMBRAL_COGGIN / 2) && Vel_I > -(UMBRAL_COGGIN + 15)) {
    Vel_I = -(UMBRAL_COGGIN + 15);
  } else if (Vel_I > -(UMBRAL_COGGIN / 2) && Vel_I < (UMBRAL_COGGIN / 2)) {
    Vel_I = 0;
  }

  // de tal modo que si la correccion hace que una rueda se ponga a mas de 255, se limita a 255 y a la otra se le aplique la correccion restante
  if (Vel_D > 500) {
    Vel_I = Vel_I - (Vel_D - 500);
    Vel_D = 500;
  }
  if (Vel_I > 500) {
    Vel_D = Vel_D - (Vel_I - 500);
    Vel_I = 500;
  }

  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));

  Vel_D_anterior = Vel_D;
  Vel_I_anterior = Vel_I;

  // Serial.print(map(Vel_D, -1000, 1000, 1024, 2048));
  // Serial.print(" ");
  // Serial.println(map(Vel_I, -1000, 1000, 1024, 2048));
  Serial.print(Vel_D);
  Serial.print(" ");
  Serial.println(Vel_I);
  // delay(100);
}
void parar_motores() {

  Vel_D = 0;
  Vel_I = 0;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
}

void secuencia_linea_D() {
  Vel_D = 0;
  Vel_I = 0;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(50);
  Vel_D = -250;
  Vel_I = -250;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(150);
  Vel_D = 200;
  Vel_I = -200;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(300);
  Vel_D = 0;
  Vel_I = 0;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
}
void secuencia_linea_I() {
  Vel_D = 0;
  Vel_I = 0;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(50);
  Vel_D = -250;
  Vel_I = -250;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(150);
  Vel_D = -200;
  Vel_I = 200;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
  delay(300);
  Vel_D = 0;
  Vel_I = 0;
  ledcWrite(Mot_D, map(Vel_D, -1000, 1000, 1024, 2048));
  ledcWrite(Mot_I, map(Vel_I, -1000, 1000, 1024, 2048));
}
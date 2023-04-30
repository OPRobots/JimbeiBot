

#include <TimerOne.h>
//#include <Servo.h>


#define START 2
#define LED 13
#define SW1 A2
#define SW2 A3
#define SENSOR_LINEA_DERECHO 7
#define SENSOR_LINEA_IZQUIERDO 6

#define MOTOR_DERECHO 9
#define MOTOR_IZQUIERDO 10

#define SENSOR_DERECHO A1
#define SENSOR_IZQUIERDO A0

#define ESTRATEGIA_FRENTE 0
#define ESTRATEGIA_RIVAL_DERECHO 1
#define ESTRATEGIA_RIVAL_IZQUIERDO 2
#define ESTRATEGIA_ESPALDA 3

long millisInicio = 0;
long millisMaxBusqueda = 1500;

/*
   VARIABLES ESTRATEGIAS
*/
int interruptores[] = {SW1, SW2};
int lectura_sw[] = {0, 0};
int estrategia = 0;

/*
   VARIABLES SENSORES RIVAL
*/
int enemigo_D = 0;
int enemigo_I = 0;
int adelante_D = 0;
int adelante_I = 0;

/*
   VARIABLES DE ARRANQUE
*/
bool competicionIniciada = false;
long millisArranque = 5000;

/*
   VARIABLES DE VELOCIDAD
*/
int vel_base = 825;
int vel_max = 900;
int vel_robot_D = 0;
int vel_robot_I = 0;

int giroInicial = true;

//Servo motorDerecho;
//Servo motorIzquierdo;

void setup() {
  pinMode(MOTOR_DERECHO, OUTPUT);
  pinMode(MOTOR_IZQUIERDO, OUTPUT);

  pinMode(SENSOR_DERECHO, INPUT);
  pinMode(SENSOR_IZQUIERDO, INPUT);

  pinMode(SENSOR_LINEA_DERECHO, INPUT);
  pinMode(SENSOR_LINEA_IZQUIERDO, INPUT);

  pinMode(START, INPUT_PULLUP);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);

  pinMode(LED, OUTPUT);

  Serial.begin(9600);

  //  motorDerecho.attach(MOTOR_DERECHO);
  //  motorIzquierdo.attach(MOTOR_IZQUIERDO);
  //  motorDerecho.writeMicroseconds(2000);
  //  motorIzquierdo.writeMicroseconds(2000);
  //  delay(5000);
  //  motorDerecho.writeMicroseconds(1500);
  //  motorIzquierdo.writeMicroseconds(1500);
  //  delay(5000);

  Timer1.initialize(250);

  Timer1.pwm(MOTOR_DERECHO, 1023);
  Timer1.pwm(MOTOR_IZQUIERDO, 1023);
  delay(2500);
  Timer1.pwm(MOTOR_DERECHO, 768);
  Timer1.pwm(MOTOR_IZQUIERDO, 768);
  //delay(1000);

  /*
     Lectura de estrategia
  */
  estrategia = !digitalRead(interruptores[0]) + !digitalRead(interruptores[1])*2;
  Serial.println(estrategia);
}

void loop() {
  //delay(50);
  // comprobar_pines();
  // return;

  if (!competicionIniciada) {

    digitalWrite(LED, HIGH); //esta listo
    Timer1.pwm(MOTOR_DERECHO, 768);
    Timer1.pwm(MOTOR_IZQUIERDO, 768);
    if (digitalRead(START)) {
      delay(millisArranque);
      competicionIniciada = true;
      millisInicio = millis();
    }
  }
  else if(digitalRead(START)) {
    digitalWrite(LED, LOW);

    adelante_I = 1023 - analogRead(SENSOR_LINEA_IZQUIERDO);
    adelante_D = 1023 - analogRead(SENSOR_LINEA_DERECHO);
    enemigo_D = analogRead(SENSOR_DERECHO);
    enemigo_I = analogRead(SENSOR_IZQUIERDO);

    switch (estrategia) {

      case ESTRATEGIA_FRENTE:
        estrategiaBase();
        break;

      case ESTRATEGIA_RIVAL_DERECHO:
        if(giroInicial){
          Timer1.pwm(MOTOR_DERECHO, 600);
          Timer1.pwm(MOTOR_IZQUIERDO, 836);
          delay(400);
          Timer1.pwm(MOTOR_DERECHO, 768);
          Timer1.pwm(MOTOR_IZQUIERDO, 768);
          giroInicial = false;
        }
        estrategiaBase();
        break;

      case ESTRATEGIA_RIVAL_IZQUIERDO:
        if(giroInicial){
          Timer1.pwm(MOTOR_DERECHO, 836);
          Timer1.pwm(MOTOR_IZQUIERDO, 600);
          delay(400);
          Timer1.pwm(MOTOR_DERECHO, 768);
          Timer1.pwm(MOTOR_IZQUIERDO, 768);
          giroInicial = false;
        }
        estrategiaBase();
        break;

      case ESTRATEGIA_ESPALDA:
        if(giroInicial){
          Timer1.pwm(MOTOR_DERECHO, 600);
          Timer1.pwm(MOTOR_IZQUIERDO, 836);
          delay(550);
          Timer1.pwm(MOTOR_DERECHO, 768);
          Timer1.pwm(MOTOR_IZQUIERDO, 768);
          giroInicial = false;
        }
        estrategiaBase();
        break;

      default:
        break;
    }
  }else{
    digitalWrite(LED, LOW); //esta listo
    Timer1.pwm(MOTOR_DERECHO, 768);
    Timer1.pwm(MOTOR_IZQUIERDO, 768);
  }
}

void estrategiaBase(){
  if ((adelante_I >= 850 && adelante_I < 1010) || (adelante_D >= 850 && adelante_D < 1010) ) { //Si detectas borde retrocede y gira
  
    Timer1.pwm(MOTOR_DERECHO, 520);
    Timer1.pwm(MOTOR_IZQUIERDO, 520);
    delay(300);
  
    Timer1.pwm(MOTOR_DERECHO, 600);
    Timer1.pwm(MOTOR_IZQUIERDO, 836);
    delay(350);
  
    Timer1.pwm(MOTOR_DERECHO, 785);
    Timer1.pwm(MOTOR_IZQUIERDO, 785);
    delay(100);
  }
  else if (enemigo_I > 250 && enemigo_D > 250) { //Si detectas enemigo acelera de frente
    Timer1.pwm(MOTOR_DERECHO, vel_max);
    Timer1.pwm(MOTOR_IZQUIERDO, vel_max);
    delay(100);
  }
  else if (enemigo_I > 185) { //si el enemigo esta por el lado, gira
    Timer1.pwm(MOTOR_DERECHO, 815);
    Timer1.pwm(MOTOR_IZQUIERDO, 785);
    delay(100);
  }
  else if (enemigo_D > 185) { //si el enemigo esta por el lado, gira
    Timer1.pwm(MOTOR_DERECHO, 785);
    Timer1.pwm(MOTOR_IZQUIERDO, 815);
    delay(100);
  } else {
    Timer1.pwm(MOTOR_DERECHO, 785);
    Timer1.pwm(MOTOR_IZQUIERDO, 785);
    delay(100);
  }
}

void comprobar_pines() {
  Serial.print("SRD:" );
  Serial.print(analogRead(SENSOR_DERECHO));
  Serial.print("\tSRI:" );
  Serial.print(analogRead(SENSOR_IZQUIERDO));
  Serial.print("\tSLD:" );
  Serial.print(analogRead(SENSOR_LINEA_DERECHO));
  Serial.print("\tSLI:" );
  Serial.print(analogRead(SENSOR_LINEA_IZQUIERDO));
  Serial.print("\tSW1:" );
  Serial.print(digitalRead(SW1));
  Serial.print("\tSW2:" );
  Serial.print(digitalRead(SW2));
  Serial.print("\tBTN:" );
  Serial.print(digitalRead(START));
  Serial.println();

  //Timer1.pwm(MOTOR_DERECHO, 1023);
  //Timer1.pwm(MOTOR_IZQUIERDO, 1023);
}

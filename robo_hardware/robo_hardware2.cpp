#include "robo_hardware2.h"
#include <math.h>

Servo robo_hardware::servoGarra1;
Servo robo_hardware::servoGarra2;

robo_hardware::robo_hardware():	
  corDireita(SENSOR_COR_DIR_S2,SENSOR_COR_DIR_S3,SENSOR_COR_DIR_OUT),
  corEsquerda(SENSOR_COR_ESQ_S2,SENSOR_COR_ESQ_S3,SENSOR_COR_ESQ_OUT),
  sonarFrontal(SONAR_TRIGGER_FRONTAL, SONAR_ECHO_FRONTAL),
  sonarLateralEsquerdo(SONAR_TRIGGER_LADO_ESQUERDO, SONAR_ECHO_LADO_ESQUERDO), 
  sonarLateralDireito(SONAR_TRIGGER_LADO_DIREITO,SONAR_ECHO_LADO_DIREITO)
{
}

void robo_hardware::tensao(float valor_por_cento,int pino){
  float k = 255/100.0;
  if(valor_por_cento < -100) 
	valor_por_cento = -100;	//se o valor passado em valor_por_cento for menor que -100 obriga-se o mmotor a ficar em -100
  if(valor_por_cento >  100) 
	valor_por_cento =  100;	//se o valor passado em valor_por_cento for maior que 100 obriga-se o mmotor a ficar em 100

  float valor_ate_255 = valor_por_cento * k;
  analogWrite(pino, abs(valor_ate_255)); 
}


void robo_hardware::configurar(bool habilitar_garra){
  Serial.begin(9600);

  //Com essas funcoes os sonares sao calibrados 
  //Quanto maior o valor de CALIBRACAO_SONAR menor a inclinacao da curva de calibracao 
  sonarFrontal.setDivisor(CALIBRACAO_SONAR, Ultrasonic::CM);  
  sonarLateralDireito.setDivisor(CALIBRACAO_SONAR, Ultrasonic::CM);
  sonarLateralEsquerdo.setDivisor(CALIBRACAO_SONAR, Ultrasonic::CM);
  
  if(habilitar_garra){
	servoGarra1.attach(SERVO_GARRA_1);
	servoGarra2.attach(SERVO_GARRA_2);
  }

  //pinMode(BOTAO, INPUT_PULLUP);
  pinMode(PWM_RODA_ESQUERDA, OUTPUT); 
  pinMode(PWM_RODA_DIREITA, OUTPUT); 
  pinMode(SENTIDO_RODA_ESQUERDA, OUTPUT); 
  pinMode(SENTIDO_RODA_DIREITA, OUTPUT);
}

//boolean robo_hardware::lerSensorFimDeCurso(){
//  return digitalRead(BOTAO); 
//}

float robo_hardware::lerSensorDeLinha(int sensor){
  return (100 - 100.0 * (analogRead(sensor))/1023.0);;
}

void robo_hardware::acionarMotores(float motor1, float motor2){
  if(motor1 < 0){
	digitalWrite(SENTIDO_RODA_ESQUERDA, false);
	tensao(motor1,PWM_RODA_ESQUERDA);  
  }else{
 	digitalWrite(SENTIDO_RODA_ESQUERDA, true);
	motor1 = 100 - motor1;
	tensao(motor1,PWM_RODA_ESQUERDA);  
  }
  
  if(motor2 < 0){
	digitalWrite(SENTIDO_RODA_DIREITA, false);
    tensao(motor2,PWM_RODA_DIREITA);  
  }else{
	digitalWrite(SENTIDO_RODA_DIREITA, true);
	motor2 = 100 - motor2;
	tensao(motor2,PWM_RODA_DIREITA);  
  } 
}

float robo_hardware::lerSensorSonarFrontal(){
  long microsec = sonarFrontal.timing();
  return sonarFrontal.convert(microsec, Ultrasonic::CM);  //retorna a distância do sensor ao obstáculo em cm.
}

float robo_hardware::lerSensorSonarLateralEsquerdo(){
  long microsec = sonarLateralEsquerdo.timing();
  return sonarLateralEsquerdo.convert(microsec, Ultrasonic::CM);  //retorna a distância do sensor ao obstáculo em cm.
}

float robo_hardware::lerSensorSonarLateralDireito(){
  long microsec = sonarLateralDireito.timing();
  return sonarLateralDireito.convert(microsec, Ultrasonic::CM);  //retorna a distância do sensor ao obstáculo em cm.
}

void robo_hardware::acionarServoGarra1(float angulo){
  servoGarra1.write(angulo);
}

void robo_hardware::acionarServoGarra2(float angulo){
  servoGarra2.write(angulo);
}

HSV robo_hardware::getHSVEsquerdo(){
  return corEsquerda.getHSV();
}

HSV robo_hardware::getHSVDireito(){
  return corDireita.getHSV();
}

RGB robo_hardware::getRGBEsquerdo(){
  return corEsquerda.getRGB();
}

RGB robo_hardware::getRGBDireito(){
  return corDireita.getRGB();
}

void  robo_hardware::salvarCalibracao(calibracao_dados calibraca_val){
  EEPROM.put(ENDERECO_EEPROM, calibraca_val);
}

void robo_hardware::lerCalibracao(calibracao_dados &cal){
  EEPROM.get(ENDERECO_EEPROM, cal);
}

#include <DHT_U.h>

#include <ESP8266_Lib.h> //INCLUSÃO DE BIBLIOTECA
#include <BlynkSimpleShieldEsp8266.h> //INCLUSÃO DE BIBLIOTECA
#include <SoftwareSerial.h> //INCLUSÃO DE BIBLIOTECA
#include "dht.h" //INCLUSÃO DE BIBLIOTECA

SoftwareSerial EspSerial(10, 11); //PINOS QUE EMULAM A SERIAL (PINO 10 É O RX E PINO 11 É O TX)

char auth[] = "SEU TOKEN AQUI"; //AUTH TOKEN (FORNECIDO PELO PROJETO NO BLYNK E POR E-MAIL)

BlynkTimer timer; //OBJETO DO TIPO BlynkTimer

float temperatura;
float umidade;

//PINO ANALÓGICO UTILIZADO PELO DHT22
const int pinoDHT22 = 9; 
 
//PINO DIGITAL UTILIZADO PELO MÓDULO RELÉ
const int pinoRele1 = 2;
const int pinoRele2 = 3; 
const int pinoRele3 = 4; 
const int pinoRele4 = 5; 

const int pir_signal_pin = 6;
int flag = 0;
boolean pir_status;

DHT dht(pinoDHT22, DHT22); //VARIÁVEL DO TIPO DHT
char ssid[] = "WIFI SSID"; //VARIÁVEL QUE ARMAZENA O NOME DA REDE SEM FIO EM QUE VAI CONECTAR
char pass[] = "WIFI PASS"; //VARIÁVEL QUE ARMAZENA A SENHA DA REDE SEM FIO EM QUE VAI CONECTAR

#define ESP8266_BAUD 9600 //TAXA DE COMUNICAÇÃO DO ESP8266 COM O ARDUINO UNO (SEMPRE UTILIZE 9600 BAUDS)

ESP8266 wifi(&EspSerial); //PASSO OS PARÂMETROS PARA A FUNÇÃO

void setup(){
  dht.begin();
  
  pinMode(pinoRele1, OUTPUT); //DECLARA O PINO COMO SAÍDA
  pinMode(pinoRele2, OUTPUT); 
  pinMode(pinoRele3, OUTPUT);
  pinMode(pinoRele4, OUTPUT);
  pinMode(pir_signal_pin,INPUT);

  
  EspSerial.begin(ESP8266_BAUD); //INICIA A COMUNICAÇÃO COM A TAXA INFORMADA NO ESP8266_BAUD
  delay(10); //INTERVALO DE 10 MILISSEGUNDOS

  Blynk.begin(auth, wifi, ssid, pass); //INICIALIZA A COMUNICAÇÃO BLYNK INFORMANDO OS PARÂMETROS

  timer.setInterval(1000L, sendUptime); //DEFINE O INTERVALO DE 1 SEGUNDO(1000L = 1000 MILISSEGUNDOS)
  //PARA EXECUÇÃO DA FUNÇÃO sendUptime
  //RELÉ INICIA DESLIGADO
  digitalWrite(pinoRele1, HIGH); 
  digitalWrite(pinoRele2, HIGH); 
  digitalWrite(pinoRele3, HIGH); 
  digitalWrite(pinoRele4, HIGH); 
}

void sendUptime(){ //FUNÇÃO QUE OBTÉM A TEMPERATURA / UMIDADE E ENVIA O VALOR OBTIDO PARA OS WIDGETs GAUGE SENSOR
  
  Blynk.virtualWrite(1, dht.readTemperature() ); //ENVIA AO WIDGET GAUGE (PINO VIRTUAL 1) O VALOR DA TEMPERATURA
  Blynk.virtualWrite(2, dht.readHumidity() ); //ENVIA AO WIDGET GAUGE (PINO VIRTUAL 2) O VALOR DA UMIDADE
  pir_status = digitalRead(pir_signal_pin);
  Serial.println(pir_status);
  if(pir_status == HIGH && flag == 0){
    Blynk.email("example@mail.com", "Movimento Detectado!");
    flag = 1;
  }
  else {
    flag = 0;
  }
  
}

void loop(){
  Blynk.run(); //INICIALIZA O BLYNK
  timer.run(); //INICIALIZA O TIMER
}

#include <Wire.h>
#include <virtuabotixRTC.h>
#include <Keypad.h>

virtuabotixRTC myRTC(12, 11, 10);  // Pines para la comunicación con el RTC DS1302

const byte filas = 4;
const byte columnas = 4;

char teclas[filas][columnas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pinesFilas[filas] = {9, 8, 7, 6};         // Pines conectados a las filas
byte pinesColumnas[columnas] = {5, 4, 3, 2};   // Pines conectados a las columnas

Keypad keypad = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas);

const int pinLed = 13;

int alarma1[2];  // Array para almacenar la hora de la alarma (índice 0: hora, índice 1: minutos)

void setup() {
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, LOW);
  
  Serial.begin(9600);
  myRTC.setDS1302Time(0, 0, 0, 1, 1, 1, 2023);  // Establecer una fecha inicial en el RTC (1 de enero de 2023)
}

void loop() {
  char tecla = keypad.getKey();
  myRTC.updateTime(); 
  Serial.print("Current Date / Time: ");                                                                 //| 
  Serial.print(myRTC.dayofmonth);                                                                        //| 
  Serial.print("/");                                                                                     //| 
  Serial.print(myRTC.month);                                                                             //| 
  Serial.print("/");                                                                                     //| 
  Serial.print(myRTC.year);                                                                              //| 
  Serial.print("  ");                                                                                    //| 
  Serial.print(myRTC.hours);                                                                             //| 
  Serial.print(":");                                                                                     //| 
  Serial.print(myRTC.minutes);                                                                           //| 
  Serial.print(":");                                                                                     //| 
  Serial.println(myRTC.seconds); 
  
  if (tecla == 'A') {
    ingresarHoraAlarma(alarma1);
    Serial.print("ok");
  }
  
  else if (myRTC.hours == alarma1[0] && myRTC.minutes == alarma1[1]) {
    digitalWrite(pinLed, HIGH);  // Encender el LED cuando se alcance la hora de la alarma
  } else {
    digitalWrite(pinLed, LOW);   // Apagar el LED en caso contrario
  }
  
  delay(1000);
}
        
void ingresarHoraAlarma(int alarma[2]) {
  int valor = 0;
  int indice = 0;
  
  while (indice < 2) {
    char tecla = keypad.getKey();
    
    if (tecla >= '0' && tecla <= '9') {
      valor = valor * 10 + (tecla - '0');
      Serial.print(tecla);
    } else if (tecla == '#') {
      alarma[indice] = valor;
      Serial.println();
      Serial.print("Alarma[");
      Serial.print(indice);
      Serial.print("] = ");
      Serial.println(valor);
      valor = 0;
      indice++;
    }
  }
}

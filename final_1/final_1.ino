/*
credo por el equipo la pupineta, con el rtc ds3231
*/
////inclucion de librerias///
#include <EEPROM.h> // incluye la librería para el manejo de la memoria EEPROM
#include <Wire.h>
#include <LiquidCrystal_I2C.h>///librerias para el lcd y la comunicacion i2c////
#include "RTClib.h"////libreria del rtc
#include <Keypad.h> ////libreria del teclado matricial
LiquidCrystal_I2C lcd(0x27,16,2);//Crea el objeto lcd direccion lcd 0x27 y 16 columnas x 2 filas
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const byte filas = 4;       // Número de filas del teclado matricial
const byte columnas = 4;    // Número de columnas del teclado matricial
char teclas[filas][columnas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pinesFilas[filas] = {9, 8, 7, 6};         // Pines conectados a las filas
byte pinesColumnas[columnas] = {5, 4, 3, 2};   // Pines conectados a las columnas
Keypad keypad = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas);
int Alarma[9][3];////matris de alarmas
int alarma2[9][3];
#define Led 13 ///rele
#define Boton A0
//////variables para el temporizador//////
int horas = 0;
int minutos = 0;
int segundos = 0;
bool temporizadorActivo = false;
#define direccionInicio 0 // declara una variable con la dirección inicial a escribir
const int numAlarms = sizeof(Alarma);
void setup() {
  Serial.begin(9600);////inicia la comunicacion serie a 9600 baudios
  lcd.init();  
  lcd.backlight();//Enciende la luz de fondo
  lcd.setCursor(3,0);
  lcd.print("Bienvenido");
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");  
  }
  pinMode(Led,OUTPUT); ///Led como salida
  pinMode(Boton,INPUT_PULLUP);//Boton como entrada con una resistencia en pull up
  delay(1000);
  lcd.clear();
  eeprom_read_block (( void * ) & Alarma, 20, sizeof ( Alarma )) ; //lee el valor de la eeprom y lo almacena en la matris
}

void loop() { 
   DateTime myRTC = rtc.now();
   eeprom_update_block (( void * ) & Alarma, 20, sizeof ( Alarma )) ;  ///actualiza la matris on los valores, obiamente desordenados   
   bubbleSort();///funcion para ordenar la matris  
   /*Serial.print("-> ");
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 3; j++) {        
        Serial.print(alarma2[i][j]);
        Serial.print("\t");
      }
      Serial.println();
    }
     Serial.print(">> ");
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 3; j++) {        
        Serial.print(Alarma[i][j]);
        Serial.print("\t");
      }
      Serial.println();
    }*/
    if(alarma2[0][0] <= -1){
        lcd.setCursor(0, 1);
        lcd.print("No hay Alarmas");      
      }else{
        lcd.setCursor(0,1);  
        lcd.print(alarma2[0][0]);
        lcd.print(":");
        lcd.print(alarma2[0][1]);
        lcd.print(":");
        lcd.print(alarma2[0][2]); 
      }   
    lcd.setCursor(0,0);
    lcd.print(myRTC.day(),DEC);///imprime el dia del mes
    lcd.print("/");
    lcd.print(myRTC.month(),DEC); ///imprime el mes
    lcd.print("/");
    lcd.print(myRTC.year()%100);///imprime los 2 ultimos numeros del año
    lcd.print(" ");
    lcd.print(myRTC.hour(),DEC);///imprime la hora
    lcd.print(":");
    lcd.print(myRTC.minute(),DEC);///imprime los minutos
    lcd.print(":"); 
    lcd.print(myRTC.second(),DEC);////imprime los segundos
    lcd.print(" "); 
   
    if(Boton==0){
      ///verifica si el boton a sido pulsodo y prende el Led
      digitalWrite(Led,HIGH);      
    }else{
        digitalWrite(Led,LOW);
    }
    for(int i=0;i<9;i++){
      if(myRTC.hour()==Alarma[i][0]&&myRTC.minute()==Alarma[i][1]&&myRTC.second()==Alarma[i][2]){
        digitalWrite(Led, HIGH);  // Encender el LED cuando se alcance la hora de la alarma
        delay(5000);
        digitalWrite(Led,LOW);  
      } 
    }
    
    char tecla = keypad.getKey();///lee las tecla presionadas    
    if(tecla=='*'){ ///si la tecla es un * entra al menu principal
      lcd.clear();///limpia la pantalla
      while (1){  
        //lcd.clear();      
        lcd.setCursor(0,0);
        lcd.print("C New alarm");
        lcd.setCursor(0,1);
        lcd.print("D Delete alarm"); 
        //delay(100);
        char tecla = keypad.getKey();                  
        switch(tecla){ ///lee la tecla y si cumple con cada caso crea o elimina una alarma
          case 'C':
                   lcd.clear();
                   while(1){
                     char tecla = keypad.getKey();                               
                     lcd.setCursor(0,0);
                     lcd.print("Posicion N");
                     lcd.print(char(223));
                     lcd.print(":"); 
                     delay(100);
                     lcd.clear();                               
                     switch(tecla){///posicion de la alarma que se agrega
                      case '0':
                      lcd.clear();
                             while(1){
                                    char tecla = keypad.getKey();
                                    lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print("Posicion 0");
                                    lcd.setCursor(0,1);
                                    lcd.print ("Confirme");
                                    if (tecla=='*'){////confirma que este seguro
                                      ingresarHoraAlarma(0);///llama a la funcion y le pasa el parametro de la posicion
                                      lcd.setCursor(0,1);
                                      lcd.print("lito");
                                      delay(1000);
                                      lcd.clear();
                                      break;///una vez que hico todo, sale del loop
                                    }
                                    delay(100);
                                  if(tecla=='#'){
                                    break;
                                  }
                             } 
                             break;
                    case '1':
                     lcd.clear();
                              while(1){
                                    char tecla = keypad.getKey();
                                    lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print("Posicion 1");
                                    lcd.setCursor(0,1);
                                    lcd.print ("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(1); 
                                      lcd.setCursor(0,1);
                                      lcd.print("lito");
                                      delay(1000);
                                      lcd.clear();                                
                                      break;
                                    }
                                    delay(100);
                                   if(tecla=='#'){
                                      break;
                                    }
                              }
                              break;
                                  
                     case '2':
                       lcd.clear();
                                  while(1){
                                    char tecla = keypad.getKey();
                                    lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print("Posicion 2");
                                    lcd.setCursor(0,1);
                                    lcd.print ("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(2);
                                      lcd.setCursor(0,1);
                                      lcd.print("lito"); 
                                      delay(1000);
                                      lcd.clear();                                 
                                      break;
                                    }
                                    delay(100);
                                  if(tecla=='#'){
                                    break;
                                  }
                                }
                                  break;
                                  
                     case '3':
                       lcd.clear();
                                  while(1){
                                    char tecla = keypad.getKey();
                                    lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print("Posicion 3");
                                    lcd.setCursor(0,1);
                                    lcd.print ("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(3);
                                      lcd.setCursor(0,1);
                                      lcd.print("lito");
                                      delay(1000);
                                      lcd.clear();                                  
                                      break;
                                    }
                                    delay(100);
                                   if(tecla=='#'){
                                    break;
                                  }
                                }
                                  break;
                                  
                      case '4':
                         lcd.clear();
                                  while(1){
                                    char tecla = keypad.getKey();
                                    lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print("Posicion 4");
                                    lcd.setCursor(0,1);
                                    lcd.print ("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(4);  
                                      lcd.setCursor(0,1);
                                      lcd.print("lito");      
                                      delay(1000);
                                      lcd.clear();                          
                                      break;
                                    }
                                    delay(100);
                                   if(tecla=='#'){
                                    break;
                                  }
                                }
                                  break;
                                  
                       case'5':
                           lcd.clear();
                                  while(1){
                                    char tecla = keypad.getKey();
                                    lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print("Posicion 5");
                                    lcd.setCursor(0,1);
                                    lcd.print ("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(5);
                                      lcd.setCursor(0,1);
                                      lcd.print("lito");    
                                      delay(1000);
                                      lcd.clear();                              
                                      break;
                                    }
                                    delay(100);
                                   if(tecla=='#'){
                                      break;
                                    }
                                  }
                                  break;
                                  
                       case'6':
                           lcd.clear();
                                  while(1){
                                    char tecla = keypad.getKey();
                                    lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print("Posicion 6");
                                    lcd.setCursor(0,1);
                                    lcd.print ("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(6); 
                                      lcd.setCursor(0,1);
                                      lcd.print("lito"); 
                                      delay(1000);
                                      lcd.clear();                                
                                      break;
                                    }
                                    delay(100);
                                  if(tecla=='#'){
                                      break;
                                    }
                                }
                                break;
                                  
                       case '7':
                           lcd.clear();
                                  while(1){
                                    char tecla = keypad.getKey();
                                    lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print("Posicion 7");
                                    lcd.setCursor(0,1);
                                    lcd.print ("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(7); 
                                      lcd.setCursor(0,1);
                                      lcd.print("lito"); 
                                      delay(1000);
                                      lcd.clear();                                
                                      break;
                                    }
                                    delay(100);
                                   if(tecla=='#'){
                                      break;
                                    }
                                  }
                                  break;
                                  
                       case'8':
                           lcd.clear();
                                  while(1){
                                    char tecla = keypad.getKey();
                                    lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print("Posicion 8");
                                    lcd.setCursor(0,1);
                                    lcd.print ("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(8);
                                      lcd.setCursor(0,1);
                                      lcd.print("lito");
                                      delay(1000);
                                      lcd.clear();                                 
                                      break;
                                    }
                                    delay(100);                                  
                                  if(tecla=='#'){
                                      break;
                                    }
                                  }
                                  break;                                  
                                  
                       case'9':
                           lcd.clear();
                                  while(1){
                                    char tecla = keypad.getKey();
                                   lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print("Posicion 9");
                                    lcd.setCursor(0,1);
                                    lcd.print ("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(9); 
                                      lcd.setCursor(0,1);
                                      lcd.print("lito");  
                                      delay(1000);
                                      lcd.clear();                               
                                      break;
                                    }
                                    delay(100);                                                           
                                    if(tecla=='#'){
                                      break;
                                    }
                                  }
                                  break;
                   }
                   if(tecla=='#'){
                    break;
                  }
                  }              
                  break;                                  
          case 'D': 
                   lcd.clear();
                   while(1){
                            char tecla = keypad.getKey();                                     
                            ///lcd.setCursor(0,0);
                            lcd.print("Posicion N");
                            lcd.print(char(223));
                            lcd.print(":"); 
                            delay(100);  
                            lcd.clear();         
                            switch(tecla){
                                        case '0':
                                              lcd.clear();
                                              while (1) {
                                                char tecla = keypad.getKey();
                                                lcd.clear();
                                                if(Alarma[0][0] <= -1){
                                                  while(1) {
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("No hay Alarmas");
                                                    delay(1000);
                                                    lcd.clear();
                                                    break;    
                                                   }
                                                   break;
                                                }else {
                                                  lcd.setCursor(0, 0);
                                                  lcd.print(Alarma[0][0]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[0][1]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[0][2]);
                                                  lcd.setCursor(0, 1);
                                                  lcd.print("Desea borrar?");
                                                  if (tecla == '*') {
                                                  while (1) {
                                                    char tecla = keypad.getKey();
                                                    lcd.clear();
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("Esta seguro?");
                                                    delay(100);
                                                    if (tecla == '*') {
                                                      borrarAlarma(0);
                                                      lcd.setCursor(0, 1);
                                                      lcd.print("Listo!");
                                                      delay(1000);
                                                      lcd.clear();
                                                      break;
                                                    } else if (tecla == '#') {
                                                      break;
                                                    }
                                                  }
                                                  break;
                                                } else if (tecla == '#') {
                                                  break;
                                                }
                                                delay(200);
                                              }
                                              }
                                              break;
                                        case '1':
                                                lcd.clear();
                                              while (1) {
                                                char tecla = keypad.getKey();
                                                lcd.clear();
                                                if(Alarma[1][0] <= -1){
                                                  while(1) {
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("No hay Alarmas");
                                                    delay(1000);
                                                    lcd.clear();
                                                    break;    
                                                   }
                                                   break;
                                                }else {
                                                  lcd.setCursor(0, 0);
                                                  lcd.print(Alarma[1][0]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[1][1]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[1][2]);
                                                  lcd.setCursor(0, 1);
                                                  lcd.print("Desea borrar?");
                                                  if (tecla == '*') {
                                                  while (1) {
                                                    char tecla = keypad.getKey();
                                                    lcd.clear();
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("Esta seguro?");
                                                    delay(100);
                                                    if (tecla == '*') {
                                                      borrarAlarma(1);
                                                      lcd.setCursor(0, 1);
                                                      lcd.print("Listo!");
                                                      delay(1000);
                                                      lcd.clear();
                                                      break;
                                                    } else if (tecla == '#') {
                                                      break;
                                                    }
                                                  }
                                                  break;
                                                } else if (tecla == '#') {
                                                  break;
                                                }
                                                delay(200);
                                              }
                                              }
                                              break;
                                        case '2':
                                              lcd.clear();
                                              while (1) {
                                                char tecla = keypad.getKey();
                                                lcd.clear();
                                                if(Alarma[2][0] <= -1){
                                                  while(1) {
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("No hay Alarmas");
                                                    delay(1000);
                                                    lcd.clear();
                                                    break;    
                                                   }
                                                   break;
                                                }else {
                                                  lcd.setCursor(0, 0);
                                                  lcd.print(Alarma[2][0]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[2][1]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[2][2]);
                                                  lcd.setCursor(0, 1);
                                                  lcd.print("Desea borrar?");
                                                  if (tecla == '*') {
                                                  while (1) {
                                                    char tecla = keypad.getKey();
                                                    lcd.clear();
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("Esta seguro?");
                                                    delay(100);
                                                    if (tecla == '*') {
                                                      borrarAlarma(2);
                                                      lcd.setCursor(0, 1);
                                                      lcd.print("Listo!");
                                                      delay(1000);
                                                      lcd.clear();
                                                      break;
                                                    } else if (tecla == '#') {
                                                      break;
                                                    }
                                                  }
                                                  break;
                                                } else if (tecla == '#') {
                                                  break;
                                                }
                                                delay(200);
                                              }
                                              }
                                              break;
                                        case '3':
                                                lcd.clear();
                                              while (1) {
                                                char tecla = keypad.getKey();
                                                lcd.clear();
                                                if(Alarma[3][0] <= -1){
                                                  while(1) {
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("No hay Alarmas");
                                                    delay(1000);
                                                    lcd.clear();
                                                    break;    
                                                   }
                                                   break;
                                                }else {
                                                  lcd.setCursor(0, 0);
                                                  lcd.print(Alarma[3][0]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[3][1]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[3][2]);
                                                  lcd.setCursor(0, 1);
                                                  lcd.print("Desea borrar?");
                                                  if (tecla == '*') {
                                                  while (1) {
                                                    char tecla = keypad.getKey();
                                                    lcd.clear();
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("Esta seguro?");
                                                    delay(100);
                                                    if (tecla == '*') {
                                                      borrarAlarma(3);
                                                      lcd.setCursor(0, 1);
                                                      lcd.print("Listo!");
                                                      delay(1000);
                                                      lcd.clear();
                                                      break;
                                                    } else if (tecla == '#') {
                                                      break;
                                                    }
                                                  }
                                                  break;
                                                } else if (tecla == '#') {
                                                  break;
                                                }
                                                delay(200);
                                              }
                                              }
                                              break;
                                       case '4':
                                                lcd.clear();
                                              while (1) {
                                                char tecla = keypad.getKey();
                                                lcd.clear();
                                                if(Alarma[4][0] <= -1){
                                                  while(1) {
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("No hay Alarmas");
                                                    delay(1000);
                                                    lcd.clear();
                                                    break;    
                                                   }
                                                   break;
                                                }else {
                                                  lcd.setCursor(0, 0);
                                                  lcd.print(Alarma[4][0]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[4][1]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[4][2]);
                                                  lcd.setCursor(0, 1);
                                                  lcd.print("Desea borrar?");
                                                  if (tecla == '*') {
                                                  while (1) {
                                                    char tecla = keypad.getKey();
                                                    lcd.clear();
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("Esta seguro?");
                                                    delay(100);
                                                    if (tecla == '*') {
                                                      borrarAlarma(4);
                                                      lcd.setCursor(0, 1);
                                                      lcd.print("Listo!");
                                                      delay(1000);
                                                      lcd.clear();
                                                      break;
                                                    } else if (tecla == '#') {
                                                      break;
                                                    }
                                                  }
                                                  break;
                                                } else if (tecla == '#') {
                                                  break;
                                                }
                                                delay(200);
                                              }
                                              }
                                              break;
                                        case'5':
                                                lcd.clear();
                                              while (1) {
                                                char tecla = keypad.getKey();
                                                lcd.clear();
                                                if(Alarma[5][0] <= -1){
                                                  while(1) {
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("No hay Alarmas");
                                                    delay(1000);
                                                    lcd.clear();
                                                    break;    
                                                   }
                                                   break;
                                                }else {
                                                  lcd.setCursor(0, 0);
                                                  lcd.print(Alarma[5][0]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[5][1]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[5][2]);
                                                  lcd.setCursor(0, 1);
                                                  lcd.print("Desea borrar?");
                                                  if (tecla == '*') {
                                                  while (1) {
                                                    char tecla = keypad.getKey();
                                                    lcd.clear();
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("Esta seguro?");
                                                    delay(100);
                                                    if (tecla == '*') {
                                                      borrarAlarma(5);
                                                      lcd.setCursor(0, 1);
                                                      lcd.print("Listo!");
                                                      delay(1000);
                                                      lcd.clear();
                                                      break;
                                                    } else if (tecla == '#') {
                                                      break;
                                                    }
                                                  }
                                                  break;
                                                } else if (tecla == '#') {
                                                  break;
                                                }
                                                delay(200);
                                              }
                                              }
                                              break;
                                        case'6':
                                              lcd.clear();
                                              while (1) {
                                                char tecla = keypad.getKey();
                                                lcd.clear();
                                                if(Alarma[6][0] <= -1){
                                                  while(1) {
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("No hay Alarmas");
                                                    delay(1000);
                                                    lcd.clear();
                                                    break;    
                                                   }
                                                   break;
                                                }else {
                                                  lcd.setCursor(0, 0);
                                                  lcd.print(Alarma[6][0]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[6][1]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[6][2]);
                                                  lcd.setCursor(0, 1);
                                                  lcd.print("Desea borrar?");
                                                  if (tecla == '*') {
                                                  while (1) {
                                                    char tecla = keypad.getKey();
                                                    lcd.clear();
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("Esta seguro?");
                                                    delay(100);
                                                    if (tecla == '*') {
                                                      borrarAlarma(6);
                                                      lcd.setCursor(0, 1);
                                                      lcd.print("Listo!");
                                                      delay(1000);
                                                      lcd.clear();
                                                      break;
                                                    } else if (tecla == '#') {
                                                      break;
                                                    }
                                                  }
                                                  break;
                                                } else if (tecla == '#') {
                                                  break;
                                                }
                                                delay(200);
                                              }
                                              }
                                              break;
                                        case'7':
                                                lcd.clear();
                                              while (1) {
                                                char tecla = keypad.getKey();
                                                lcd.clear();
                                                if(Alarma[7][0] <= -1){
                                                  while(1) {
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("No hay Alarmas");
                                                    delay(1000);
                                                    lcd.clear();
                                                    break;    
                                                   }
                                                   break;
                                                }else {
                                                  lcd.setCursor(0, 0);
                                                  lcd.print(Alarma[7][0]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[7][1]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[7][2]);
                                                  lcd.setCursor(0, 1);
                                                  lcd.print("Desea borrar?");
                                                  if (tecla == '*') {
                                                  while (1) {
                                                    char tecla = keypad.getKey();
                                                    lcd.clear();
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("Esta seguro?");
                                                    delay(100);
                                                    if (tecla == '*') {
                                                      borrarAlarma(7);
                                                      lcd.setCursor(0, 1);
                                                      lcd.print("Listo!");
                                                      delay(1000);
                                                      lcd.clear();
                                                      break;
                                                    } else if (tecla == '#') {
                                                      break;
                                                    }
                                                  }
                                                  break;
                                                } else if (tecla == '#') {
                                                  break;
                                                }
                                                delay(200);
                                              }
                                              }
                                              break;
                                        case'8':
                                               lcd.clear();
                                              while (1) {
                                                char tecla = keypad.getKey();
                                                lcd.clear();
                                                if(Alarma[8][0] <= -1){
                                                  while(1) {
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("No hay Alarmas");
                                                    delay(1000);
                                                    lcd.clear();
                                                    break;    
                                                   }
                                                   break;
                                                }else {
                                                  lcd.setCursor(0, 0);
                                                  lcd.print(Alarma[8][0]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[8][1]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[8][2]);
                                                  lcd.setCursor(0, 1);
                                                  lcd.print("Desea borrar?");
                                                  if (tecla == '*') {
                                                  while (1) {
                                                    char tecla = keypad.getKey();
                                                    lcd.clear();
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("Esta seguro?");
                                                    delay(100);
                                                    if (tecla == '*') {
                                                      borrarAlarma(8);
                                                      lcd.setCursor(0, 1);
                                                      lcd.print("Listo!");
                                                      delay(1000);
                                                      lcd.clear();
                                                      break;
                                                    } else if (tecla == '#') {
                                                      break;
                                                    }
                                                  }
                                                  break;
                                                } else if (tecla == '#') {
                                                  break;
                                                }
                                                delay(200);
                                              }
                                              }
                                              break;
                                        case'9':
                                              lcd.clear();
                                              while (1) {
                                                char tecla = keypad.getKey();
                                                lcd.clear();
                                                if(Alarma[9][0] <= -1){
                                                  while(1) {
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("No hay Alarmas");
                                                    delay(1000);
                                                    lcd.clear();
                                                    break;    
                                                   }
                                                   break;
                                                }else {
                                                  lcd.setCursor(0, 0);
                                                  lcd.print(Alarma[9][0]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[9][1]);
                                                  lcd.print("/");
                                                  lcd.print(Alarma[9][2]);
                                                  lcd.setCursor(0, 1);
                                                  lcd.print("Desea borrar?");
                                                  if (tecla == '*') {
                                                  while (1) {
                                                    char tecla = keypad.getKey();
                                                    lcd.clear();
                                                    lcd.setCursor(0, 0);
                                                    lcd.print("Esta seguro?");
                                                    delay(100);
                                                    if (tecla == '*') {
                                                      borrarAlarma(9);
                                                      lcd.setCursor(0, 1);
                                                      lcd.print("Listo!");
                                                      delay(1000);
                                                      lcd.clear();
                                                      break;
                                                    } else if (tecla == '#') {
                                                      break;
                                                    }
                                                  }
                                                  break;
                                                } else if (tecla == '#') {
                                                  break;
                                                }
                                                delay(200);
                                              }
                                              }
                                              break;
                                        }if(tecla=='#'){
                                            break;
                                          }                                  
                             }
                   }
          if(tecla=='#'){break;}
      }
    }    
    //delay(100);
    //lcd.clear();
}
////////ordenamiento de burbuja
void bubbleSort() {
  DateTime myRTC = rtc.now();
  // Copiar la matriz original a la matriz ordenada
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 3; j++) {
      alarma2[i][j] = Alarma[i][j];
    }
  }
  for (int i = 0; i < 9 - 1; i++) {
    for (int j = 0; j < 9 - i - 1; j++) {
      // Obtener la hora, los minutos y los segundos de las dos filas a comparar
      int hour1 = alarma2[j][0];
      int minute1 = alarma2[j][1];
      int second1 = alarma2[j][2];
      int hour2 = myRTC.hour();
      int minute2 = myRTC.minute();
      int second2 = myRTC.second();
       if (hour1 < hour2 || (hour1 == hour2 && minute1 < minute2) || (hour1 == hour2 && minute1 == minute2 && second1 < second2)) {
        // Intercambiar las filas si están en el orden incorrecto
        for (int k = 0; k < 3; k++) {
          int temp = alarma2[j][k];
          alarma2[j][k] = alarma2[j+1][k];
          alarma2[j+1][k] = temp;
        }
      }
    }
  }
  
   int closestAlarm = -1;
  int closestTimeDiff = 24 * 60 * 60; // Inicializar con un valor alto (24 horas en segundos)
  for (int i = 0; i < 9; i++) {
    int alarmHour = alarma2[i][0];
    int alarmMinute = alarma2[i][1];
    int alarmSecond = alarma2[i][2];

    int timeDiff = calculateTimeDifference(myRTC.hour(), myRTC.minute(), myRTC.second(), alarmHour, alarmMinute, alarmSecond);
    if (timeDiff < closestTimeDiff) {
      closestAlarm = i;
      closestTimeDiff = timeDiff;
    }
  }

  // Mover la alarma más cercana a la posición 0 de la matriz ordenada
  if (closestAlarm != -1) {
    for (int i = closestAlarm; i > 0; i--) {
      for (int j = 0; j < 3; j++) {
        int temp = alarma2[i][j];
        alarma2[i][j] = alarma2[i - 1][j];
        alarma2[i - 1][j] = temp;
      }
    }
  }
}


int calculateTimeDifference(int currentHour, int currentMinute, int currentSecond, int alarmHour, int alarmMinute, int alarmSecond) {
  int currentTimeInSeconds = currentHour * 3600 + currentMinute * 60 + currentSecond;
  int alarmTimeInSeconds = alarmHour * 3600 + alarmMinute * 60 + alarmSecond;

  if (alarmTimeInSeconds >= currentTimeInSeconds) {
    return alarmTimeInSeconds - currentTimeInSeconds;
  } else {
    return (24 * 60 * 60 - currentTimeInSeconds) + alarmTimeInSeconds;
  }
}
////////funciones de la alarma////////////
void borrarAlarma(int i){
  /*
   solo el parametro de la matris para que todo se vuelva 0 y por ende se elimine el dato
   */
   for(int a=0;a<3;a++){
      Alarma[i][a]=-1;
    }   
}

void ingresarHoraAlarma(int i) {
  /*
   se ingresa la posición de la matriz que fue seleccionada,
   y se ingresa la hora, minutos y segundos
   */
  
lcd.clear();
if (Alarma[i][0] > 0) {
  lcd.setCursor(0, 0);
  lcd.print("Ya hay una");
  lcd.setCursor(0, 1);
  lcd.print("Sobreescribir?");
  while (1) {
    char tecla = keypad.getKey();
    if (tecla == '*') {
      lcd.clear();
      aux(i);
    } else if (tecla == '#') {
      lcd.clear();
      break;
    }
  }
} else {
  aux(i);
}

}
void aux(int i){
  int valor = 0;
  int indice = 0;
  while (indice <= 2) {
    if (indice == 0) {
      lcd.setCursor(0, 0);
      lcd.print("Ingrese Hora");
    } else if (indice == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Ingrese Minutos");
    } else if (indice == 2) {
      lcd.setCursor(0, 0);
      lcd.print("Ingrese Segundos");
    }
    char tecla = keypad.getKey();
    if (tecla >= '0' && tecla <= '9') {
      valor = valor * 10 + (tecla - '0');
      lcd.setCursor(0, 1);
      lcd.print(valor);
    } else if (tecla == '*') {
      lcd.clear();
      if (valor > 23 && indice == 0) {
        lcd.setCursor(0, 0);
        lcd.print("Hora imposible");
        lcd.setCursor(0, 1);
        lcd.print("Intente de nuevo");
        delay(1000);
        lcd.clear();
        valor=0;
      } else if (valor > 59 && indice == 1) {
        lcd.setCursor(0, 0);
        lcd.print("Minutos imposible");
        lcd.setCursor(0, 1);
        lcd.print("Intente de nuevo");
        delay(1000); 
        lcd.clear();
        valor=0;             
      } else if (valor > 59 && indice == 2) {
        lcd.setCursor(0, 0);
        lcd.print("Segundos imposible");
        lcd.setCursor(0, 1);
        lcd.print("Intente de nuevo");
        delay(1000); 
        lcd.clear();
        valor=0;      
      } else {
        Alarma[i][indice] = valor;
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
}

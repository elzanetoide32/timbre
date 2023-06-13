////inclucion de librerias///
#include <EEPROM.h> // incluye la librería para el manejo de la memoria EEPROM
#include <Wire.h>
#include <LiquidCrystal_I2C.h>///librerias para el lcd y la comunicacion i2c////
#include <virtuabotixRTC.h> ////libreria del rtc
#include <Keypad.h> ////libreria del teclado matricial
LiquidCrystal_I2C lcd(0x27,16,2);//Crea el objeto lcd direccion lcd 0x27 y 16 columnas x 2 filas
#define data 12
#define rst 11
#define clk 10
virtuabotixRTC myRTC(data,rst,clk); ///crea el objeto myRTC (data,reset,clk)
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
#define Led 13 ///rele
#define Boton A0
//////variables para el temporizador//////
int horas = 0;
int minutos = 0;
int segundos = 0;
bool temporizadorActivo = false;
#define direccionInicio 0 // declara una variable con la dirección inicial a escribir
//const int numAlarms = sizeof(alarms) / sizeof(alarms[0]);
void setup() {
  Serial.begin(9600);////inicia la comunicacion serie a 9600 baudios
  lcd.init();  
  lcd.backlight();//Enciende la luz de fondo
  lcd.setCursor(3,0);
  lcd.print("Bienvenido");
  myRTC.setDS1302Time(00, 30, 14, 6, 13, 6, 2023); ///setea la hora en el rtc en el formato(segundo,minutos,horas,dia de la semana, dia del mes, mes, año)  
  pinMode(Led,OUTPUT); ///Led como salida
  pinMode(Boton,INPUT_PULLUP);//Boton como entrada con una resistencia en pull up
  ///EEPROM.get( direccionInicio, Alarma );  
  delay(1000);
  lcd.clear();
}

void loop() { 
    /*bubbleSort(alarms, numAlarms);  
    Serial.print("La siguiente alarma es a las ");
    Serial.print(nextAlarm.hour);
    Serial.print(":");
    Serial.print(nextAlarm.minute);
    Serial.println();*/
    //EEPROM.put(direccionInicio, Alarma); // escribe en la dirección indicada, y la actualiza   
    myRTC.updateTime(); ///actualiza el rtc
    lcd.setCursor(0,0);
    lcd.print(myRTC.dayofmonth);///imprime el dia del mes
    lcd.print("/");
    lcd.print(myRTC.month); ///imprime el mes
    lcd.print("/");
    lcd.print(myRTC.year%100);///imprime los 2 ultimos numeros del año
    lcd.print(" ");
    lcd.print(myRTC.hours);///imprime la hora
    lcd.print(":");
    lcd.print(myRTC.minutes);///imprime los minutos
    lcd.print(":"); 
    lcd.print(myRTC.seconds);////imprime los segundos
    lcd.print(" "); 
   
    if(Boton==0){
      ///verifica si el boton a sido pulsodo y prende el Led
      digitalWrite(Led,HIGH);      
    }     
    else{
        digitalWrite(Led,LOW);
    }
    for(int i=0;i<9;i++){
      if(myRTC.hours==0&&myRTC.minutes==0&&myRTC.seconds==0){
        ///compara si la hora es = a 0 y no prende, ya que la matris x defecto si no ponemos nada los numeros son 0
      digitalWrite(Led,LOW);
      }
      else if(myRTC.hours==Alarma[i][0]&&myRTC.minutes==Alarma[i][1]&&myRTC.seconds==Alarma[i][2]){
        digitalWrite(Led, HIGH);  // Encender el LED cuando se alcance la hora de la alarma
        delay(5000);
        digitalWrite(Led,LOW);  
      }
    }  
    
    char tecla = keypad.getKey();///lee las tecla presionadas    
    if(tecla=='*'){ ///si la tecla es un * entra al menu principal
      lcd.clear();///limpia la pantalla
      while (1){
        char tecla = keypad.getKey();        
        switch(tecla){ ///toma la tecla presionada y en base a cada caso hace cosas distintas 
          case 'A': ///entra en un menu
                  lcd.clear();
                  while(1){ 
                    char tecla = keypad.getKey();         
                    lcd.setCursor(0,0);
                    lcd.print("C New alarm");
                    lcd.setCursor(0,1);
                    lcd.print("D Delete alarm");
                    switch(tecla){ ///lee la tecla y si cumple con cada caso crea o elimina una alarma
                      case 'C':
                              lcd.clear();
                              while(1){
                                char tecla = keypad.getKey();                               
                                lcd.setCursor(0,0);
                                lcd.print("N alarma:");                                
                                switch(tecla){///posicion de la alarma que se agrega
                                  
                                  case '0':
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
                                  }                               
                                  break;
                                  
                                  case '1':
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
                                  }
                                  break;
                                  
                                  case '2':
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
                                  }
                                  break;
                                  
                                  case '3':
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
                                  }
                                  break;
                                  
                                  case '4':
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
                                  }
                                  break;
                                  
                                  case'5':
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
                                  }
                                  break;
                                  
                                  case'6':
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
                                  }
                                  break;
                                  
                                  case '7':
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
                                  }
                                  break;
                                  
                                  case'8':
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
                                  }
                                  break;
                                  
                                  case'9':
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
                lcd.setCursor(0,0);
                lcd.print("Ingrese la posicion");                             
                lcd.scrollDisplayLeft();
                delay(300);
                switch(tecla){
                  case '0':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Borrar alarma 0");
                    if (tecla=='*'){
                      while(1){
                        char tecla = keypad.getKey();  
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Estas seguro?");
                        delay(100);                                           
                        if(tecla=='*'){///confirma que este seguro
                          borrarAlarma(0);////llama a la funcion y le pasa el parametro de la posicion
                          lcd.setCursor(0,1);
                          lcd.print("lito");
                          delay(1000);
                          lcd.clear();                        
                          break;
                        }                    
                      }   
                      break;                   
                    }
                      delay(200);
                  }
                  break;
                  case '1':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Borrar alarma 1");
                    if (tecla=='*'){
                      while(1){
                        char tecla = keypad.getKey();  
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Estas seguro?");
                        delay(100);                                           
                        if(tecla=='*'){
                          borrarAlarma(1);
                          lcd.setCursor(0,1);
                          lcd.print("lito");
                          delay(1000);
                          lcd.clear();                        
                          break;
                        }                    
                      }   
                      break;                   
                    }
                      delay(200);
                  }
                  break;
                  case '2':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Borrar alarma 2");
                    if (tecla=='*'){
                      while(1){
                        char tecla = keypad.getKey();  
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Estas seguro?");
                        delay(100);                                           
                        if(tecla=='*'){
                          borrarAlarma(2);
                          lcd.setCursor(0,1);
                          lcd.print("lito");
                          delay(1000);
                          lcd.clear();                        
                          break;
                        }                    
                      }   
                      break;                   
                    }
                      delay(200);
                  }
                  break;
                  case '3':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Borrar alarma 3");
                    if (tecla=='*'){
                      while(1){
                        char tecla = keypad.getKey();  
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Estas seguro?");
                        delay(100);                                           
                        if(tecla=='*'){
                          borrarAlarma(3);
                          lcd.setCursor(0,1);
                          lcd.print("lito");
                          delay(1000);
                          lcd.clear();                        
                          break;
                        }                    
                      }   
                      break;                   
                    }
                      delay(200);
                  }
                  break;
                  case '4':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Borrar alarma 4");
                    if (tecla=='*'){
                      while(1){
                        char tecla = keypad.getKey();  
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Estas seguro?");
                        delay(100);                                           
                        if(tecla=='*'){
                          borrarAlarma(4);
                          lcd.setCursor(0,1);
                          lcd.print("lito");
                          delay(1000);
                          lcd.clear();                        
                          break;
                        }                    
                      }   
                      break;                   
                    }
                      delay(200);
                  }
                  break;
                  case'5':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Borrar alarma 5");
                    if (tecla=='*'){
                      while(1){
                        char tecla = keypad.getKey();  
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Estas seguro?");
                        delay(100);                                           
                        if(tecla=='*'){
                          borrarAlarma(5);
                          lcd.setCursor(0,1);
                          lcd.print("lito");
                          delay(1000);
                          lcd.clear();                        
                          break;
                        }                    
                      }   
                      break;                   
                    }
                      delay(200);
                  }
                  break;
                  case'6':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Borrar alarma 6");
                    if (tecla=='*'){
                      while(1){
                        char tecla = keypad.getKey();  
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Estas seguro?");
                        delay(100);                                           
                        if(tecla=='*'){
                          borrarAlarma(6);
                          lcd.setCursor(0,1);
                          lcd.print("lito");
                          delay(1000);
                          lcd.clear();                        
                          break;
                        }                    
                      }   
                      break;                   
                    }
                      delay(200);
                  }
                  break;
                  case'7':
                 while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Borrar alarma 7");
                    if (tecla=='*'){
                      while(1){
                        char tecla = keypad.getKey();  
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Estas seguro?");
                        delay(100);                                           
                        if(tecla=='*'){
                          borrarAlarma(7);
                          lcd.setCursor(0,1);
                          lcd.print("lito");
                          delay(1000);
                          lcd.clear();                        
                          break;
                        }                    
                      }   
                      break;                   
                    }
                      delay(200);
                  }
                  break;
                  case'8':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Borrar alarma 8");
                    if (tecla=='*'){
                      while(1){
                        char tecla = keypad.getKey();  
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Estas seguro?");
                        delay(100);                                           
                        if(tecla=='*'){
                          borrarAlarma(8);
                          lcd.setCursor(0,1);
                          lcd.print("lito");
                          delay(1000);
                          lcd.clear();                        
                          break;
                        }                    
                      }   
                      break;                   
                    }
                      delay(200);
                  }
                  break;
                  case'9':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Borrar alarma 9");
                    if (tecla=='*'){
                      while(1){
                        char tecla = keypad.getKey();  
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Estas seguro?");
                        delay(100);                                           
                        if(tecla=='*'){
                          borrarAlarma(9);
                          lcd.setCursor(0,1);
                          lcd.print("lito");
                          delay(1000);
                          lcd.clear();                        
                          break;
                        }                    
                      }   
                      break;                   
                    }
                      delay(200);
                  }
                  break;
                } 
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
  
          /*case'B':
          lcd.clear();
          while(1){
            char tecla = keypad.getKey(); 
            Temporizador( tecla); ///llama a la funcion temporizador y pasa el parametro de la tecla
            if(tecla=='#'){///si latecla es un # sale del loop
              break;
            }
          } */       
        }
        lcd.clear();      
        lcd.setCursor(0,0);
        lcd.print("A Alarma");
        //lcd.setCursor(0,1);
        //lcd.print("B Temporizador");
        delay(100);
        
        if(tecla=='#'){
          lcd.clear();
          break;
      }     
        //contarTiempo();///llama a la funcion contarTiempo para seguir ejecutando el temporizador en segundo plano
      }
    }
    
}
////////ordenamiento de burbuja
/*void bubbleSort(Alarm arr[], int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      // Comparar las alarmas utilizando la hora y el minuto
      if (arr[j].hour > arr[j + 1].hour || (arr[j].hour == arr[j + 1].hour && arr[j].minute > arr[j + 1].minute)) {
        // Intercambiar las alarmas si están en el orden incorrecto
        Alarm temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}*/

////////funciones de la alarma////////////
void borrarAlarma(int i){
  /*
   solo el parametro de la matris para que todo se vuelva 0 y por ende se elimine el dato
   */
  for(int a=0;a<3;a++){
    Alarma[i][a]=0;
  }
  
}

void ingresarHoraAlarma(int i) {
  /*
   se ingresa la posicion de la matris que fue seleccionada,
   y se ingresa la hora, minutos y segundos
   */
  int valor = 0;
  int indice = 0;
  char tecla = keypad.getKey();
  lcd.clear();
  while (indice <= 2) {
    if(indice==0){
      lcd.setCursor(0,0);
      lcd.print("Ingrese Hora");
      }
     else if(indice==1){        
        lcd.setCursor(0,0);
        lcd.print("Ingrese Minutos");
     }
     else if(indice==2){
        lcd.setCursor(0,0);
        lcd.print("Ingrese Segundos");
     }
    char tecla = keypad.getKey();  
    if (tecla >= '0' && tecla <= '9') {
      valor = valor * 10 + (tecla - '0'); 
      lcd.setCursor(0,1);
      lcd.print(valor);     
    } 
    else if (tecla == '*') {
      lcd.clear();  
       if(valor>23&&indice==0){
        lcd.setCursor(0,0);
         lcd.print("Hora imposible");
         lcd.setCursor(0,1);
         lcd.print("Intente de nuevo");
         delay(1000);               
        }    
       if(valor>59&&indice==1){
        lcd.setCursor(0,0);
        lcd.print("Minutos imposible");
        lcd.setCursor(0,1);
        lcd.print("Intente de nuevo");
        delay(1000);
        valor=0;
        indice=0;
        }
        else if(valor>59&&indice==2){
          lcd.setCursor(0,0);
          lcd.print("Segundos imposible");
          lcd.setCursor(0,1);
          lcd.print("Intente de nuevo");
          delay(1000);
          valor=0;
          indice=1;
        }
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
void Temporizador(char tecla){
  /*
   funcion principal del temporizador
   */
  lcd.setCursor(0, 0);  
  lcd.print("Temporizador");
  if (tecla) {
    switch (tecla) {///depende la tecla presionada, depende lo que va a hacer
      case 'A':
        temporizadorActivo = true; ///activa el temporizador
        break;
      case 'B':
        temporizadorActivo = false; ////frena el temporizador
        break;
      case 'C':
        resetTemporizador(); ///llama a la funcion
        break;
      case 'D':
        mostrarTiempo();///llama a la funcion
        break;
      default:
        ajustarTiempo(tecla); ///llama a la funcion y le pasa el parametro tecla
        break;
    }
  }
  
  if (temporizadorActivo) {
    contarTiempo();
    mostrarTiempo();
    if (!temporizadorActivo && horas == 0 && minutos == 0 && segundos == 0) {
    digitalWrite(Led, HIGH);  //prende el led si el temporisador llego a 0
    delay(5000);  
    digitalWrite(Led,LOW);
  }
  }
 
}
/////funciones del temporizador////////
void ajustarTiempo(char tecla) {
  /*
   ingreso de datos del temporizador
   */
int valor = tecla - '0';

if (valor >= 0 && valor <= 9) {
  if (horas < 10) {
    horas = horas * 10 + valor;
  } else if (minutos < 10) {
    minutos = minutos * 10 + valor;
  }
  }
}

void contarTiempo() {
  /*
   * cuenta el tiempo transcurrido 
   */
  if (segundos == 0) {
    if (minutos == 0) {
      if (horas == 0) {
        temporizadorActivo = false;
      } else {
        horas--;
        minutos = 59;
        segundos = 59;
      }
    } else {
      minutos--;
      segundos = 59;
    }
  } else {
    segundos--;
  }  
  delay(1000);
}

void mostrarTiempo() {
  /*
   * muestra el tiempo transcurrido en el lcd
   */
  lcd.setCursor(0, 1);
  lcd.print("Tiempo: ");
  if (horas < 10) {
    lcd.print("0");
  }
  lcd.print(horas);
  lcd.print(":");
  if (minutos < 10) {
    lcd.print("0");
  }
  lcd.print(minutos);
  lcd.print(":");
  if (segundos < 10) {
    lcd.print("0");
  }
  lcd.print(segundos);
}

void resetTemporizador() {
  /*
   * resetea el temporizador
   */
  horas = 0;
  minutos = 0;
  segundos = 0;
  temporizadorActivo = false;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temporizador");
}

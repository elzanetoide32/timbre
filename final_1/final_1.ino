#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <virtuabotixRTC.h> 
#include <Keypad.h> 

//Crea el objeto lcd direccion lcd 0x27 y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);

// Creation of the Real Time Clock Object
// Reset -> 11,CLK -> 13 , DAT -> 12,
virtuabotixRTC myRTC(12,11,10);

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

int alarma_0[3], alarma_1[3],alarma_2[3],alarma_3[3],alarma_4[3],alarma_5[3],alarma_6[3],alarma_7[3],alarma_8[3],alarma_9[3];////hasta 9 alarmas se puede configurar////
int alarm_Horas[]={alarma_0[0],alarma_1[0],alarma_2[0],alarma_3[0],alarma_4[0],alarma_5[0],alarma_6[0],alarma_7[0],alarma_8[0],alarma_9[0]};
int alarm_Min[]={alarma_0[1],alarma_1[1],alarma_2[1],alarma_3[1],alarma_4[1],alarma_5[1],alarma_6[1],alarma_7[1],alarma_8[1],alarma_9[1]};

#define Led 13
#define Boton A2
//////variables para el temporizador//////
int horas = 0;
int minutos = 0;
int segundos = 0;
bool temporizadorActivo = false;
int H,i=0;
void setup() {
  Serial.begin(9600);
  lcd.init();
  //Enciende la luz de fondo
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello Moto");
// Set the current date, and time in the following format:
// seconds, minutes, hours, day of the week, day of the month, month, year
  myRTC.setDS1302Time(00, 59, 23, 6, 10, 1, 2014);
  
  pinMode(Led,OUTPUT);
  pinMode(Boton,INPUT_PULLUP);
  delay(1000);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  // This allows for the update of variables for time or accessing the individual elements.                //|
    
    myRTC.updateTime();
    lcd.setCursor(0,0);
    lcd.print(myRTC.dayofmonth);
    lcd.print("/");
    lcd.print(myRTC.month);
    lcd.print("/");
    lcd.print(myRTC.year%100);
    lcd.print(" ");
    lcd.print(myRTC.hours);
    lcd.print(":");
    lcd.print(myRTC.minutes);
    lcd.print(":");
    lcd.print(myRTC.seconds);
    lcd.print(" ");    
    while(i<9){
      lcd.setCursor(0,1);
      lcd.print(alarm_Horas[i]);
      lcd.print(":");
      lcd.print(alarm_Min[i]);
      if(myRTC.hours==alarm_Horas[i]&&myRTC.minutes==alarm_Min[i]){
        i+=1; 
      }
    }
    delay(300);
    lcd.clear();   
   
    if(Boton==0){
      digitalWrite(Led,HIGH);
    }
    else if(myRTC.hours==0&&myRTC.minutes==0&&myRTC.seconds==0){
      digitalWrite(Led,LOW);
    }
    else if (myRTC.hours == alarma_0[0] && myRTC.minutes == alarma_0[1]&&myRTC.seconds==alarma_0[2]||myRTC.hours == alarma_1[0] && myRTC.minutes == alarma_1[1]&&myRTC.seconds==alarma_1[2]||myRTC.hours == alarma_2[0] && myRTC.minutes == alarma_2[1]&&myRTC.seconds==alarma_2[2]||myRTC.hours == alarma_3[0] && myRTC.minutes == alarma_3[1]&&myRTC.seconds==alarma_3[2]||myRTC.hours == alarma_4[0] && myRTC.minutes == alarma_4[1]&&myRTC.seconds==alarma_4[2]||myRTC.hours == alarma_5[0] && myRTC.minutes == alarma_5[1]&&myRTC.seconds==alarma_5[2]||myRTC.hours == alarma_6[0] && myRTC.minutes == alarma_6[1]&&myRTC.seconds==alarma_6[2]||myRTC.hours == alarma_7[0] && myRTC.minutes == alarma_7[1]&&myRTC.seconds==alarma_7[2]||myRTC.hours == alarma_8[0] && myRTC.minutes == alarma_8[1]&&myRTC.seconds==alarma_8[2]||myRTC.hours == alarma_9[0] && myRTC.minutes == alarma_9[1]&&myRTC.seconds==alarma_9[2]) {
      digitalWrite(Led, HIGH);  // Encender el LED cuando se alcance la hora de la alarma
      delay(5000);
      digitalWrite(Led,LOW);
      
      
    }
    else{
        digitalWrite(Led,LOW);
    }
    
    char tecla = keypad.getKey();    
    if(tecla=='*'){
      lcd.clear();
      while (1){
        char tecla = keypad.getKey();
        
        switch(tecla){
          case 'A':
                  lcd.clear();
                  while(1){ 
                    char tecla = keypad.getKey();         
                    lcd.setCursor(0,0);
                    lcd.print("C New alarm");
                    lcd.setCursor(0,1);
                    lcd.print("D Delete alarm");
                    switch(tecla){
                      case 'C':
                              lcd.clear();
                              while(1){
                                char tecla = keypad.getKey();                               
                                lcd.setCursor(0,0);
                                lcd.print("Ingrese la posicion");                             
                                switch(tecla){
                                  case '0':
                                  while(1){
                                    char tecla = keypad.getKey();
                                    lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(alarma_0);
                                      break;
                                    }
                                    delay(100);
                                  }                               
                                  break;
                                  case '1':
                                  while(1){
                                    char tecla = keypad.getKey();
                                    lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(alarma_1);                                 
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
                                    lcd.print("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(alarma_2);                                 
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
                                    lcd.print("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(alarma_3);                                 
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
                                    lcd.print("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(alarma_4);                                 
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
                                    lcd.print("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(alarma_5);                                 
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
                                    lcd.print("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(alarma_6);                                 
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
                                    lcd.print("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(alarma_1);                                 
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
                                    lcd.print("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(alarma_8);                                 
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
                                    lcd.print("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(alarma_9);                                 
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
              while(1){
                char tecla = keypad.getKey();
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Ingrese la posicion");
                switch(tecla){
                  case '0':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Confirme 2");
                    if (tecla=='*'){
                      H++;
                      if(H==2){
                        borrarAlarma(alarma_0);
                        lcd.setCursor(0,1);
                        lcd.print("lito");
                        delay(1000);
                        lcd.clear();
                        
                        break;
                      }                    
                    }
                    delay(200);
                  }
                  break;
                  case '1':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Confirme 2");
                    if (tecla=='*'){
                      H++;
                      if(H==2){
                        borrarAlarma(alarma_1);
                        lcd.setCursor(0,1);
                        lcd.print("lito");
                        delay(1000);
                        lcd.clear();
                        
                        break;
                      }                    
                    }
                    delay(200);
                  }
                  break;
                  case '2':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Confirme 2");
                    if (tecla=='*'){
                      H++;
                      if(H==2){
                        borrarAlarma(alarma_2);
                        lcd.setCursor(0,1);
                        lcd.print("lito");
                        delay(1000);
                        lcd.clear();
                        
                        break;
                      }                    
                    }
                    delay(200);
                  }
                  break;
                  case '3':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Confirme 2");
                    if (tecla=='*'){
                      H++;
                      if(H==2){
                        borrarAlarma(alarma_3);
                        lcd.setCursor(0,1);
                        lcd.print("lito");
                        delay(1000);
                        lcd.clear();
                        
                        break;
                      }                    
                    }
                    delay(200);
                  }
                  break;
                  case '4':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Confirme 2");
                    if (tecla=='*'){
                      H++;
                      if(H==2){
                        borrarAlarma(alarma_4);
                        lcd.setCursor(0,1);
                        lcd.print("lito");
                        delay(1000);
                        lcd.clear();
                        
                        break;
                      }                    
                    }
                    delay(200);
                  }
                  break;
                  case'5':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Confirme 2");
                    if (tecla=='*'){
                      H++;
                      if(H==2){
                        borrarAlarma(alarma_5);
                        lcd.setCursor(0,1);
                        lcd.print("lito");
                        delay(1000);
                        lcd.clear();
                        
                        break;
                      }                    
                    }
                    delay(200);
                  }
                  break;
                  case'6':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Confirme 2");
                    if (tecla=='*'){
                      H++;
                      if(H==2){
                        borrarAlarma(alarma_6);
                        lcd.setCursor(0,1);
                        lcd.print("lito");
                        delay(1000);
                        lcd.clear();
                        
                        break;
                      }                    
                    }
                    delay(200);
                  }
                  break;
                  case'7':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Confirme 2");
                    if (tecla=='*'){
                      H++;
                      if(H==2){
                        borrarAlarma(alarma_7);
                        lcd.setCursor(0,1);
                        lcd.print("lito");
                        delay(1000);
                        lcd.clear();
                        
                        break;
                      }                    
                    }
                    delay(200);
                  }
                  break;
                  case'8':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Confirme 2");
                    if (tecla=='*'){
                      H++;
                      if(H==2){
                        borrarAlarma(alarma_8);
                        lcd.setCursor(0,1);
                        lcd.print("lito");
                        delay(1000);
                        lcd.clear();
                        
                        break;
                      }                    
                    }
                    delay(200);
                  }
                  break;
                  case'9':
                  while(1){
                    char tecla = keypad.getKey();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Confirme 2");
                    if (tecla=='*'){
                      H++;
                      if(H==2){
                        borrarAlarma(alarma_9);
                        lcd.setCursor(0,1);
                        lcd.print("lito");
                        delay(1000);
                        lcd.clear();
                        
                        break;
                      }                    
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
  
          case'B':
          lcd.clear();
          while(1){
            char tecla = keypad.getKey(); 
            Temporizador( tecla); 
            if(tecla=='#'){
              break;
            }
          }        
        }
        lcd.clear();      
        lcd.setCursor(0,0);
        lcd.print("A Alarma");
        lcd.setCursor(0,1);
        lcd.print("B Temporizador");
        delay(100);
        if(tecla=='#'){
          break;
      }     
        contarTiempo();
        delay(100);
      }
    }
    
}


////////funciones de la alarma////////////
void borrarAlarma(int Alarma[2]){
  Alarma[0]=0;
  Alarma[1]=0;
  Serial.print("Alarma[0]=0");
  Serial.print("Alarma[1]=0");
}

void ingresarHoraAlarma(int Alarma[3]) {
  int valor = 0;
  int indice = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ingrese Hora");
  while (indice < 3) {
    char tecla = keypad.getKey();  
    if (tecla >= '0' && tecla <= '9') {
      valor = valor * 10 + (tecla - '0');      
      lcd.setCursor(0,1);
      lcd.print(valor);
    } 
    else if (tecla == '#') {
      lcd.clear();
      Alarma[indice] = valor;
      Serial.println();
      Serial.print("Alarma[");
      Serial.print(indice);
      Serial.print("] = ");
      Serial.println(valor);
      valor = 0;      
      indice++;
      if(indice==1){        
        lcd.setCursor(0,0);
        lcd.print("Ingrese Minutos");
     }
     else if(indice==2){
        lcd.setCursor(0,0);
        lcd.print("Ingrese Segundos");
     }
      
    }
        
  }
  
  
}
void Temporizador(char tecla){
  
  lcd.setCursor(0, 0);  
  lcd.print("Temporizador");
  if (tecla) {
    switch (tecla) {
      case 'A':
        temporizadorActivo = true;
        break;
      case 'B':
        temporizadorActivo = false;
        break;
      case 'C':
        resetTemporizador();
        break;
      case 'D':
        mostrarTiempo();
        break;
      default:
        ajustarTiempo(tecla);
        break;
    }
  }
  
  if (temporizadorActivo) {
    contarTiempo();
    mostrarTiempo();
    if (!temporizadorActivo && horas == 0 && minutos == 0 && segundos == 0) {
    digitalWrite(Led, HIGH);  // Activar el pin 13 (LED) cuando el tiempo se haya cumplido
    delay(5000);  
    digitalWrite(Led,LOW);
  }
  }
 
}
/////funciones del temporizador////////
void ajustarTiempo(char tecla) {
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
  horas = 0;
  minutos = 0;
  segundos = 0;
  temporizadorActivo = false;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temporizador");
}

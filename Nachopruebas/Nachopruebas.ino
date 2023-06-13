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

int Alarma[9][3];////matris de alarmas
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
  lcd.setCursor(3,0);
  lcd.print("Bienvenido");
// Set the current date, and time in the following format:
// seconds, minutes, hours, day of the week, day of the month, month, year
  myRTC.setDS1302Time(00, 02, 15, 6, 13, 6, 2023);
  
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
   
    if(Boton==0){
      digitalWrite(Led,HIGH);
    }
    else if(myRTC.hours==0&&myRTC.minutes==0&&myRTC.seconds==0){
      digitalWrite(Led,LOW);
    }    
    else{
        digitalWrite(Led,LOW);
    }
    for(int i=0;i<9;i++){
      if(myRTC.hours==0&&myRTC.minutes==0&&myRTC.seconds==0){
      digitalWrite(Led,LOW);
      }
      else if(myRTC.hours==Alarma[i][0]&&myRTC.minutes==Alarma[i][1]&&myRTC.seconds==Alarma[i][2]){
        digitalWrite(Led, HIGH);  // Encender el LED cuando se alcance la hora de la alarma
        delay(5000);
        digitalWrite(Led,LOW);  
      }
    }

    // Variables para almacenar la próxima alarma
  int proximaAlarmaHora = -1;
  int proximaAlarmaMinuto = -1;
  int proximaAlarmaSecond = -1;
  // Comparar la hora con los horarios en el array
  for (int i = 0; i < 9; i++) {
    if (myRTC.hours < Alarma[i][0] || (myRTC.hours == Alarma[i][0] && myRTC.hours < Alarma[i][1])) {
      proximaAlarmaHora = Alarma[i][0];
      proximaAlarmaMinuto = Alarma[i][1];
      proximaAlarmaSecond=Alarma[i][2];
      break; // Romper el bucle una vez que se encuentre la próxima alarma
    }
  }

  // Mostrar la próxima alarma en el LCD
  lcd.setCursor(0, 1);
  if (proximaAlarmaHora != -1 && proximaAlarmaMinuto != -1) {
    lcd.print(proximaAlarmaHora);
    lcd.print(":");
    if (proximaAlarmaMinuto < 10) {
      lcd.print("0"); // Asegurarse de que los minutos sean mostrados con 2 dígitos
    }
    lcd.print(proximaAlarmaMinuto);
    lcd.print(":");
    if (proximaAlarmaSecond < 10) {
      lcd.print("0"); // Asegurarse de que los minutos sean mostrados con 2 dígitos
    }
    lcd.print(proximaAlarmaSecond);
  } else {
    lcd.print("No hay alarmas");
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
                                lcd.scrollDisplayLeft();
                                delay(500);
                                switch(tecla){
                                  case '0':
                                  while(1){
                                    char tecla = keypad.getKey();
                                    lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print("Confirme");
                                    if (tecla=='*'){
                                      ingresarHoraAlarma(0);
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
                                      ingresarHoraAlarma(1);                                 
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
                                      ingresarHoraAlarma(2);                                 
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
                                      ingresarHoraAlarma(3);                                 
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
                                      ingresarHoraAlarma(4);                                 
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
                                      ingresarHoraAlarma(5);                                 
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
                                      ingresarHoraAlarma(6);                                 
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
                                      ingresarHoraAlarma(7);                                 
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
                                      ingresarHoraAlarma(8);                                 
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
                                      ingresarHoraAlarma(9);                                 
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
                lcd.scrollDisplayLeft();
                delay(500);
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
                        borrarAlarma(0);
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
                        borrarAlarma(1);
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
                        borrarAlarma(2);
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
                        borrarAlarma(3);
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
                        borrarAlarma(4);
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
                        borrarAlarma(5);
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
                        borrarAlarma(6);
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
                        borrarAlarma(7);
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
                        borrarAlarma(8);
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
                        borrarAlarma(9);
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
void borrarAlarma(int i){
  for(int a=0;a<3;a++){
    Alarma[i][a]=0;
  }
  
}

void ingresarHoraAlarma(int i) {
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
    else if (tecla == '*') {
      lcd.clear();
      Alarma[i][indice] = valor;
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

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte filas = 4;
const byte columnas = 4;

char teclas[filas][columnas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pinesFilas[filas] = {9, 8, 7, 6};
byte pinesColumnas[columnas] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas);

int horas = 0;
int minutos = 0;
int segundos = 0;
bool temporizadorActivo = false;
const int pinLed = 13;
void setup() {
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temporizador");
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, LOW);
}

void loop() {
  
  char tecla = keypad.getKey();
  
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
    digitalWrite(pinLed, HIGH);  // Activar el pin 13 (LED) cuando el tiempo se haya cumplido
    delay(5000);
    digitalWrite(pinLed,LOW);
  }
  }
  
}

void ajustarTiempo(char tecla) {
  int valor = tecla - '0';
  
  if (valor >= 0 && valor <= 9) {
    if (horas < 99) {
      if (minutos < 60) {
        minutos = minutos * 10 + valor;
      } else {
        horas = horas * 10 + valor;
      }
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

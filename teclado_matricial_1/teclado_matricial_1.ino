#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
const byte filas = 4;       // Número de filas del teclado matricial
const byte columnas = 4;    // Número de columnas del teclado matricial
LiquidCrystal_I2C lcd(0x27,16,2);
char teclas[filas][columnas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pinesFilas[filas] = {9, 8, 7, 6};         // Pines conectados a las filas
byte pinesColumnas[columnas] = {5, 4, 3, 2};   // Pines conectados a las columnas

Keypad keypad = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas);

void setup() {
  Serial.begin(9600);
  lcd.init();
  //Enciende la luz de fondo
  lcd.backlight();
}

void loop() {
  char tecla = keypad.getKey();
  
  if (tecla) {
    lcd.clear();
    lcd.print(tecla);
    if(tecla=='A'){
      Serial.print("vamos a tu casa");
    }
  }

}

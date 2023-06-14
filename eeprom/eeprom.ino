#include <EEPROM.h>

const unsigned char PROGMEM ALFANUMERICO[] =
{
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'Ñ', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
  'i', 'j', 'k', 'l', 'm', 'n', 'ñ', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'x', 'y',
  'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', '-', '_', ':', ';',
  '¿', '?', '(', ')', '[', ']', '{', '}', '=', '&', '"', ' '
};
// Matriz de horas del RTC
int rtcHours[9][3] = {
  {10, 30, 15},
  {8, 15, 20},
  {12, 0, 45},
  {16, 45, 5},
  {9, 20, 10},
  {14, 0, 0},
  {7, 30, 55},
  {11, 0, 30},
  {13, 45, 25}
};
void setup() {
  Serial.begin(9600);
}

void loop() {
  // Leemos el array de la memoria flash, byte a byte, lo mostramos por el puerto
  // serie y a la vez lo grabamos en la eeprom.
  for (int i=0; i<9; i++) {
    for(int j=0;j<3;j++){
       EEPROM.update(i, (byte)pgm_read_byte(rtcHours[i][j]));
      Serial.print(" ");
    }
    //Serial.print((char)pgm_read_byte(ALFANUMERICO+i));
   
}
  Serial.println();
  delay(5000);

//Leemos el array de la EEPROM y lo mostramos por el puerto serie.
  for (int i=0; i<9; i++) {
    Serial.print(EEPROM.read(i));
    Serial.print(" ");
  }
  Serial.println();
}

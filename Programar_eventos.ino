#include <Wire.h>

uint8_t second, minute, hour, wday, day, month, year, ctrl;
void setup() {

  Serial.begin(9600);
  Wire.begin();
  pinMode(13,OUTPUT);
  Serial.println("----------------------------------------------------------------------");
  Serial.println("EJEMPLO DE PROGRAMADO DE EVENTOS CON RTC DS1307 EN TINYRTC CON ARDUINO");
  Serial.println("----------------------------------------------------------------------");
}

void loop() {
  
  delay(1000);

  // Leer los registros del RTC
  if (read_ds1307()) {
    // Mostrar la fecha y hora
    print_time();
  } else {
    // No se puede leer desde le DS1307 (NACK en I2C)
    Serial.println("No se detecta el DS1307, revisar conexiones");
  }


 //AQui empieza la locura 
read_ds1307();
if (hour>= 15 && hour<16 )
  {
    if(minute >10 && minute<13)  // si funciono pero encendio en el minuto 4 
    {
    digitalWrite(13,HIGH);
    delay(100);
    }
    else
  {
    digitalWrite(13,LOW);
  }
  }
  
  
}

/**
   Esta funcion establece la cominicación con el DS1307 y lee los registros
   de fecha y hora. Entrega la informacion horaria en las variables globales
   declaradas al principio del sketch.
*/
bool read_ds1307()
{
  // Iniciar el intercambio de información con el DS1307 (0xD0)
  Wire.beginTransmission(0x68);

  // Escribir la dirección del segundero
  Wire.write(0x00);

  // Terminamos la escritura y verificamos si el DS1307 respondio
  // Si la escritura se llevo a cabo el metodo endTransmission retorna 0
  if (Wire.endTransmission() != 0)
    return false;

  // Si el DS1307 esta presente, comenzar la lectura de 8 bytes
  Wire.requestFrom(0x68, 8);

  // Recibimos el byte del registro 0x00 y lo convertimos a binario
  second = bcd2bin(Wire.read());
  minute = bcd2bin(Wire.read()); // Continuamos recibiendo cada uno de los registros
  hour = bcd2bin(Wire.read());
  wday = bcd2bin(Wire.read());
  day = bcd2bin(Wire.read());
  month = bcd2bin(Wire.read());
  year = bcd2bin(Wire.read());

  // Recibir los datos del registro de control en la dirección 0x07
  ctrl = Wire.read();

  // Operacion satisfactoria, retornamos verdadero
  return true;
}

/**
   Esta función convierte un número BCD a binario. Al dividir el número guardado
   en el parametro BCD entre 16 y multiplicar por 10 se convierten las decenas
   y al obtener el módulo 16 obtenemos las unidades. Ambas cantidades se suman
   para obtener el valor binario.
*/
uint8_t bcd2bin(uint8_t bcd)
{
  // Convertir decenas y luego unidades a un numero binario
  return (bcd / 16 * 10) + (bcd % 16);
}

/**
   Imprime la fecha y hora al monitor serial de arduino
*/
void print_time()
{
  Serial.print("Fecha: ");
  Serial.print(day);
  Serial.print('/');
  Serial.print(month);
  Serial.print('/');
  Serial.print(year);

  Serial.print("  Hora: ");
  Serial.print(hour);
  Serial.print(':');
  Serial.print(minute);
  Serial.print(':');
  Serial.print(second);

  Serial.println();
}

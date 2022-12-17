/* 
 * es-sumarRestar.ino - Usando la librería LedRGB
 * Usa las funciones "sumar" y "restar" de la librería, que necesitan parámetros RGB:
 * - 200ms BLANCO (SÓLO AL PRINCIPIO)
 * 
 * - 700ms AMARILLO
 * - 700ms VERDE
 * - 300ms APAGADO
 * - 700ms ROJO
 * - 700ms VIOLETA CLARO
 * - 700ms BLANCO
 * 
 * Por Alejandro Bertinelli
 * 
 * Escrito el 29 de Junio de 2017
 * Última actualización el 22 de Junio de 2019
 */
 
/*
 * LICENCIA MIT License 3.0
 */
#include <LedRGB.h>
LedRGB miLED(9, 10, 11, CC);

void setup() {
  miLED.ponerColor(255, 255, 255);  //  Al principio ponemos el color a blanco
  delay(200);                       //  Esperamos 200ms así hasta empezar el loop
}

void loop() {
  // Usamos la función "restar"
  miLED.restar(0, 0, 255);    //  Restamos 0 a R, 0 a G, y 255 a B
  delay(700);                 //  Esperamos 700ms así
  miLED.restar(255, 0, 0);    //  Restamos 255 a R, 0 a G, y 0 a B
  delay(700);                 //  Esperamos 700ms así
  miLED.restar(0, 255, 0);    //  Restamos 0 a R, 255 a G, y 0 a B
  delay(300);                 //  Esperamos 300ms así
  miLED.sumar(255, 0, 0);     //  Sumamos 255 a R, 0 a G, y 0 a B
  delay(700);                 //  Esperamos 700ms así
  miLED.sumar(0, 100, 255);   //  Sumamos 0 a R, 100 a G, y 255 a B
  delay(700);                 //  Esperamos 700ms así
  miLED.sumar(0, 155, 0);     //  Sumamos 0 a R, 155 a G, y 0 a B
  delay(700);                 //  Esperamos 700ms así
}

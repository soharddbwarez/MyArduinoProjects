/* 
 * es-parpadeoRGBsimple.ino
 * Prueba todos los diodos R, G y B del LED:
 * -300ms ROJO
 * -300ms VERDE
 * -300ms AZUL
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


// Iniciamos el objeto
LedRGB holi(9, 10, 11, CC);
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // Hacemos que rojo, verde y azul estén 300 milisegundos encendidos
  holi.ponerColor(255, 0, 0);
  delay(300);
  holi.ponerColor(0, 255, 0);
  delay(300);
  holi.ponerColor(0, 0, 255);
  delay(300);
}

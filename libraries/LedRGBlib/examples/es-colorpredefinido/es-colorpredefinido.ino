/* 
 * es-colorpredefinido.ino - Usando la librería LedRGB
 * Muestra la función "colorPredefinido"
 * 
 * Por Alejandro Bertinelli
 * 
 * Escrito el 22 de Junio de 2019
 * Última actualización el 22 de Junio de 2019
 */
 
/*
 * LICENCIA MIT License 3.0
 */
#include <LedRGB.h>
LedRGB miLED(9, 10, 11, CC);

void setup() {

}

void loop() {
  // Todos los colores que hay disponibles se muestran a continuación:
  miLED.colorPredefinido("blanco");
  delay(300);
  miLED.colorPredefinido("rojo");
  delay(300);
  miLED.colorPredefinido("verde");
  delay(300);
  miLED.colorPredefinido("azul");
  delay(300);
  miLED.colorPredefinido("amarillo");
  delay(300);
  miLED.colorPredefinido("naranja");
  delay(300);
  miLED.colorPredefinido("violeta");
  delay(300);
  miLED.colorPredefinido("turquesa");
  delay(300);
  miLED.colorPredefinido("gris");
  delay(300);
  miLED.colorPredefinido("granate");
  delay(300);
  miLED.colorPredefinido("fucsia");
  delay(300);
}

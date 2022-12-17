/*
LedRGB.h - Easy to use RGB LEDs with Arduino
Created by Alejandro Bertinelli, 27-06-2017
Modified 22-06-2019
Version 1.0.0
*/
#ifndef LedRGB_h
#define LedRGB_h
#include <arduino.h>

#define CA 0
#define CC 1

class LedRGB 
{
  public:
   LedRGB(int pinR, int pinG, int pinB, int modo);
   void ponerColor(int CR, int CG, int CB);
   void setColor(int CR, int CG, int CB);
   void ponerColorCA(int crA, int cgA, int cbA);
   void ponerColorCC(int crC, int cgC, int cbC);
   void colorPredefinido(String color);
   void defaultColor(String color);
   void sumar(int sumR, int sumG, int sumB);
   void add(int addR, int addG, int addB);
   void restar(int restR, int restG, int restB);
   void substract(int restR, int restG, int restB);
   void apagar();
   void off();
   void testear();
   void test();
   void efectoFlash(int delON, int delOFF);
   void flashEffect(int delON, int delOFF);
   void efectoFade(int fR, int fG, int fB);
   void fadeEffect(int fR, int fG, int fB);
   
  private:  
   int _pinR;
   int _pinG;
   int _pinB;
   int _modo;
   
   int _CR;
   int _CG;
   int _CB;
   
   int _crA;
   int _cgA;
   int _cbA;
   int _crC;
   int _cgC;
   int _cbC;
   
   int _sumR;
   int _sumG;
   int _sumB;
   int _restR;
   int _restG;
   int _restB;
   
   int _delON;
   int _delOFF;
   
   int _fR;
   int _fG;
   int _fB;
   int _valR = 0;
   int _valG = 0;
   int _valB = 0;
	
	String _color;
	String _colorFade;
};
#endif
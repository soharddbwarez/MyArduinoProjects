/*
LedRGB.cpp - Easy to use RGB LEDs with Arduino
Created by Alejandro Bertinelli, 27-06-2017
Modified 22-06-2019
Version 1.0.0
*/

#include <arduino.h>
#include <LedRGB.h>

LedRGB::LedRGB(int pinR, int pinG, int pinB, int mode)
{
	_pinR = pinR;
	_pinG = pinG;
	_pinB = pinB;
	pinMode(_pinR, OUTPUT);
	pinMode(_pinG, OUTPUT);
	pinMode(_pinB, OUTPUT);
	_modo = mode;
}
void LedRGB::ponerColor(int CR, int CG, int CB){
	_CR = CR;
	_CG = CG;
	_CB = CB;
	if(_modo == 0){
		ponerColorCA(_CR, _CG, _CB);
	}else if(_modo == 1){
		ponerColorCC(_CR, _CG, _CB);
	}
}
void LedRGB::setColor(int CR, int CG, int CB){
	ponerColor(CR, CG, CB);
}

void LedRGB::ponerColorCA(int crA, int cgA, int cbA){
	_crA = crA;
	_cgA = cgA;
	_cbA = cbA;
	analogWrite(_pinR, 255-_crA);
	analogWrite(_pinG, 255-_cgA);
	analogWrite(_pinB, 255-_cbA);
}
void LedRGB::ponerColorCC(int crC, int cgC, int cbC){
	_crC = crC;
	_cgC = cgC;
	_cbC = cbC;
	analogWrite(_pinR, _crC);
	analogWrite(_pinG, _cgC);
	analogWrite(_pinB, _cbC);
}

void LedRGB::colorPredefinido(String color){
	_color = color;
	if(_color == "blanco"){
		ponerColor(255, 255, 255);
	}else if(_color == "rojo"){
		ponerColor(255, 0, 0);
	}else if(_color == "verde"){
		ponerColor(0, 255, 0);
	}else if(_color == "azul"){
		ponerColor(0, 0, 255);
	}else if(_color == "amarillo"){
		ponerColor(255, 255, 0);
	}else if(_color == "naranja"){
		ponerColor(255, 150, 0);
	}else if(_color == "violeta"){
		ponerColor(150, 0, 255);
	}else if(_color == "turquesa"){
		ponerColor(0, 255, 220);
	}else if(_color == "gris"){
		ponerColor(70, 70, 70);
	}else if(_color == "granate"){
		ponerColor(176, 54, 72);
	}else if(_color == "fucsia"){
		ponerColor(255, 0, 255);
	}else{
		ponerColor(0, 0, 0);
	}
}
void LedRGB::defaultColor(String color){
	_color = color;
	if(_color == "white"){
		ponerColor(255, 255, 255);
	}else if(_color == "red"){
		ponerColor(255, 0, 0);
	}else if(_color == "green"){
		ponerColor(0, 255, 0);
	}else if(_color == "blue"){
		ponerColor(0, 0, 255);
	}else if(_color == "yellow"){
		ponerColor(255, 255, 0);
	}else if(_color == "orange"){
		ponerColor(255, 150, 0);
	}else if(_color == "violet"){
		ponerColor(150, 0, 255);
	}else if(_color == "turquoise"){
		ponerColor(0, 255, 220);
	}else if(_color == "grey"){
		ponerColor(70, 70, 70);
	}else if(_color == "maroon"){
		ponerColor(176, 54, 72);
	}else if(_color == "fuchsia"){
		ponerColor(255, 0, 255);
	}else{
		ponerColor(0, 0, 0);
	}
}

void LedRGB::sumar(int sumR, int sumG, int sumB){
	_sumR = sumR;
	_sumG = sumG;
	_sumB = sumB;
	ponerColor(_CR+_sumR, _CG+_sumG, _CB+_sumB);
}
void LedRGB::add(int addR, int addG, int addB){
	sumar(addR, addG, addB);
}

void LedRGB::restar(int restR, int restG, int restB){
	_restR = restR;
	_restG = restG;
	_restB = restB;
	ponerColor(_CR-_restR, _CG-_restG, _CB-_restB);
}
void LedRGB::substract(int restR, int restG, int restB){
	restar(restR, restG, restB);
}

void LedRGB::testear(){
	ponerColor(255, 0, 0);
	delay(300);
	ponerColor(0, 255, 0);
	delay(300);
	ponerColor(0, 0, 255);
	delay(300);
	ponerColor(255, 255, 255);
	delay(500);
	ponerColor(0, 0, 0);
}
void LedRGB::test(){
	testear();
}

void LedRGB::apagar(){
	ponerColor(0, 0, 0);
}
void LedRGB::off(){
	setColor(0, 0, 0);
}

void LedRGB::efectoFlash(int delON, int delOFF){
	_delON = delON;
	_delOFF = delOFF;
	ponerColor(255, 255, 255);
	delay(_delON);
	ponerColor(0, 0, 0);
	delay(_delOFF);
}

void LedRGB::flashEffect(int delON, int delOFF){
	efectoFlash(delON, delOFF);
}


void LedRGB::efectoFade(int fR, int fG, int fB){
	_fR = fR;
	_fG = fG;
	_fB = fB;
	
	while (_valR <= _fR) {
		_valR += 1;
	}
	while (_valG <= _fG) {
		_valG += 1;
	}
	while (_valB <= _fB) {
		_valB += 1;
	}
	ponerColor(_valR, _valG, _valB);
	delay(15);
}

void LedRGB::fadeEffect(int fR, int fG, int fB){
	efectoFade(fR, fG, fB);
}

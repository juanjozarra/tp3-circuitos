/*
 * generador.c
 *
 * Created: 14/6/2021 15:12:53
 *  Author: tomas
 */ 

#include "generador.h"
#include <avr/io.h>

volatile unsigned short GENERADOR_frec=100; //Variable global donde se almacena la frecuencia. Por defecto 100.

void GENERADOR_turnOn(){
		DDRB = (1<<PINB1); //PINB0 SALIDA
		TCCR1A = (1<<COM1A0); //COM1A TOGGLE
		TCCR1B = (1<<WGM12)|(1<<CS11); //Modo CTC 8 prescaler
		GENERADOR_refreshFrecuency();
}

void GENERADOR_turnOff(){
	TCCR1A = 0;
	TCCR1B = 0;	
}

void GENERADOR_refreshFrecuency (){
	OCR1A= 1000000/GENERADOR_frec-1;
}


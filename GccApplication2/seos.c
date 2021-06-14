/*
 * CFile1.c
 *
 * Created: 14/06/2021 15:17:03
 *  Author: Juanjo
 */ 


#include "seos.h"
#include "serialPort.h"
#include "menu.h"


static unsigned char Task_Flag=0;
static unsigned char Serial_Flag=0;


void SEOSTimer0Init(){
	OCR0A = 78; //Valor con el cual comparar
	TCCR0A = (1<<WGM01);// = 0b00000010; Modo CTC
	TCCR0B = (1<<CS02)|(1<<CS00); // = 0b00000101; clk/1024 (From Prescaler) 16MHz/1024 =  15625 Hz
	TIMSK0 = (1<<OCIE0A); // Habilita el comparador. T=78/15625 Hz ~= 5 ms 
}


void SEOSDispatcherTasks(){
	if(Task_Flag){
		Menu_Update(); //aplicacion
		Task_Flag = 0;
	}
	if(Serial_Flag){
		SerialPort_Update(); //driver UART
		Serial_Flag = 0;
	}	
}

void SEOSGoToSleep(void){
	//PONER EN IDLE
}

ISR(TIMER0_COMPA_vect){	
	static char serialCont=0, taskCount=0;
	
	if(++serialCont==1)	{
		Serial_Flag = 1;
		serialCont=0;
	}
	if(++taskCount==20){
		Task_Flag=1;
		taskCount=0;
	}
}

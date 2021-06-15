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
static unsigned char Reader_Flag=0;


void SEOSTimer0Init(){
	OCR0A = 77; //Valor con el cual comparar
	TCCR0A = (1<<WGM01);// = 0b00000010; Modo CTC
	TCCR0B = (1<<CS02)|(1<<CS00); // = 0b00000101; clk/1024 (From Prescaler) 16MHz/1024 =  15625 Hz
	TIMSK0 = (1<<OCIE0A); // Habilita el comparador. T=78/15625 Hz ~= 5 ms 
}


void SEOSDispatcherTasks(){
	if(Task_Flag){
		Menu_Print(); //aplicacion
		Task_Flag = 0;
	}
	if(Serial_Flag){
		SerialPort_Update(); //driver UART
		Serial_Flag = 0;
	}
	if(Reader_Flag){
		Menu_Update();
		Reader_Flag = 0;
	}	
}

void SEOSGoToSleep(void){
	//PONER EN IDLE
}

ISR(TIMER0_COMPA_vect){	
	static char taskCount=0,readerCount=10;
	
	Serial_Flag = 1;
	
	if(++readerCount==10){
		Reader_Flag=1;
		readerCount=0;
	}

	if(++taskCount==60){
		Task_Flag=1;
		taskCount=0;
	}
}

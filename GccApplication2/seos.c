/*
 * CFile1.c
 *
 * Created: 14/06/2021 15:17:03
 *  Author: Juanjo
 */ 


#include "seos.h"
#include "uart.h"
#include "menu.h"


static unsigned char Flag_PrintMenu=0;
static unsigned char Flag_UART=0;
static unsigned char Flag_MenuUpdate=0;

/* Funcion que inicializa el timer0 en modo CTC para generar una fuente de interrupcion periodica. Tal como
lo indican las siglas CTC (Clear Timer on Compare match) el sistema contabiliza un tiempo dentro de un contador
y cuando este contador alcanza el valor del registro de los timers, se ejecuta la interrupcion del microcontrolador.
 */
void SEOSTimer0Init(){
	OCR0A = 77; //Valor con el cual comparar
	TCCR0A = (1<<WGM01);// = 0b00000010; Modo CTC
	TCCR0B = (1<<CS02)|(1<<CS00); // = 0b00000101; clk/1024 (From Prescaler) 16MHz/1024 =  15625 Hz
	TIMSK0 = (1<<OCIE0A); // Habilita la interrupcion del comparador. T=78/15625 Hz ~= 5 ms 
}


//El dispatcher es el encargado de asignar el proceso al procesador. Dependiendo el flag que esté activo, se asigna el proceso correspondiente
void SEOSDispatcherTasks(){
	if(Flag_PrintMenu){
		Menu_Print(); //aplicacion imprimir Menu
		Flag_PrintMenu = 0;
	}
	if(Flag_UART){
		SerialPort_Update(); //driver UART
		Flag_UART = 0;
	}
	if(Flag_MenuUpdate){
		Menu_Update(); //aplicacion actualizar el menu
		Flag_MenuUpdate = 0;
	}	
}

void SEOSGoToSleep(void){
	//PONER EN IDLE
}


//Rutinas de servicio de interrupciones
ISR(TIMER0_COMPA_vect){	
	static char taskCount=0,readerCount=10;
	
	Flag_UART = 1; //Cada 1 tick = 5ms atendemos al periferico UART
	
	if(++readerCount==10){ //Cada 10 ticks = 50ms, activo el flag MenuUpdate
		Flag_MenuUpdate=1;
		readerCount=0;
	}

	if(++taskCount==60){ //Cada 60 ticks = 300ms , activo el flag PrintMenu
		Flag_PrintMenu=1;
		taskCount=0;
	}
}

/*
 * GccApplication2.c
 *
 * Created: 12/05/2021 14:10:27
 * Author : Juanjo
 */ 


#include "uart.h"
#include "generador.h"
#include "seos.h"
#include "menu.h"
#define BR9600 (0x67)	// 0x67=103 configura BAUDRATE=9600@16MHz

int main(void)
{
	SerialPort_Init(BR9600); 		// Inicializo formato 8N1 y BAUDRATE = 9600bps
	SerialPort_TX_Enable();			// Activo el Transmisor del Puerto Serie
	SerialPort_RX_Enable();			// Activo el Receptor del Puerto Serie
	SEOSTimer0Init();				//Inicializo el timer0
	sei();							//Habilito las interrupciones


	while(1)
	{		
		SEOSDispatcherTasks();
	}
	return 0;
}
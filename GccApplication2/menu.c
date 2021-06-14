/*
 * CFile1.c
 *
 * Created: 14/06/2021 17:13:26
 *  Author: Juanjo
 */ 

#include "menu.h"
#include "serialPort.h"

volatile unsigned char print_Menu = 0;

char msg [3][64]  = {{"Generador de seniales digitales programable.\r\0"},{"Ingrese frecuencia entre 100 y 10000Hz\r\0"},{"ON: para encender.\rOFF: para apagar.\rRST: para reiniciar.\r\0"}};

void Menu_init(void){
	print_Menu = 1;
}

void Menu_Update(void){
		if(print_Menu){
			static unsigned char step = 0;
			SerialPort_Write_String_To_Buffer(msg[step]);
			step++;
			if(step==3) {step = 0;
			print_Menu =0;
			}
		}
}
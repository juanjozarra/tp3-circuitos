/*
 * CFile1.c
 *
 * Created: 14/06/2021 17:13:26
 *  Author: Juanjo
 */ 

#include "menu.h"
#include "serialPort.h"

void Menu_init(void){
	SerialPort_Write_String_To_Buffer("Generador de seniales digitales programable\n\rIngrese frecuencia entre 100 y 10000Hz\n\rON: para encender, OFF para apagar, RST para reiniciar");
}

void Menu_Update(void){
	SerialPort_Write_String_To_Buffer("n\r\Hola, esto funciona");
	/*char ch;
	if (SerialPort_Get_Char_From_Buffer( &ch)){
		SerialPort_Write_Char_To_Buffer(ch);
		SerialPort_Write_String_To_Buffer("n\r\ >: ");
	}*/
}
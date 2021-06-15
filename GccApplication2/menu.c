/*
 * CFile1.c
 *
 * Created: 14/06/2021 17:13:26
 *  Author: Juanjo
 */ 

#include "menu.h"
#include "serialPort.h"
#include "string.h"

volatile unsigned char print_Menu = 0;

static char msg [3][64]  = {{"Generador de seniales digitales programable.\r\0"},{"Ingrese frecuencia entre 100 y 10000Hz\r\0"},{"ON: para encender.\rOFF: para apagar.\rRST: para reiniciar.\r\0"}};
static char command [5] = "    ";
static unsigned char indice_command=0;
static unsigned char too_long = 0;
static char cmds [3][5] = {{"ON   "},{"OFF  "},{"RST  "}};

void Menu_options(void){
	print_Menu = 1;
}

void Menu_Print(void){
		if(print_Menu){
			static unsigned char step = 0;
			SerialPort_Write_String_To_Buffer(msg[step]);
			step++;
			if(step==3) {step = 0;
			print_Menu =0;
			}
		}
}

void Menu_Update(){
	char aux;
	if(!print_Menu){
		if(SerialPort_Get_Char_From_Buffer(&aux)){ //Si recibio un caracter
			SerialPort_Write_Char_To_Buffer(aux);
			if(aux == 'a'){  //Si apreto enter
				Menu_Command();
			}else{
				if(indice_command<5){ //El comando más largo es de 5 caracteres
					command [indice_command] = aux;
					indice_command++;
				}else {	//Ingreso más de 5 caracteres, no va a ser un comando valido
				too_long = 1;			
				}
			}
		}
	}
}

static void Menu_Command(){
	if(!too_long){
		
		if(strcmp(&command,cmds[0])){
			
		}
		
		
	}
	
	//Reinicializo para el siguiente comando
	too_long = 0;
	for(unsigned char i = 0 ; i<5 ; i++){
		command [i] = ' ';
	}
	indice_command = 0;
}
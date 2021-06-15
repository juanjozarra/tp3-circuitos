/*
 * CFile1.c
 *
 * Created: 14/06/2021 17:13:26
 *  Author: Juanjo
 */ 

#include "menu.h"
#include "serialPort.h"
#include "stdlib.h"
#include "generador.h"

extern unsigned short GENERADOR_frec;

static unsigned char print_Menu = 1;
static char msg [3][64]  = {{"Generador de seniales digitales programable.\r\0"},{"Ingrese frecuencia entre 100 y 10000Hz\r\0"},{"ON: para encender.\rOFF: para apagar.\rRST: para reiniciar.\r\0"}};
static char command [5] = {0,0,0,0,0};
static unsigned char indice_command=0;
static unsigned char too_long = 0;
static char cmds [3][5] = {{"ON"},{"OFF"},{"RST"}};

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
			if(aux == '\r'){  //Si apreto enter
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

void Menu_Command(){
	if(!too_long){
		if(command[0]==cmds[0][0]&&command[1]==cmds[0][1]&&command[2]==cmds[0][2]&&command[3]==cmds[0][3]&&command[4]==cmds[0][4]){
			GENERADOR_turnOn();
		} else
		if(command[0]==cmds[1][0]&&command[1]==cmds[1][1]&&command[2]==cmds[1][2]&&command[3]==cmds[1][3]&&command[4]==cmds[1][4]){
			GENERADOR_turnOff();
		} else
		if(command[0]==cmds[2][0]&&command[1]==cmds[2][1]&&command[2]==cmds[2][2]&&command[3]==cmds[2][3]&&command[4]==cmds[2][4]){
			GENERADOR_turnOff();
			GENERADOR_frec = 100;		
			print_Menu = 1;						
		} else{
		unsigned long frec = atoi(command);
		if((frec>=100)&&(frec<=10000)){
			GENERADOR_frec = frec;
			GENERADOR_refreshFrecuency();
		}else{
			SerialPort_Write_String_To_Buffer("Comando No Valido.\r\r");
		}
		}
	}	
	else{
		SerialPort_Write_String_To_Buffer("Comando No Valido.\r\r");
	}
	//Reinicializo para el siguiente comando
	too_long = 0;
	//strcpy(command,"\0\0\0\0\0");
	command[0] = 0;
	command[1] = 0;
	command[2] = 0;
	command[3] = 0;
	command[4] = 0;
	indice_command = 0;
}
/*
 * CFile1.c
 *
 * Created: 14/06/2021 17:13:26
 *  Author: Juanjo
 */ 

#include "menu.h"
#include "uart.h"
#include "stdlib.h"
#include "generador.h"

extern unsigned short GENERADOR_frec; //Variable global usada para representar la frecuencia que se generará en el Generador de señales.

static unsigned char print_Menu = 1; //Variable que usamos para indicar si se debe mostrar el menu 
static char msg [3][64]  = {{"Generador de seniales digitales programable.\r\0"},{"Ingrese frecuencia entre 100 y 10000Hz\r\0"},{"ON: para encender.\rOFF: para apagar.\rRST: para reiniciar.\r\0"}}; //Definimos los mensajes que componen al menu, los dividimos en 3 "frases" de 64 bits cada una por cuestiones de longitud.  
static char command [5] = {0,0,0,0,0}; //variable que usamos para almacenar el comando ingresado por el usuario
static unsigned char indice_command=0; //indice para recorrer la variable comando
static unsigned char too_long = 0; //variable que usamos para indicar si el comando que ingresó el usuario supera la longitud permitida
static char cmds [3][5] = {{"ON"},{"OFF"},{"RST"}}; //Matriz que contiene los comandos validos que el usuario puede ingresar

		/*Chequeamos si debemos imprimir el menu, ya sea porque inició el programa o el usuario ingresó el comando RST.
		Para mostrar el menu nos apoyamos en una variable de tipo static unsigned char llamada Step (representa un paso), la cual indica la "frase" a mostrar
		en el menú. Una vez que se muestran las 3 frases en el menú, se resetea la variable print_Menu en 0 y el contador de pasos en 0.
		Esto para que la proxima vez que se necesite mostrar el menú, las variables se encuentren iniciadas en los valores correctos para el
		funcionamiento de Menu_Print(). */
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


		/*En esta funcion lo que se hace es que si no debemos mostrar el menú, vamos leyendo los caracteres ingresados por el usuario.
		Leemos un caracter, si no corresponde a la tecla "Enter" seguimos procesando los caracteres ingresados hasta llenar (o no) el vector que representa al comando.
		Si ingresa más de 5 caracteres ya pasa a ser un comando NO valido debido a que la cantidad de caracteres maximo de un comando valido es 5,
		esto es debido a que la frecuencia maxima valida es 10.000 que posee 5 caracteres, los demas comandos representan longitudes menores. 
		Una vez presionada la tecla Enter por parte del usuario, pasamos a procesar el comando. De esto se encarga la funcion Menu_Command() detallada a continuacion.
		*/
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

		/* Menu_Command() es la funcion encargada de validar el comando ingresado por el usuario. Ya de por si, si la variable
		too_long = 1 no procesaremos el comando debido a que es no valido, se encuentra fuera de rango.
		En el caso de que se encuentre dentro del rango, lo que hacemos es comparar el comando ingresado por el usuario con los comandos
		válidos. Los mismos se encuentran almacenados en la matriz cmds, por lo cual comparamos caracter por caracter si corresponde a uno de ellos.
		Una vez que se chequee la validez y se corresponda con algun comando valido, llamamos a la funcion del generador que corresponda.
		*/
void Menu_Command(){
	if(!too_long){
		//Si ingresó ON: encendemos el generador de señal
		if(command[0]==cmds[0][0]&&command[1]==cmds[0][1]&&command[2]==cmds[0][2]&&command[3]==cmds[0][3]&&command[4]==cmds[0][4]){
			GENERADOR_turnOn();
		} else //Si ingresó OFF: apagamos el generador de señal
			if(command[0]==cmds[1][0]&&command[1]==cmds[1][1]&&command[2]==cmds[1][2]&&command[3]==cmds[1][3]&&command[4]==cmds[1][4]){
				GENERADOR_turnOff();
			} else
				//Si ingresó RST: apagamos el generador de señal, seteamos la frecuencia por default e indicamos a la variable print_Menu que se debe 
				//volver a imprimir el menú
				if(command[0]==cmds[2][0]&&command[1]==cmds[2][1]&&command[2]==cmds[2][2]&&command[3]==cmds[2][3]&&command[4]==cmds[2][4]){ 
					GENERADOR_turnOff();
					GENERADOR_frec = 100;		
					print_Menu = 1;						
				} else{
					//Si ingresó un valor numerico para representar la frecuencia, chequeamos que este dentro de los valores validos y procedemos a 
					//asignar el valor de frecuencia a la variable GENERADOR_frec y actualizamos la frecuencia del Generador. 
					unsigned long frec = atoi(command);
					if((frec>=100)&&(frec<=10000)){
						GENERADOR_frec = frec;
						GENERADOR_refreshFrecuency();
					}else{
						//Caso negativo a todo lo evaluado anteriormente, le comunicamos al UART el mensaje "Comando no valido" para que lo imprima en consola
						SerialPort_Write_String_To_Buffer("Comando No Valido.\r");
					}
		}
	}	
	else{
		//Si el comando se encuentra fuera de rango, le transmitimos el mensaje "Comando no valido" al UART para que lo imprima en consola
		SerialPort_Write_String_To_Buffer("Comando No Valido.\r");
	}
	//Reinicializo para el siguiente comando
	too_long = 0;
	command[0] = 0;
	command[1] = 0;
	command[2] = 0;
	command[3] = 0;
	command[4] = 0;
	indice_command = 0;
}
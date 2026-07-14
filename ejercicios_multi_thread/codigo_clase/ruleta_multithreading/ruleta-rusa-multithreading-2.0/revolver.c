#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <pthread.h>

#include "global.h"
#include "define.h"
#include "funciones.h"
#include "archivos.h"
#include "semaforo.h"
#include "memoria.h"
#include "colamsg.h"
#include "thread.h"

/*
Proceso REVOLVER: (MSG_ REVOLVER)
Hilo Principal (main):
1- La cantidad de jugadores es siempre 6.
2- Borrar los mensajes anteriores para empezar de 0. 
3- Elije, en forma aleatoria, un número del 1 al 6 y lo guarda en memoria local.
4- Le envía un mensaje a cada JUGADOR, EVT_INICIO. (MSG_JUGADOR+i).
5- Espera recibir mensajes de los JUGADORES.
EVT_DISPARO, NUMERO_ELEGIDO.
•	Si NUMERO_ELEGIDO recibido es diferente al número del punto 3, contesta al JUGADOR con EVT_SALVADO. (msg.int_rte). Sigue con el punto 6.
•	Si NUMERO_ELEGIDO recibido es igual al número del punto 3, el “juego” finaliza; 
	se envía un mensaje EVT_FIN, NRO_JUGADOR (“eliminado”) a todos los JUGADORES (MSG_JUGADOR+i). Sigue con el punto 7.
6- Repite desde el punto 5.
7- Finaliza.

*/


int main(int argc, char *argV[])
{
	int i, pos_bala, nro_jugador, nro_elegido, meta;
	mem_inicial *inicial = NULL;
	mensaje msg;
	
	inicial = (mem_inicial*)creo_memoria(sizeof(mem_inicial), &id_memoria);
	inicial->main2 = 1; /*Indico que el main2 inicio*/

	/*Valido que el main1 haya iniciado
	if(inicial->main1 != 1) 
		printf("Esperando que inicie el proceso jugadores....\n");

	while(inicial->main1 != 1){ 
		usleep(200 * MS);
	}
	if(inicial->main1 == inicial->main2){
		liberarMemoria(&id_memoria, inicial);
	}
	*/
	srand(time(NULL));

	
	if(abrirArchivo(INICIALIZADOR, "w"))
		cerrarArchivo();
	

	id_colamsg = creo_cola_mensajes(); /*Creo la cola de mensajes*/
	borrar_mensajes(id_colamsg); /*Vacío la cola de mensajes*/

	system("clear");
	printf("\n================= PROCESO REVOLVER =================\n");


	meta = inicial->meta;
	printf("META -> %d\n", meta);
	
	pos_bala = devolverNumAleatorio(DESDE , HASTA);
	printf("\n---- POSICION DE LA BALA -> %d\n\n", pos_bala);

	printf("Enter para iniciar ruleta rusa \n");
	getchar();

	for(i=0;i<JUGADORES;i++)
	{
		
		enviar_mensaje(id_colamsg, MSG_JUGADOR + i, MSG_REVOLVER, EVT_INICIO, NULL);

		usleep(100 *MS);
		recibir_mensaje(id_colamsg, MSG_REVOLVER, &msg);

		if(msg.int_evento == EVT_DISPARO)
		{
			sscanf(msg.char_mensaje, "%d %d", &nro_jugador, &nro_elegido);
			printf("Disparo del jugador %d\n", nro_jugador);
			
		/*Si en la posicion del tambor recibida por el jugador está la bala, muere*/
			if(pos_bala == nro_elegido)
			{
				printf("------- BALA -----\n\n");
				enviar_mensaje(id_colamsg, msg.int_rte, MSG_REVOLVER, EVT_MUERTO, NULL);
				/*break;*/
			}else
			{
				printf("------- SALVADO -----\n\n");
				enviar_mensaje(id_colamsg, msg.int_rte, MSG_REVOLVER, EVT_SALVADO, NULL);
			}
		}
	}

	return 0;
}


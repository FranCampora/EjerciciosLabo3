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

int main(int argc, char *argV[])
{
	int i, metros_mosquito_tot, metros_humo_aux;
	int nro_mosquito, index_mosquito;
	int metros_humo = 0, fin = FALSE; 
	int mosquitos_fin[CANTIDAD_HILOS] = {0};

	char buffer[CORTO];

	mem_inicial *inicial = NULL;
	mensaje msg;
	
	inicial = (mem_inicial*)creo_memoria(sizeof(mem_inicial), &id_memoria);
	inicial->main2 = 1; /*Indico que el main2 inicio*/

	if(inicial->main1 != 1) /*Valido que el main1 haya iniciado*/
		printf("Esperando que inicie el proceso mosquitos....\n");

	while(inicial->main1 != 1){ 
		usleep(200 * MS);
	}
	if(inicial->main1 == inicial->main2){
		liberarMemoria(&id_memoria, inicial);
	}

	srand(time(NULL));
	system("clear");
	printf("\n______________ PROCESO PISTA ______________\n");
	
	printf("Presione <enter> para comenzar ");
	getchar();

	id_colamsg = creo_cola_mensajes(); /*Obtengo el id de la cola de mensajes*/
	
	/*Inicia los hilos*/
	for(i=0;i<CANTIDAD_HILOS;i++)
	{
		enviar_mensaje(id_colamsg, MSG_HILO + i, MSG_PISTA, EVT_AVANZAR, NULL);
	}

	while(!fin)
	{
		memset(&msg,0x00,sizeof(mensaje));
		memset(&buffer,0x00,sizeof(buffer));

		recibir_mensaje(id_colamsg, MSG_PISTA, &msg);
		usleep(500 * MS);

		nro_mosquito =  msg.int_rte -1;
		index_mosquito = msg.int_rte -2;
		
		switch (msg.int_evento)
		{
		case EVT_AVANZAR:
			metros_mosquito_tot = atoi(msg.char_mensaje); /*Recibe la cantidad de metros total*/
			
			/*Avanza el humo*/
			metros_humo_aux = devolverNumAleatorio(HUMO_DESDE, HUMO_HASTA);
			metros_humo += metros_humo_aux;
			printf("\n- El humo avanzó %d metros - total %d\n", metros_humo_aux, metros_humo);

			printf("El mosquito %d lleva %d metros en total\n",nro_mosquito, metros_mosquito_tot);

			if(metros_humo >= metros_mosquito_tot)
			{
				printf("\t EL HUMO ATRAPO AL MOSQUITO %d\n", nro_mosquito);
				sprintf(buffer, "%d", PERDEDOR);
				enviar_mensaje(id_colamsg, msg.int_rte, MSG_PISTA, EVT_FIN, buffer);
				mosquitos_fin[index_mosquito] = TRUE;
			}else if(metros_mosquito_tot >= META)
			{
				printf("\t EL mosquito %d llegó a la meta\n", nro_mosquito);
				sprintf(buffer, "%d", GANADOR);
				enviar_mensaje(id_colamsg, msg.int_rte, MSG_PISTA, EVT_FIN, buffer);
				mosquitos_fin[index_mosquito] = TRUE;
			}else
				enviar_mensaje(id_colamsg, msg.int_rte, MSG_PISTA, EVT_AVANZAR, NULL);
			break;
		
		default:
			break;
		}

		if(todos_terminaron(mosquitos_fin, CANTIDAD_HILOS))
			fin = TRUE;
	}
	return 0;
}


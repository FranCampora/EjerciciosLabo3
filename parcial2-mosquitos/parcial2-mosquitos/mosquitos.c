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
	int i;

	mem_inicial *inicial = NULL;
	st_data *datos_thread;
	
/*Creo bloque de memoria compartida para que main 2 verifique*/
	inicial = (mem_inicial*)creo_memoria(sizeof(mem_inicial), &id_memoria);
	inicial->main1 = 1; /*Indico que el main1 inició*/

	if(inicial->main2 != 1) /*Valido que el main2 haya iniciado*/
		printf("Esperando que inicie el proceso pista....\n");

	while(inicial->main2 != 1){ 
		usleep(200 * MS);
	}

	srand(time(NULL));
	system("clear");
	printf("\n______________ PROCESO MOSQUITOS ______________\n");
	

	/*Asigno memoria*/
	id_hilo = (pthread_t* ) malloc(sizeof(pthread_t)*CANTIDAD_HILOS);
	datos_thread = (st_data*) malloc(sizeof(st_data)*CANTIDAD_HILOS);

	pthread_mutex_init(&mutex, NULL); /*Inicializo el mutex*/
	pthread_attr_init(&atributos); /*Inicializo los atributos del thread*/
	pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_JOINABLE); /*Flag para el thread*/

	id_colamsg = creo_cola_mensajes(); /*Creo la cola de mensajes*/
	borrar_mensajes(id_colamsg); /*Vacío la cola de mensajes*/

/*Cargo el struct y lanzo los hilos*/
	for(i=0;i<CANTIDAD_HILOS;i++)
	{
		datos_thread[i].nro_hilo = i;
		datos_thread[i].id_colamsg = id_colamsg;
		
		if(pthread_create(&id_hilo[i], &atributos, funcionThread, &datos_thread[i]) != 0)
		{
			printf("No puedo crear thread '%d'\n",i);
			exit(1);
		}
	}

/*Espero a que terminen todos los hilos*/
	for(i=0;i<CANTIDAD_HILOS;i++)
	{
		pthread_join(id_hilo[i], NULL);
	}

/*Libero los recursos*/
	free(id_hilo);
	free(datos_thread);
	borrar_cola_mensajes(id_colamsg);
	
	return 0;
}


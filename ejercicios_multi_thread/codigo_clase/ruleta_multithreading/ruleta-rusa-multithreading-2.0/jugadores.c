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
El hilo principal (main): 
•	La cantidad de jugadores es siempre 6.
•	Genera un vector de 6 posiciones inicializado en 0, llamado “vector_tambor”, [0][0][0][0][0][0]. Este vector simula la posición de cada bala.
•	Lanza tantos HiloJUGADOR como JUGADORES haya, y les pasa un puntero con la dirección del “vector_tambor”.
•	Se queda esperando que terminen los hilos.
•	Muestra el número del JUGADOR “eliminado” por pantalla (recibido de los hilos).
*/


int main(int argc, char *argV[])
{
	int i;
	int eliminado = 0;
	int vector_tambor[TAMBOR] = {0};
	
	mem_inicial *inicial = NULL;

	tjugador *datos_thread;
	
	/*Creo bloque de memoria compartida para que main 2 verifique*/
	inicial = (mem_inicial*)creo_memoria(sizeof(mem_inicial), &id_memoria);
	inicial->main1 = 1; /*Indico que el main1 inició*/

	/*Valido que el main2 haya iniciado
	if(inicial->main2 != 1) 
		printf("Esperando que inicie el proceso revolver....\n");

	while(inicial->main2 != 1){ 
		usleep(200 * MS);
	}
	*/

	printf("Esperando que inicie el proceso revolver....\n");
	while(!abrirArchivo(INICIALIZADOR, "r"))
	{
		usleep(200 * MS);
	}
	cerrarArchivo();
	borrarArchivo(INICIALIZADOR);

	/*Asigno memoria*/
	idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*JUGADORES);
	datos_thread = (tjugador*) malloc(sizeof(tjugador)*JUGADORES);

	pthread_mutex_init(&mutex, NULL); /*Inicializo el mutex*/
	pthread_attr_init(&atributos); /*Inicializo los atributos del thread*/
	pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_JOINABLE); /*Flag para el thread*/
	
	id_colamsg = creo_cola_mensajes(); /*Obtengo el id de la cola de mensajes*/
	
	system("clear");
	printf("\n================= PROCESO JUGADORES =================\n");
	printf("Numero de jugadores: %d\n\n",JUGADORES);

	esperarSemaforo(id_semaforo);
	inicial->meta = META;
	levantarSemaforo(id_semaforo);


/*Cargo el struct y lanzo los hilos*/
	for(i=0;i<JUGADORES;i++)
	{
		datos_thread[i].id_jugador = i;
		datos_thread[i].id_colamsg = id_colamsg;
		datos_thread[i].eliminado = &eliminado;
		datos_thread[i].vector_tambor = vector_tambor; /*La struct recibe la direccion del primer elemento del vector*/
		
		if(pthread_create(&idHilo[i], &atributos, funcionThread, &datos_thread[i]) != 0)
		{
			printf("No puedo crear thread '%d'\n",i);
			exit(1);
		}
	}

/*Espero a que terminen todos los hilos jugadores*/
	for(i=0;i<JUGADORES;i++)
	{
		pthread_join(idHilo[i], NULL);
	}
	
	printf("\nJugador eliminado: %d\n\n", eliminado);

	free(idHilo);
	free(datos_thread);
	borrar_cola_mensajes(id_colamsg);

	return 0;
}


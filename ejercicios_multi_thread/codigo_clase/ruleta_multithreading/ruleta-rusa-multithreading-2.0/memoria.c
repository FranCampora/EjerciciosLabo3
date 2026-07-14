#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#include "memoria.h"
#include "clave.h"
#include "global.h"
#include "define.h"

mem_data* creo_memoria(int size, int *r_id_memoria)
{
	mem_data* p_memoria;
	key_t clave = creo_clave(); /*Le sumo para que sea distinta la semaforo*/
	int id_memoria;

	id_memoria = shmget(clave, size, IPC_CREAT | 0666); 

	if(id_memoria == -1)
	{
		printf("No consigo id para memoria compartida\n");
		exit (1);
	}

	p_memoria = (mem_data *)shmat(id_memoria, NULL, 0);

	if(p_memoria == NULL)
	{
		printf("No consigo memoria compartida\n");
		exit (1);
	}

	*r_id_memoria = id_memoria; /*Carga la direccion del id*/
	return p_memoria; /*Devuelve un puntero a la memoria*/
}


void liberarMemoria(int *id_memoria, void *memoria)
{
	shmctl (*id_memoria, IPC_RMID, (struct shmid_ds *)NULL);
	shmdt (memoria);
}



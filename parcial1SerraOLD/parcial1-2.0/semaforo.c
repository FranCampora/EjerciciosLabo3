#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

#include "semaforo.h"
#include "clave.h"
#include "define.h"

int crearSemaforo(){

	key_t clave =  creo_clave();
	int id_sem = semget(clave,1, 0600|IPC_CREAT);

	if(id_sem == -1){
		printf("Error: no puedo crear el semaforo\n");
		exit(0);
	}
	return id_sem;
}

void iniciarSemaforo(int id_sem){
	semctl(id_sem,0,SETVAL,1);
}


void levantarSemaforo(int id_sem){
	struct sembuf operacion;

	operacion.sem_num = 0;
	operacion.sem_op = 1; /*Incrementa el sem en 1*/
	operacion.sem_flg = 0;

	semop(id_sem, &operacion, 1);
}

void esperarSemaforo(int id_sem){
	struct sembuf operacion;

	operacion.sem_num = 0;
	operacion.sem_op = -1; /*Decrementa el sem en -1*/
	operacion.sem_flg = 0;

	semop(id_sem, &operacion, 1);
}




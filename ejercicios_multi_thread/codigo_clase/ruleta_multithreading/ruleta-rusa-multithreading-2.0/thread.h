#ifndef _THREAD_H
#define _THREAD_H

void *funcionThread(void *parametro);


typedef struct tipo_jugador{
	int id_jugador;
	int id_colamsg;
	int* vector_tambor;
	int* eliminado;
}tjugador;


#endif


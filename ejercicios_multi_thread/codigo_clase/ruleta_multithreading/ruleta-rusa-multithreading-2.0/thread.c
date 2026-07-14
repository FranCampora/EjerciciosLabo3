#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "colamsg.h"
#include "thread.h"
#include "global.h"
#include "define.h"
#include "funciones.h"

/*
HiloJUGADOR (uno por JUGADOR): (MSG_JUGADOR+nro_JUGADOR)
Para evitar que todas disparen juntas se deberá usar un MUTEX.
1- Espera recibir el mensaje EVT_INICIO.
2- Escribir por pantalla, “Soy el jugador N y voy a dispararme“
3- Envía al REVOLVER el EVT_DISPARO, NUMERO_ELEGIDO; este número lo saca del “vector_tambor”, 
	de uno por vez (es decir, donde no hay un 0), después pone esa posición en 1. vector_tambor=>[1][0][0][0][0][0]. 
4- Escribir por pantalla, “Soy el jugador N y la posición del tambor es M“
5- Espera recibir mensajes del REVOLVER.
6- SI recibe un mensaje EVT_SALVADO.
Si recibe un mensaje EVT_FIN, devuelve el número del JUGADOR “eliminado” al main.
7- Devuelve si se “salvó” o fue “eliminado” y termina.
*/

void *funcionThread(void *parametro)
{
	int i, id_jugador, nro_jugador, nro_elegido;
	char buffer[CORTO];
	mensaje msg;

/*Obtengo el puntero al struct pasado por parametro*/
	tjugador *data = (tjugador*) parametro;

	id_jugador = data->id_jugador;  /*0, 1, 2, 3, 4, 5*/
	id_colamsg = data->id_colamsg;
	nro_jugador = id_jugador +1; 	/*1, 2, 3, 4, 5, 6*/

	memset(buffer,0x00,sizeof(buffer));

	recibir_mensaje(id_colamsg, MSG_JUGADOR + id_jugador, &msg);
	pthread_mutex_lock(&mutex);

	if(msg.int_evento == EVT_INICIO)
	{

		printf("Soy el jugador %d y voy a dispararme\n",nro_jugador);

		for(i=0;i<TAMBOR;i++)
		{
		/*Busca la primera posicion del vector_tambor con '0'*/
			if(data->vector_tambor[i] == 0)
			{
				nro_elegido = i;
				data->vector_tambor[i] = 1;
				break; /*Sale del for*/
			}
		}
		printf("Posicion del tambor %d\n", nro_elegido);
		sprintf(buffer,"%d %d",nro_jugador, nro_elegido);

		/*Envia mensaje al proceso revolver con la posicion del tambor elegida*/
		enviar_mensaje(id_colamsg, MSG_REVOLVER, MSG_JUGADOR + id_jugador, EVT_DISPARO, buffer);
		usleep(100*MS);
		recibir_mensaje(id_colamsg, MSG_JUGADOR + id_jugador, &msg);

		if(msg.int_evento == EVT_SALVADO)
			printf("Soy jugador %d y me SALVE! :)\n\n",nro_jugador);
		else if(msg.int_evento == EVT_MUERTO)
		{
			printf("Soy jugador %d y MORI :(\n\n",nro_jugador);
			*data->eliminado = nro_jugador;
		}
	}

	pthread_mutex_unlock(&mutex);
	usleep(100 * MS);

	pthread_exit(NULL);
}





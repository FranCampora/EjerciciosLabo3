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

void *funcionThread(void *arg)
{
	int nro_hilo, metros_actual, resultado;
	int metros_total = 0, fin = FALSE;
	
	char buffer[CORTO];
	mensaje msg;

/*Obtengo el puntero al struct pasado por arg*/
	st_data *data = (st_data*) arg;

	nro_hilo = data->nro_hilo;
	id_colamsg = data->id_colamsg;
	
	while(!fin)
	{
		memset(&msg,0x00,sizeof(mensaje));
		memset(&buffer,0x00,sizeof(buffer));

		recibir_mensaje(id_colamsg, MSG_HILO + nro_hilo, &msg);
		pthread_mutex_lock(&mutex);

		switch (msg.int_evento)
		{
		case EVT_AVANZAR:

			metros_actual = devolverNumAleatorio(MOSQUITO_DESDE,MOSQUITO_HASTA);
			metros_total += metros_actual;

			printf("Soy mosquito %d avanzo %d metros\n", nro_hilo + 1, metros_actual);

			sprintf(buffer,"%d",metros_total);
			enviar_mensaje(id_colamsg, MSG_PISTA, MSG_HILO + nro_hilo, EVT_AVANZAR, buffer);

		break;
		case EVT_FIN:
			resultado = atoi(msg.char_mensaje);

			if(resultado == GANADOR)
				printf("Soy mosquito %d -- Llegué a la meta :)\n", nro_hilo+1);
			else if(resultado == PERDEDOR)
				printf("Soy mosquito %d -- me atrapó el humo :(\n",nro_hilo+1);
			fin = TRUE;

		break;
		default:
			printf("Evento desconocido\n");
			break;
		}

		pthread_mutex_unlock(&mutex);
		usleep(500 * MS);
	}
	pthread_exit(NULL);
}



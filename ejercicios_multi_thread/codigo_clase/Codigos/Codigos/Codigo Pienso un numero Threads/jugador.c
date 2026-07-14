#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "clave.h"
#include "mensajes.h"
#include "unistd.h"
#include "funciones.h"
#include "def.h"
#include <pthread.h>

#define DESDE 1
#define HASTA 99


//declaro la estructura pra ser usada por los threads
typedef struct tipo_jugador tjugador;
struct tipo_jugador
{
	int 	nro_jugador;						
	int  id_colamensaje;
	int   cant_jugadores;
};

void *ThreadJugador (void *parametro)
{
	int 			nro_jugador;
	int 			id_cola_mensajes;
	int 			done=0;
	int 			cant_jugadores;
	int 			numero;
	int 			cant_intentos;
	int 			i;
	char 	cadena[50];
	mensaje		msg;
	
	tjugador *datos_thread = (tjugador*) parametro;
	nro_jugador = datos_thread->nro_jugador;
	id_cola_mensajes = datos_thread->id_colamensaje;
	cant_jugadores = datos_thread -> cant_jugadores;
	
	srand(time(NULL));							      
	while(done==0)
	{
		
		numero = (rand()%(HASTA-DESDE+1))+DESDE;
		pthread_mutex_lock (&mutex);
		printf("\nJugador:%d Envia el numero:%d  A ver si le pegue!\n", nro_jugador, numero);
		sprintf(cadena,"%d",numero);
		enviar_mensaje(id_cola_mensajes , MSG_PIENSO,MSG_JUGADOR+nro_jugador, EVT_NUMERO, cadena);
		cant_intentos++;
		pthread_mutex_unlock (&mutex);	
		usleep(100*1000);
		recibir_mensaje(id_cola_mensajes, MSG_JUGADOR, &msg);	//bloqueate
		
		pthread_mutex_lock (&mutex);
		switch (msg.int_evento)
		{
			case EVT_ACERTO:
				printf("\nAcerte: Con %d Cantidad de intentos \n", cant_intentos);
				for(i=0;i<cant_jugadores;i++)
				{
					enviar_mensaje(id_cola_mensajes , MSG_JUGADOR, MSG_JUGADOR, EVT_FINALIZAR, "FINALIZAR");
				}
				
				break;		    	
			case EVT_NO_ACERTO:
					printf("Jugador: %d Pucha, sigo intentando! \n", nro_jugador);
				break;
			case EVT_FINALIZAR:
				printf("\nJugador:%d Finalizo\n", nro_jugador);
				done=1;
				break;
			default:
				printf("\nJugador:%d Evento sin definir\n", nro_jugador);
			break;
		}
		pthread_mutex_unlock (&mutex);	
		usleep(100*1000);
		//sleep (1);
	};
	pthread_exit ((void *)"Listo");
}


int main(int argc, char *argv[])
{
	int id_cola_mensajes;
	int i, ctl = 0;
	int cant_jugadores;
	
	tjugador *datos_thread;

	printf("Ingrese cantidad de jugadores \n");
	scanf("%d",&cant_jugadores);
	

	printf("cantidad de jugadores %d \n\n", cant_jugadores);
	id_cola_mensajes 	= creo_id_cola_mensajes(CLAVE_BASE);
	pthread_mutex_init (&mutex, NULL);

	pthread_t* idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cant_jugadores);
	pthread_attr_t 	atributos;
	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
	
	datos_thread = (tjugador*) malloc(sizeof(tjugador)*cant_jugadores);
	
	for(i=0; i<cant_jugadores; i++)
	{
		datos_thread[i].nro_jugador = i;
		datos_thread[i].id_colamensaje = id_cola_mensajes;
		datos_thread[i].cant_jugadores=cant_jugadores;
		
		pthread_create (&idHilo[i], &atributos, ThreadJugador, &datos_thread[i]);
	}

	for(i=0;i<cant_jugadores;i++)
	{
		pthread_join (idHilo[i], NULL);
		printf("Finalizo el jugador %d \n",datos_thread[i].nro_jugador);
	}
	
	free(idHilo); 
	
	return 0;
}
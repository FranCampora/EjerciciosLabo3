#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "clave.h"
#include "mensajes.h"
#include "unistd.h"
#include "funciones.h"
#include "def.h"
#include <pthread.h>

/*debe estar en definiciones*/
#define DESDE 1
#define HASTA 99


int main(int argc, char *argv[])
{
	int pienso_un_numero;
	int id_cola_mensajes;
	int numero_recibido;
	int nro_jugador;
	int cant_jugadores;
	int done =0;
	mensaje		msg;

	srand(time(NULL));
	//pienso el numero y lo guardo en la variable local
	pienso_un_numero = (rand()%(DESDE-HASTA+1))+DESDE;

	printf("Pense este numero %d\n", pienso_un_numero);

	id_cola_mensajes 	= creo_id_cola_mensajes(CLAVE_BASE);

	borrar_mensajes(id_cola_mensajes); //Borra todos los mensajes que haya en la cola.

	while(done==0)
	{

		recibir_mensaje(id_cola_mensajes, MSG_PIENSO, &msg);//bloqueate
		//Obtengo el id del jugador que me esta enviando el mensaje.
		nro_jugador = msg.int_rte-MSG_JUGADOR;
		
		switch (msg.int_evento)
		{
			case EVT_NUMERO:
				
				numero_recibido = atoi(msg.char_mensaje);
				
				if(numero_recibido==pienso_un_numero)
				{
					enviar_mensaje(id_cola_mensajes , MSG_JUGADOR+nro_jugador, MSG_PIENSO, EVT_ACERTO, "FINALIZAR");
					printf("Jugador:%d ", nro_jugador);
					printf("Has ganado! con el número %d ", numero_recibido);
					done = 1;
				}
				else
				{
					enviar_mensaje(id_cola_mensajes , MSG_JUGADOR, MSG_PIENSO, EVT_NO_ACERTO, "FINALIZAR");
				}
					
				break;		    	
			default:
				printf("\nPienso:%d Evento sin definir\n", nro_jugador);
			break;
		}
	};

	return 0;
}
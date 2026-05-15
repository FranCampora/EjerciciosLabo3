#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <clave.h>
#include <def.h>
#include <mensajes.h>


#define SOY_PROCESO 2
#define DESTINATARIO 1


int main(int argc,char *argv[])
{
	int id_cola_mensajes;
	
	id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);

	//borrar los mensajes pendientes antes de comenzar
	borrar_mensajes(id_cola_mensajes);
	
	
	printf("\n---> soy proceso %d, espero para recibir un mensaje <---\n",SOY_PROCESO);

	recibir_mensaje(id_cola_mensajes);

	printf("\n---> soy proceso %d, envio un mensaje al proceso %d <---\n",SOY_PROCESO,DESTINATARIO);
	sleep(1);
	enviar_mensaje(id_cola_mensajes);
	
	borrar_cola_de_mensajes(id_cola_mensajes);

	return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <def.h>
#include <mensajes.h>
#include <clave.h>

/*en define deberia estar*/
#define LARGO_TMENSAJE 1024
#define DESDE 10
#define HASTA 20
#define INTERVALO 1

#define MSG_JOSE 		1		
#define MSG_PEPITO		2		

#define EVT_MENSAJE		1		
#define EVT_FIN			2


int main(int argc,char *argv[])
{
	int 		id_cola_mensajes;
	mensaje		msg;	
	msg.int_evento = 0;
	id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);
	
	borrar_mensajes(id_cola_mensajes);

	printf("\nSOY DON JOSE\n");
	
	enviar_mensaje(id_cola_mensajes , MSG_PEPITO, MSG_JOSE, EVT_MENSAJE, "HOLA DON PEPITO");

	while( msg.int_evento!=EVT_FIN)
	{
		recibir_mensaje(id_cola_mensajes, MSG_JOSE, &msg);
		printf("Destino   %d\n", (int) msg.long_dest);
		printf("Remitente %d\n", msg.int_rte);
		printf("Evento    %d\n", msg.int_evento);
		printf("Mensaje   %s\n", msg.char_mensaje);
		switch (msg.int_evento)
		{
			case EVT_MENSAJE:
				printf("Recibi el EVT_MENSAJE\n");
				//printf("Mensaje   %s\n", msg.char_mensaje);
				sleep(INTERVALO);
				if(strcmp(msg.char_mensaje, "HOLA DON JOSE")==0)
					enviar_mensaje(id_cola_mensajes , msg.int_rte, MSG_JOSE, EVT_MENSAJE, "PASO USTED POR CASA");
				else if(strcmp(msg.char_mensaje, "POR SU CASA YO PASE")==0)
					enviar_mensaje(id_cola_mensajes , msg.int_rte, MSG_JOSE, EVT_MENSAJE, "VIO USTED A MI ABUELA");
				else if(strcmp(msg.char_mensaje, "A SU ABUELA YO LA VI")==0)
					enviar_mensaje(id_cola_mensajes , msg.int_rte, MSG_JOSE, EVT_FIN, "ADIOS DON PEPITO");
			break;
			case EVT_FIN:
				printf("Recibi el EVT_FIN\n");
			break;
			default:
				printf("\nEvento sin definir\n");
			break;
		}
		printf("------------------------------\n");
	}

	if(msg.int_evento!=EVT_FIN)	
		enviar_mensaje(id_cola_mensajes, MSG_PEPITO, MSG_JOSE, EVT_FIN, "");		

	return 0;
}



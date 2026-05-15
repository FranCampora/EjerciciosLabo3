#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "funciones.h"
#include "definiciones.h"
#include "mensajes.h"
#include "def.h"
#include "clave.h"
#include "unistd.h"

#define INTERVALO_PEDIDOS 2000

void procesar_evento(int id_cola_mensajes, mensaje msg)
{

	printf("Destino   %d\n", (int) msg.long_dest);
	printf("Remitente %d\n", msg.int_rte);
	printf("Evento    %d\n", msg.int_evento);
	printf("Mensaje   %s\n", msg.char_mensaje);
	switch (msg.int_evento)
	{
		case EVT_RESPUESTA_STOCK:
			printf("Rta stock\n");
			printf("STOCK %d\n", atoi(msg.char_mensaje));
		break;

		default:
			printf("\nEvento sin definir\n");
		break;
	}
	printf("------------------------------\n");
}

int main(int argc,char *argv[])
{
	int 			id_cola_mensajes;
	mensaje			msg;

	id_cola_mensajes 	= creo_id_cola_mensajes(CLAVE_BASE);
	
	while(1)
	{
		enviar_mensaje(id_cola_mensajes , MSG_SUPERMERCADO, MSG_REPOSITOR, EVT_SUMA_STOCK, "SUMA UNO");
		enviar_mensaje(id_cola_mensajes , MSG_SUPERMERCADO, MSG_REPOSITOR, EVT_CONSULTA_STOCK, "DECIME EL STOCK POR FAVOR");
		recibir_mensaje(id_cola_mensajes, MSG_REPOSITOR, &msg);
		procesar_evento(id_cola_mensajes, msg);
		usleep (INTERVALO_PEDIDOS*1000);
	};
	
	return 0;
}

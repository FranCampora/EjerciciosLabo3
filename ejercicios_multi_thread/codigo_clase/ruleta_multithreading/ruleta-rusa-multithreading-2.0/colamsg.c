#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>

#include "colamsg.h"
#include "clave.h"
#include "global.h"
#include "define.h"


int creo_cola_mensajes()
{
	int id_cola = msgget(creo_clave(), 0600 | IPC_CREAT);
	if (id_cola == -1)
	{
		printf("Error al obtener identificador para cola mensajes\n");
		exit (1);
	}
	return id_cola;
}


int enviar_mensaje(int id_cola, long rLongDest, int rIntRte, int rIntEvento, char* rpCharMsg)
{
	mensaje msg;
	int res;

	msg.long_dest = rLongDest;
	msg.int_rte = rIntRte;
	msg.int_evento = rIntEvento;
	if(rpCharMsg != NULL)
		strcpy(msg.char_mensaje, rpCharMsg);

	/*printf("\nEnvío Mensaje '%s'\n", msg.char_mensaje);*/
	res = msgsnd(id_cola, (struct msgbuf *)&msg, sizeof(msg.int_rte) + sizeof(msg.int_evento) + sizeof(msg.char_mensaje), IPC_NOWAIT);

	return res;
}		


int recibir_mensaje(int id_cola, long rLongDest, mensaje* rMsg)
{
	mensaje msg;
	int res;

	res = msgrcv(id_cola, (struct msgbuf *)&msg, sizeof(msg.int_rte)+ sizeof(msg.int_evento)+ sizeof(msg.char_mensaje), rLongDest, 0);
	
	rMsg->long_dest = msg.long_dest;
	rMsg->int_rte = msg.int_rte;
	rMsg->int_evento = msg.int_evento;
	strcpy(rMsg->char_mensaje, msg.char_mensaje);
	/*printf(" --- Recibo mensaje -- evento %d\n",msg.int_evento);*/

	return res;
}

int borrar_mensajes(int id_cola)
{
	mensaje msg;
	int res;
	/*"Recibe" todos los mensajes de la cola para limpiarla*/
	do
	{
		res = msgrcv(id_cola, (struct msgbuf *)&msg,  sizeof(msg.int_rte)+sizeof(msg.int_evento)+sizeof(msg.char_mensaje), 0, IPC_NOWAIT);
	}while(res>0);
	
	return res;
}


void borrar_cola_mensajes(int id_cola)
{
	msgctl (id_cola, IPC_RMID, (struct msqid_ds *)NULL);
}



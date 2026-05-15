
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "string.h"
#include "clave.h"

int creo_id_cola_mensajes(int clave)
{
	int id_cola_mensajes = msgget (creo_clave(clave), 0600 | IPC_CREAT);
	if (id_cola_mensajes == -1)
	{
		printf("Error al obtener identificador para cola mensajes\n");
		exit (-1);
	}
	return id_cola_mensajes;
}

int enviar_mensaje(int id_Cola_Mensajes)
{
	//
	//	Se rellenan los campos del mensaje que se quiere enviar.
	//	El Id_Mensaje es un identificador del tipo de mensaje. Luego se podr�
	//	recoger aquellos mensajes de tipo 1.
	//	Dato_Numerico es un dato que se quiera pasar al otro proceso. Se pone, 
	//	por ejemplo 29.
	//	Mensaje es un texto que se quiera pasar al otro proceso.
	//
	mensaje Un_Mensaje;
	Un_Mensaje.Id_Mensaje = DESTINATARIO;
	Un_Mensaje.Dato_Numerico = 29;
	sprintf (Un_Mensaje.Mensaje, "-------- Hola proceso %d ---------",DESTINATARIO);

	//
	//	Se envia el mensaje. Los par�metros son:
	//	- Id de la cola de mensajes.
	//	- Direcci�n al mensaje, convirti�ndola en puntero a (struct msgbuf *)
	//	- Tama�o total de los campos de datos de nuestro mensaje, es decir
	//	de Dato_Numerico y de Mensaje
	//	- Unos flags. IPC_NOWAIT indica que si el mensaje no se puede enviar
	//	(habitualmente porque la cola de mensajes esta llena), que no espere
	//	y de un error. Si no se pone este flag, el programa queda bloqueado
	//	hasta que se pueda enviar el mensaje.
	//

		
	return msgsnd (id_Cola_Mensajes, (struct msgbuf *)&Un_Mensaje, sizeof(Un_Mensaje.Dato_Numerico)+sizeof(Un_Mensaje.Mensaje), IPC_NOWAIT);
}		

int recibir_mensaje(int id_Cola_Mensajes)
{

	//
	//	Se recibe un mensaje del otro proceso. Los par�metros son:
	//	- Id de la cola de mensajes.
	//	- Direcci�n del sitio en el que queremos recibir el mensaje,
	//	convirti�ndolo en puntero a (struct msgbuf *).
	//	- Tama�o m�ximo de nuestros campos de datos. 
	//	- Identificador del tipo de mensaje que queremos recibir. En este caso
	//	se quiere un mensaje de tipo 1. Si ponemos tipo 1, se extrae el mensaje
	//	que se acaba de enviar en la llamada anterior a msgsnd().
	//	- flags. En este caso se quiere que el programa quede bloqueado hasta
	//	que llegue un mensaje de tipo 1. Si se pone IPC_NOWAIT, se devolver�a
	//	un error en caso de que no haya mensaje de tipo 1 y el programa
	//	continuar�a ejecut�ndose.
	//
	mensaje Un_Mensaje;
	int res;
		
	res = msgrcv (id_Cola_Mensajes, (struct msgbuf *)&Un_Mensaje, sizeof(Un_Mensaje.Dato_Numerico) + sizeof(Un_Mensaje.Mensaje), 
			SOY_PROCESO, 0);

	printf("Recibido mensaje tipo: %d\n",SOY_PROCESO);
	printf("Dato_Numerico = %d\n", Un_Mensaje.Dato_Numerico);
	printf("Mensaje = %s\n", Un_Mensaje.Mensaje);

	

	return res;
}

int borrar_cola_de_mensajes(int Id_Cola_Mensajes){
	//
	//	Se borra y cierra la cola de mensajes.
	// IPC_RMID indica que se quiere borrar. El puntero del final son datos
	// que se quieran pasar para otros comandos. IPC_RMID no necesita datos,
	// as� que se pasa un puntero a NULL.
	//
	msgctl (Id_Cola_Mensajes, IPC_RMID, (struct msqid_ds *)NULL);

}

int borrar_mensajes(int id_cola_mensajes)
{
	mensaje Un_Mensaje;
	int res;
	do
	{
		res = msgrcv (id_cola_mensajes, (struct msgbuf *)&Un_Mensaje, sizeof(Un_Mensaje.Dato_Numerico) + sizeof(Un_Mensaje.Mensaje), 
			0, IPC_NOWAIT);

	}while(res>0);
	return res;
}
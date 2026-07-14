#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#include "string.h"
#include "clave.h"
#include "mensajes.h"


int creo_id_cola_mensajes()
{
    int id_cola_mensajes;
    id_cola_mensajes = msgget (creo_clave(), 0600 | IPC_CREAT);
    
    if (id_cola_mensajes == -1)
    {
        printf("Error al obtener identificador para cola mensajes\n");
        exit (-1);
    }
    return id_cola_mensajes;
}

/*
int creo_id_cola_mensajes(int clave)
{
    int id_cola_mensajes;
    id_cola_mensajes = msgget ((key_t)clave, 0600 | IPC_CREAT);
    
    if (id_cola_mensajes == -1)
    {
        printf("Error al obtener identificador para cola mensajes\n");
        exit (-1);
    }
    return id_cola_mensajes;
}
    */
int enviar_mensaje(int id_cola_mensajes, long destinatario, int remitente, int evento, char* textoMensaje)
{
    /*
    Se rellenan los campos del mensaje que se quiere enviar.
    El Id_Mensaje es un identificador del tipo de mensaje. Luego se podr�
    recoger aquellos mensajes de tipo 1.
    Dato_Numerico es un dato que se quiera pasar al otro proceso. Se pone, 
    por ejemplo 29.
    Mensaje es un texto que se quiera pasar al otro proceso.
    */
    mensaje un_mensaje;
    int res;

    memset(&un_mensaje,0x00,sizeof(un_mensaje));
    un_mensaje.long_dest = destinatario;
    un_mensaje.int_rte = remitente;
    un_mensaje.int_evento = evento;
    strncpy(un_mensaje.char_mensaje,textoMensaje,sizeof(un_mensaje.char_mensaje)-1);

    

    /*sprintf (un_mensaje.char_mensaje, "-------- Hola proceso %ld desde proceso %d ---------",destinatario,remitente);

    
    Se envia el mensaje. Los par�metros son:
    - Id de la cola de mensajes.
    - Direcci�n al mensaje, convirti�ndola en puntero a (struct msgbuf *)
    - Tama�o total de los campos de datos de nuestro mensaje, es decir
    de Dato_Numerico y de Mensaje
    - Unos flags. IPC_NOWAIT indica que si el mensaje no se puede enviar
    (habitualmente porque la cola de mensajes esta llena), que no espere
    de un error. Si no se pone este flag, el programa queda bloqueado
    hasta que se pueda enviar el mensaje.
    */
    res = msgsnd(id_cola_mensajes,
                (void *)&un_mensaje,
                sizeof(un_mensaje.int_rte) + sizeof(un_mensaje.int_evento) + sizeof(un_mensaje.char_mensaje),
                IPC_NOWAIT);
    if (res == -1)
    {
        printf("ERROR AL ENVIAR MENSAJE");
    }
    return res;
    /*ENVIAR MENSAJE TIENE QUE TENER COMO PARAMETRO: primero el id, identificador del destinatario, id del remitente, id del evento y cadena del texto*/
}

int recibir_mensaje(int id_cola_mensajes, long destinatario, mensaje* respuesta_texto_mensaje)
{
/*recibe el id de la cola
el numero del destinatario a donde va a recibirse el mensaje
un puntero a la estructura donde SE VA A GUARDAR EL MENSAJE*/
	/*
		Se recibe un mensaje del otro proceso. Los par�metros son:
		- Id de la cola de mensajes.
		- Direcci�n del sitio en el que queremos recibir el mensaje,
		convirti�ndolo en puntero a (struct msgbuf *).
		- Tama�o m�ximo de nuestros campos de datos. 
		- Identificador del tipo de mensaje que queremos recibir. En este caso
		se quiere un mensaje de tipo 1. Si ponemos tipo 1, se extrae el mensaje
		que se acaba de enviar en la llamada anterior a msgsnd().
		- flags. En este caso se quiere que el programa quede bloqueado hasta
		que llegue un mensaje de tipo 1. Si se pone IPC_NOWAIT, se devolver�a
		un error en caso de que no haya mensaje de tipo 1 y el programa
		continuar�a ejecut�ndose.
	*/
	mensaje un_mensaje;
	int res;
	
    memset(&un_mensaje,0x00,sizeof(un_mensaje));
	res = msgrcv(id_cola_mensajes,
                (void *)&un_mensaje,sizeof(un_mensaje.int_rte) + sizeof(un_mensaje.int_evento) + sizeof(un_mensaje.char_mensaje),
                destinatario,0); /* 0 = bloqueante, espera hasta recibir */
            /*filtra los mensajes que tengan el long_desti = DESTINATARIO DE PARAMETRO*/
	
    respuesta_texto_mensaje->long_dest = un_mensaje.long_dest;
    respuesta_texto_mensaje->int_rte = un_mensaje.int_rte;
    respuesta_texto_mensaje->int_evento = un_mensaje.int_evento;
    
    strcpy(respuesta_texto_mensaje->char_mensaje,un_mensaje.char_mensaje);
	

	return res;
}

int borrar_cola_de_mensajes(int id_cola_mensajes){
	/*
		Se borra y cierra la cola de mensajes.
	IPC_RMID indica que se quiere borrar. El puntero del final son datos
	que se quieran pasar para otros comandos. IPC_RMID no necesita datos,
	as� que se pasa un puntero a NULL.
    */
	int res;
    res = msgctl (id_cola_mensajes, IPC_RMID, (struct msqid_ds *)NULL);
    
    return res;

}

int borrar_mensajes(int id_cola_mensajes)
{
	mensaje un_mensaje;
	int res;
    memset(&un_mensaje,0x00,sizeof(un_mensaje));
	do
	{
        res = msgrcv(id_cola_mensajes,
                    (void *)&un_mensaje,
                    sizeof(un_mensaje.int_rte) + sizeof(un_mensaje.int_evento) + sizeof(un_mensaje.char_mensaje),
                    0,IPC_NOWAIT);

    } while (res > 0);

	return res;
}

cliente* buscar_cliente(char *szCodigo, cliente *clientes, int cantClientes)
{
    /*recibe el codigo del cliente que tiene que buscar, el array donde tiene que buscar, el maximo osea la cantidad del array asi tiene corte*/
    int codBuscado;
    int i;

    codBuscado = atoi(szCodigo); /* convierte el char_mensaje a int */
    i = 0;

    while (i < cantClientes && clientes[i].codigo_cliente != codBuscado)
    {
        i++;
    }

    if (i < cantClientes)
    {
        return &clientes[i]; /* encontrado: devuelve puntero al elemento del array */
    }else
    {
        return NULL; /* no encontrado */
    }
    

    
}

#ifndef _COLAMSG_H
#define _COLAMSG_H

#include "define.h"

typedef struct tipo_mensaje{
	long long_dest; /*Destinatario*/
	int int_rte; /*Remitente*/
	int int_evento;	/*Numero de nevento*/
	char char_mensaje[LINEA]; /*Mensaje*/
}mensaje;


int creo_cola_mensajes();
int enviar_mensaje(int id_colamsg, long rLongDest, int rIntRte, int rIntEvento, char* rpCharMsg);
int recibir_mensaje(int id_colamsg, long rLongDest, mensaje* rMsg);
int borrar_mensajes(int id_colamsg);
void borrar_cola_mensajes(int id_colamsg);


#endif



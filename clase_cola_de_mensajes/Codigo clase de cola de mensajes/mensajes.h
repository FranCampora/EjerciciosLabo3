#ifndef _MENSAJE
#define _MENSAJE

int creo_id_cola_mensajes(int clave);
int enviar_mensaje(int id_Cola_Mensajes);
int recibir_mensaje(int id_Cola_Mensajes);
int borrar_cola_de_mensajes(int Id_Cola_Mensajes);
int borrar_mensajes(int id_cola_mensajes);

/* Estructura para los mensajes que se quieren enviar y/o recibir. Deben llevar
 obligatoriamente como primer campo un long para indicar un identificador
 del mensaje. 
 Los siguientes campos son la informaci�n que se quiera transmitir en el  
 mensaje. Cuando m�s adelante, en el c�digo, hagamos un cast a 
 (struct msgbuf *), todos los campos de datos los ver� el sistema como
 un �nico (char *)
*/
/* ejemplo codigo 1*/
/*
typedef struct tipo_mensajes mensaje;
struct tipo_mensajes
{
	long Id_Mensaje;
	int Dato_Numerico;
	char Mensaje[50];						//Numero de nevento
	
};
*/

/* estructura para envio mensajes*/
typedef struct tipo_mensajes mensaje;
struct tipo_mensajes
{
	long	long_dest; 						//Destinatario
	int 	int_rte;						//Remitente
	int 	int_evento;						//Numero de nevento
	char 	char_mensaje[LARGO_TMENSAJE];	//mensajse
};

#endif
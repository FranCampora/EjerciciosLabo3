
#ifndef _MENSAJE
#define _MENSAJE

    int creo_id_cola_mensajes(int clave);
    int enviar_mensaje(int id_cola_mensajes, long dest, int rte);
    int recibir_mensaje(int id_cola_mensajes, long tipo);
    int borrar_cola_de_mensajes(int id_cola_mensajes);
    int borrar_mensajes(int id_cola_mensajes);



    /*typedef struct tipo_mensajes mensaje;*/
    typedef struct tipo_mensajes
    {
        long	long_dest; 						/*Destinatario*/
        int 	int_rte;						/*Remitente*/
        int 	int_evento;						/*Numero de nevento*/
        char 	char_mensaje[256];	/*mensajse*/
    }mensaje;

#endif

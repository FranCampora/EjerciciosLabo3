
#ifndef _MENSAJE
#define _MENSAJE

    typedef struct tipo_mensajes
    {
        long	long_dest; 						/*Destinatario*/
        int 	int_rte;						/*Remitente*/
        int 	int_evento;						/*Numero de nevento*/
        char 	char_mensaje[256];	/*mensajse*/
    }mensaje;

    
    
    
    typedef struct
    {
        int codigo_cliente;
        int importe_depositar;
        int importe_extraer;
        int saldo_inicial;
        int saldo_final;
    }cliente;

    int creo_id_cola_mensajes();
    int enviar_mensaje(int id_cola_mensajes, long destinatario, int remitente, int evento, char* textoMensaje);
    int recibir_mensaje(int id_cola_mensajes, long destinatario, mensaje* respuesta_texto_mensaje);
    int borrar_cola_de_mensajes(int id_cola_mensajes);
    int borrar_mensajes(int id_cola_mensajes);
    cliente* buscar_cliente(char *szCodigo, cliente *clientes, int cantClientes);



    /*typedef struct tipo_mensajes mensaje;*/
    
    

#endif

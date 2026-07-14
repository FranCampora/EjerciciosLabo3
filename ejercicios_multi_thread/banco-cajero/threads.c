#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "global.h"
#include "clave.h"
#include "mensajes.h"
#include "funciones.h"
#include "def.h"

void *funcionThread(void *parametro){

    int nro_jugador;
    int done;
    int monto;
    int opcion;
    int codRta;
    int saldoRta;
    int codCliente;
    int id_cola_mensajes;

    int *puntos_ret;
    char szBuffer[LARGO];
    char szError[LARGO];
    mensaje msg; 
    
    /*THREADS*/


    tjugador *datos_thread = (tjugador*) parametro;

    nro_jugador = datos_thread->numeroJugador;
    monto = datos_thread->monto;
    id_cola_mensajes = datos_thread->id_colamsg;
    codCliente = nro_jugador;
    
    done = 0;
    codRta = 0;
    saldoRta = 0;

    memset(szBuffer,0x00,sizeof(szBuffer));
    memset(&msg,0x00,sizeof(msg));
    memset(szError,0x00,sizeof(szError));
    

    puntos_ret = (int *)malloc(sizeof(int));
    if (puntos_ret == NULL)
    {
        perror("No puedo crear USER\n");
        exit(-1);
    }
    *puntos_ret = 0;
/**/
    /*
    printf("HILO USER %d iniciado. Esperando EVENTO INICIO\n",nro_jugador);
    pthread_mutex_lock(&mutex);
    recibir_mensaje(id_cola_mensajes,nro_jugador, &msg);
    pthread_mutex_unlock(&mutex);
    
    pthread_mutex_lock(&mutex);

    switch (msg.int_evento)
    {
        case EVT_INICIO:
            printf("HILO JUGADOR %d RECIBI EVENTO INICIO\n",nro_jugador);
            break;
        default:
            printf("HILO JUGADOR %d RECIBI EVENTO INESPERADO\n",nro_jugador);
            break;


    pthread_mutex_unlock(&mutex);
    }*/
    
    
    
    printf("SOY EL HILO JUGADOR %d Y VOY A TIRAR LA BOLA\n",nro_jugador);
    while (done == 0)
    {
        opcion = menu();
        /*CASOS DE EVENTOS*/
        switch (opcion)
        {
        case 1:/*consulta saldo*/
            
            memset(szBuffer,0x00,sizeof(szBuffer));
            sprintf(szBuffer,"%d",nro_jugador);

            pthread_mutex_lock(&mutex);
            enviar_mensaje(id_cola_mensajes, MSG_BANCO,nro_jugador,EVT_CONSULTA_SALDO,szBuffer);
            pthread_mutex_unlock(&mutex);
            
            usleep(1000*1000);
            
            printf("CAJERO: ENVIO *CONSULTA DE SALDO* AL CAJERO. Espera respuesta\n");
            

            pthread_mutex_lock(&mutex);
            recibir_mensaje(id_cola_mensajes,nro_jugador,&msg);
            pthread_mutex_unlock(&mutex);
            if (msg.int_evento == EVT_RTA_SALDO)/*del msg de la respuesta del BACNO*//*con que compara este if?*/
            {
                sscanf(msg.char_mensaje,"%d|%d",&codRta,&saldoRta); /*el mensaje q envio el BANCO, extraigo el code y lo pongo en la var "codRta", y lo mismo para el saldo
                con saldoRta*/
                printf("CAJERO: saldo del cliente %d es: $%d\n", codRta, saldoRta);
            }else if (msg.int_evento == EVT_RTA_SALDO_NOK)
            {
                sscanf(msg.char_mensaje,"%d|%s",&codRta,szError);
                printf("ERROR del cliente %d: %s ",codRta,szError);
            }
            
            break;
        case 2:/*depostio*/
            monto = cargarInt("De cuanto queres que sea el deposito?\n");

            memset(szBuffer,0x00,sizeof(szBuffer));
            sprintf(szBuffer,"%d|%d",nro_jugador,monto);

            pthread_mutex_lock(&mutex);
            enviar_mensaje(id_cola_mensajes, MSG_BANCO,nro_jugador,EVT_DEPOSITO,szBuffer);
            pthread_mutex_unlock(&mutex);
            usleep(1000*1000);
            
            printf("CAJERO: ENVIO *DEPOSITO* desde L CAJERO. Espera respuesta\n");
            
            pthread_mutex_lock(&mutex);
            recibir_mensaje(id_cola_mensajes,nro_jugador,&msg);
            pthread_mutex_unlock(&mutex);

            sscanf(msg.char_mensaje,"%d|%d",&codRta,&saldoRta);
            if (msg.int_evento == EVT_RTA_DEPOSITO_OK )
            {
                printf("CAJERO: Recibio respuesta, DEPOSITO EXITOS0 del cajero\n");
                printf("El cliente %d , depositó CORRECTAMENTE Y tiene un SALDO DE:%d",codRta,saldoRta);
            }else if (msg.int_evento == EVT_RTA_DEPOSITO_NOK)
            {
                sscanf(msg.char_mensaje,"%d|%s",&codRta,szError);
                printf("ERROR del cliente %d: %s ",codRta,szError);
            }
            
            
            
            break;
        case 3:/*extraccion*/
            monto = cargarInt("De cuanto queres que sea la extraccion?\n");
        
            memset(szBuffer,0x00,sizeof(szBuffer));
            sprintf(szBuffer,"%d|%d",nro_jugador,monto);

            pthread_mutex_lock(&mutex);
            enviar_mensaje(id_cola_mensajes, MSG_BANCO,nro_jugador,EVT_EXTRACCION,szBuffer);
            pthread_mutex_unlock(&mutex);

            usleep(1000*1000);
            printf("CAJERO: ENVIO *EXTRACCION* AL CAJERO. Espera respuesta\n");
            
            pthread_mutex_lock(&mutex);
            recibir_mensaje(id_cola_mensajes,nro_jugador,&msg);
            pthread_mutex_unlock(&mutex);


            if (msg.int_evento == EVT_RTA_EXTRACCION_OK)
            {
                sscanf(msg.char_mensaje,"%d|%d",&codRta,&saldoRta);
                printf("CAJERO: Recibio RESPUESTA EXTRACCION EXITOSA del cajero\n");
                printf("La extraccion del cliente %d fue de de:%d",codRta,saldoRta);
            }
            else if (msg.int_evento == EVT_RTA_EXTRACCION_NOK)
            {
                sscanf(msg.char_mensaje,"%d|%s",&codRta,szError);
                printf("ERROR del cliente %d: %s ",codRta,szError);
            }
            
            break;
        case 0:/*salir*/
            pthread_mutex_lock(&mutex);
            enviar_mensaje(id_cola_mensajes,MSG_BANCO,nro_jugador,EVT_FIN,"FIN");
            pthread_mutex_unlock(&mutex);
            usleep(1000*1000);
            
            printf("CAJERO: ENVIO *FIN* AL CAJERO. Espera respuesta\n");
            
        
            pthread_mutex_lock(&mutex);
            recibir_mensaje(id_cola_mensajes,nro_jugador,&msg);
            pthread_mutex_unlock(&mutex);
            printf("CAJERO: Recibio RESPUESTA de FIN del cajero. ADIOS CLIENTE %d\n",codCliente);
            printf("El programa finalizo");
            done = 1;
            
            break;
        



        default:
            printf("Opcion invalida");
            
            break;
        }
    }

    printf("Hijo cajero %d: termino\n",nro_jugador);
    pthread_exit((void *)puntos_ret);
}

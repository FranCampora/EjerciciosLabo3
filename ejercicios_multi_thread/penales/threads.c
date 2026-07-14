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
    char szBuffer[LARGO];
    int tipoTiro;
    
    int id_cola_mensajes;
    

    mensaje msg; 
    
    tjugador *datos_thread = (tjugador*) parametro;
    nro_jugador = datos_thread->numeroJugador;
    id_cola_mensajes = datos_thread->id_colamsg;
    tipoTiro = datos_thread->tipoTiro;

    done = 0;
    
    printf("HILO JUGADOR %d iniciado. \n",nro_jugador);

    pthread_mutex_lock(&mutex);
        /*sprintf(str_dato,"%d",)*/
    memset(szBuffer,0x00,sizeof(szBuffer));
    sprintf(szBuffer,"%d",nro_jugador);
                
    enviar_mensaje(id_cola_mensajes,MSG_ARQUERO,nro_jugador,EVT_TIRO,szBuffer);
    printf("Hilo envio tiro\n");
    
    pthread_mutex_unlock(&mutex);
    

    recibir_mensaje(id_cola_mensajes, PROCESO_JUGADOR+nro_jugador, &msg);
    pthread_mutex_lock(&mutex);


    switch (msg.int_evento)
    {
        case EVT_GOL:
            printf("HILO JUGADOR %d RECIBI EVENTO gol\n",nro_jugador);
            printf("GOOOL\n");
            datos_thread->tipoTiro = EVT_GOL;
            break;
        case EVT_PALO:
            printf("HILO JUGADOR %d RECIBI EVENTO palo\n",nro_jugador);
            datos_thread->tipoTiro = EVT_PALO;
            break;
        case EVT_TRAVESANO:
            printf("HILO JUGADOR %d RECIBI EVENTO travesaño\n",nro_jugador);
            datos_thread->tipoTiro = EVT_TRAVESANO;
            break;
        case EVT_FUERA:
            printf("HILO JUGADOR %d RECIBI EVENTO fuera\n",nro_jugador);
            datos_thread->tipoTiro = EVT_FUERA;
            break;
        
        default:
            printf("HILO JUGADOR %d RECIBI EVENTO INESPERADO\n",nro_jugador);
            datos_thread->tipoTiro = EVT_NINGUNO;
            break;
    }
    
    pthread_mutex_unlock(&mutex);
    

    printf("SOY EL HIJO Y ESPERO 10 SEGUNDOS\n");
    
    printf("Hijo: termino\n");
    pthread_exit(NULL);
}

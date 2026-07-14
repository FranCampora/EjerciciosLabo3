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
    int pinos_tirados;
    int *puntos_ret;
    int id_cola_mensajes;

    mensaje msg; 
    
    tjugador *datos_thread = (tjugador*) parametro;
    nro_jugador = datos_thread->numeroJugador;
    pinos_tirados = datos_thread->cantidadTiros;
    id_cola_mensajes = datos_thread->id_colamsg;

    done = 0;
    

    puntos_ret = (int *)malloc(sizeof(int));
    if (puntos_ret == NULL)
    {
        perror("No puedo crear jugador\n");
        exit(-1);
    }
/**/
    printf("HILO JUGADOR %d iniciado. Esperando EVENTO INICIO\n",nro_jugador);
    
    recibir_mensaje(id_cola_mensajes, PROCESO_JUGADOR+nro_jugador, &msg);
    pthread_mutex_lock(&mutex);

    switch (msg.int_evento)
    {
        case EVT_INICIO:
            printf("HILO JUGADOR %d RECIBI EVENTO INICIO\n",nro_jugador);
            break;
        default:
            printf("HILO JUGADOR %d RECIBI EVENTO INESPERADO\n",nro_jugador);
            break;



    }
    pthread_mutex_unlock(&mutex);
    
    
    printf("SOY EL HILO JUGADOR %d Y VOY A TIRAR LA BOLA\n",nro_jugador);
    while (done == 0)
    {
        enviar_mensaje(id_cola_mensajes,PROCESO_BOLERO,PROCESO_JUGADOR+nro_jugador,EVT_TIRO,"TIRO");

        recibir_mensaje(id_cola_mensajes, PROCESO_JUGADOR+nro_jugador, &msg);
        pthread_mutex_lock(&mutex);
        switch (msg.int_evento)
        {
            /*
            
        case EVT_RTA_TIRO_OK:
            printf("HILO JUGADOR %d RECIBI EVENTO RTA_TIRO_OK\n",nro_jugador);
            break;*/
        case EVT_RESULTADO:
        
            pinos_tirados = atoi(msg.char_mensaje);
            printf("HILO JUGADOR %d RECIBI EVENTO RESULTADO y tire %d pinos\n",nro_jugador, pinos_tirados);
            /*usleep(1000*1000);*/
            
            break;
        case EVT_FIN:
            /* DEBUG: ver exactamente que llega en el mensaje */
            printf("DEBUG JUGADOR %d EVT_FIN: int_evento=%d char_mensaje='%s'\n",
                nro_jugador,
                msg.int_evento,
                msg.char_mensaje);
            *puntos_ret = atoi(msg.char_mensaje);
            printf("HILO JUGADOR %d RECIBI EVENTO FIN con %d puntos\n",
                   nro_jugador, *puntos_ret);
            
            done = 1;
            /*pthread_mutex_unlock(&mutex);*/
            break;
        default:
            *puntos_ret = atoi(msg.char_mensaje);  
            printf("... %d puntos", *puntos_ret);
            done = 1;
        }
        pthread_mutex_unlock(&mutex);

        if (done == 0)
        {
            usleep(300*1000);
        }
        
    }
    
    

    printf("SOY EL HIJO Y ESPERO 10 SEGUNDOS\n");
    
    printf("Hijo: termino\n");
    pthread_exit((void *)puntos_ret);
}

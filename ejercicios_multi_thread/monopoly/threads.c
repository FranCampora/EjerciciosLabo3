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

    
    int prePosicion;
    int turnoActivo;
    int id_cola_mensajes;
    char nombreJugador[LARGO];
    char szBuffer[LARGO];
    int posicion;
    int dado;
    int monto;

    mensaje msg; 
    tjugador *datos_thread = (tjugador*) parametro;

    srand(time(NULL));

    

    nro_jugador = datos_thread->numeroJugador;
    id_cola_mensajes = datos_thread->id_colamsg;
    strcpy(nombreJugador, datos_thread->nombre_jug);
    monto = datos_thread->monto;
    posicion = datos_thread->posicion;
    turnoActivo = datos_thread->turnoActivo;
    
    done = 0;
    
    
    recibir_mensaje(id_cola_mensajes,MSG_JUGADOR1+nro_jugador,&msg);
    pthread_mutex_lock(&mutex);
    if (msg.int_evento == EVT_INICIO)
    {
        printf("HILO JUGADOR %d iniciado. \n",nro_jugador);

    }
    pthread_mutex_unlock(&mutex);
    while (done == 0)
    {

        printf("La posicion actual del jugador %d es la posicion %d",nro_jugador,posicion);
        prePosicion = posicion;
        
        /*tira los dados para desp mover*/
        if (turnoActivo == 1)
        {
            pthread_mutex_lock(&mutex);
            dado = devolverNumAleatorio(DESDE,HASTA);/*entre 0 y 3*/
            printf("Jugador %d tira dado y sale %d\n",nro_jugador,dado);
            pthread_mutex_unlock(&mutex);

            printf("El jugador %d tiene que mover %d posiciones\n",nro_jugador,dado);

            /*actualizo posicion del jugador*/
            posicion = prePosicion + dado;
            
            if (posicion == 40)
            {
                pthread_mutex_lock(&mutex);
                memset(szBuffer,0x00,sizeof(szBuffer));
                sprintf(szBuffer,"|%d|%d|%d",nro_jugador,posicion,monto);
                turnoActivo = 0;
                done = 1;
                enviar_mensaje(id_cola_mensajes,MSG_PISTA,MSG_JUGADOR1+nro_jugador,EVT_FIN,szBuffer);
                pthread_mutex_unlock(&mutex);
            }else/*no es 40 la posicion*/
            {
                pthread_mutex_lock(&mutex);
                memset(szBuffer,0x00,sizeof(szBuffer));
                sprintf(szBuffer,"%d|%d|%d|%d",nro_jugador,posicion,monto,turnoActivo);
                enviar_mensaje(id_cola_mensajes,MSG_PISTA,MSG_JUGADOR1+nro_jugador,EVT_CORRO,szBuffer);
                pthread_mutex_unlock(&mutex);
                
                printf("El hilo jugador %d se movio y ahora esta en la posicion %d\n",nro_jugador,posicion);

            }
        }else
            {
                printf("El jugador %d salto el turno\n",nro_jugador);
                turnoActivo = 1;
            }
        if (done == 0)
        {
            recibir_mensaje(id_cola_mensajes,MSG_JUGADOR1+nro_jugador,&msg);
            pthread_mutex_lock(&mutex);
            
            if (msg.int_evento == EVT_RTA_TABLERO)
            {
                memset(szBuffer,0x00,sizeof(szBuffer));
                sscanf(msg.char_mensaje,"%d|%d|%d|%d",&monto,&turnoActivo);
                
                datos_thread->monto = monto;
                datos_thread->turnoActivo = turnoActivo;
                
            }
            pthread_mutex_unlock(&mutex);
            }
        
        
        

    }
    
    
    
    printf("Hijo: termino\n");
    pthread_exit(NULL);
}

#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

#include "def.h"
#include "global.h"
#include "semaforo.h"
#include "archivos.h"
#include "funciones.h"
#include "clave.h"
#include "memoria.h"
#include "mensajes.h"
#include "threads.h"

int main(int argc, char *argv[])
{
    int id_cola_mensajes;
    int golesTotales;
    int i;
    


    tjugador *datos_thread;
    pthread_t idHilo[MAX_USERS];
    pthread_attr_t atributos;/*struct para los atributos del hilo*/

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    
    /*THREADS*/
    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/
    PTHREAD_CREATE_JOINABLE;
    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/

    golesTotales = 0;

    printf("PROCESO jugador LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    /*PREPARAR DATOS PARA CADA HILO*/
    datos_thread = (tjugador*) malloc(sizeof(tjugador)*MAX_USERS);
    id_cola_mensajes = creo_id_cola_mensajes();

    if (datos_thread == NULL)
        {
            printf("No puedo crear jugador\n");
            exit(-1);
    }

    for (i = 0; i < MAX_USERS; i++)
    {
        datos_thread[i].numeroJugador = MSG_PATEADOR+i;
        datos_thread[i].id_colamsg = id_cola_mensajes;
        datos_thread[i].tipoTiro = EVT_NINGUNO;
        
        if (pthread_create(&idHilo[i],&atributos,&funcionThread,&datos_thread[i])!= 0)
        {
            perror("No puedo crear el hilo\n");
            exit(-1);
        }
        printf("HILO JUGADOR %d CREADO\n",MSG_PATEADOR+i);
        
    }
    
    printf("------------------------------\n");

    printf("JUGADOR: ESPERO A LOS HILOS\n");
    printf("------------------------------\n");
    
    printf("RESULTADOS FINALES\n");

    for (i = 0; i < MAX_USERS; i++){
        /*pthread_join(idHilo[i],(void **)&valorDevuelto[i]);*/
    
        
        pthread_join(idHilo[i],NULL);
    }
    printf("------------------------------\n");
    
    printf("RESULTADOS FINALES\n");
    for (i = 0; i < MAX_USERS; i++)
    {
        switch (datos_thread[i].tipoTiro)
        {
            case EVT_GOL:
                printf("PATEADOR %d: GOL\n", MSG_PATEADOR + i);
                golesTotales++;
                break;
            case EVT_PALO:
                printf("PATEADOR %d: PALO\n", MSG_PATEADOR + i);
                break;
            case EVT_TRAVESANO:
                printf("PATEADOR %d: TRAVESANIO\n", MSG_PATEADOR + i);
                break;
            case EVT_FUERA:
                printf("PATEADOR %d: FUERA\n", MSG_PATEADOR + i);
                break;
            default:
                printf("PATEADOR %d: NINGUNO\n", MSG_PATEADOR + i);
                break;
        }
        
    }
    
    printf("------------------------------\n");
    printf("Goles totales %d\n",golesTotales);
    
    free(datos_thread);
    return 0;
}

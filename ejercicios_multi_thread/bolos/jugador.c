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

/*laburo de jugador.c es crear los hilos
no recibir los mensajes del bolera*/
int main(int argc, char  *argv[])
{
    int id_cola_mensajes;

    int i;
    int *valorDevuelto[MAX_USERS];


    tjugador *datos_thread;
    pthread_t idHilo[MAX_USERS];
    pthread_attr_t atributos;/*struct para los atributos del hilo*/

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    
    /*THREADS*/
    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/

    PTHREAD_CREATE_JOINABLE;

    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/

    printf("PROCESO jugador LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    /*PREPARAR DATOS PARA CADA HILO*/
    datos_thread = (tjugador*) malloc(sizeof(tjugador)*MAX_USERS);

    id_cola_mensajes = creo_id_cola_mensajes();
    if (datos_thread == NULL)
        {
            perror("No puedo crear jugador\n");
            exit(-1);
    }
    
    for (i = 0; i < MAX_USERS; i++)
    {
        datos_thread[i].numeroJugador = i+1;
        datos_thread[i].cantidadTiros = 0;
        datos_thread[i].cantidadPinosEnPie[0] = 10;/*en la pisicion 0 hay 10 para el J1, en la posicion 1 para el J2 hay 10 */
        datos_thread[i].cantidadPinosEnPie[1] = 10;
        datos_thread[i].id_colamsg = id_cola_mensajes;
        if (pthread_create(&idHilo[i],&atributos,&funcionThread,&datos_thread[i]) != 0)
        {
            perror("No puedo crear el hilo\n");
            exit(-1);
        }
        printf("HILO JUGADOR %d CREADO\n",i+1);
        
    }
    
    printf("------------------------------\n");

    printf("JUGADOR: ESPERO A LOS HILOS\n");

    for (i = 0; i < MAX_USERS; i++){
        pthread_join(idHilo[i],(void **)&valorDevuelto[i]);/**/
        printf("JUGADOR %d termino: puntos %d\n",i+1, *valorDevuelto[i]);
        /*pthread_join(idHilo,NULL);*/
    }
    printf("------------------------------\n");
    
    printf("RESULTADOS FINALES:\n");
    for (i = 0; i < MAX_USERS; i++){
        printf("JUGADOR %d: puntos %d\n",i+1, *valorDevuelto[i]);
    }
    if (*valorDevuelto[0] > *valorDevuelto[1])
    {
        printf("JUGADOR 1 GANA!\n");
    }
    else if (*valorDevuelto[1] > *valorDevuelto[0])
    {
        printf("JUGADOR 2 GANA!\n");
    }
    
    

    printf("JUGADOR: TERMINE EL THREAD\n");
    

    free(datos_thread);
    
    return 0;
}

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
    int cantJugadores;

    int i;
    int idHilo;

    tjugador *datos_thread;
    pthread_attr_t atributos;
    pthread_t idHilo[MAX_JUGADORES];
    mensaje msg;

    srand(time(NULL));
    
    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    if(argc != 2)
		{
			printf("Ingrese la cantidad de jugadores\n");
			exit(1);
		}

    cantJugadores = atoi(argv[1]);
    if (cantJugadores < 1)
    {
        printf("La cantidad de jugadores debe ser un número positivo\n");
        exit(1);
    }

    memset(datos_thread,  0x00, sizeof(tjugador) * cantJugadores);
    memset(valorDevuelto, 0x00, sizeof(int *) * cantJugadores);

    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/
    PTHREAD_CREATE_JOINABLE;
    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/

    printf("PROCESO jugador LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    id_cola_mensajes = creo_id_cola_mensajes();
    if (datos_thread == NULL)
    {
        perror("No puedo crear jugador\n");
        exit(-1);
    }

    /*creo los hilos, 1 x cada jugador*/
    for (i = 0; i < cantJugadores; i++)
    {
        datos_thread[i].numeroJugador = i+1;
        datos_thread[i].id_colamsg = id_cola_mensajes;
        datos_thread[i].cantJugadores = cantJugadores;
        
        if (pthread_create(&idHilo[i],&atributos,&funcionThreadJugador,&datos_thread[i]) != 0)
        {
            perror("No puedo crear el hilo\n");
            exit(-1);
        }
        printf("HILO JUGADOR %d CREADO\n",i+1);
    }

    /*espero que los hilos de jugadores hayan terminado*/
    for (i = 0; i < cantJugadores; i++){
        pthread_join(idHilo[i],(void **)&valorDevuelto[i]);
        
        /*pthread_join(idHilo,NULL);*/
        if (valorDevuelto[i] != NULL)
        {
            printf("JUGADOR %d termino de crear los cartones:\n",i+1);
        
        }
        
        
    }
    usleep(100*1000);

    free(datos_thread);
    
    return 0;
}

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
    int i;
    int id_cola_mensajes;
    
    int cantJugadores;
    int montoIncial;
    char szBuffer[LARGO];
    char nombreJugador[LARGO];

    mensaje msg;
    tjugador *datos_thread;

    pthread_t idHilo[100];
    pthread_attr_t atributos;/*struct para los atributos del hilo*/

    

    /*THREADS*/
    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/
    
    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/


    memset(&msg,0x00,sizeof(msg));
    memset(szBuffer,0x00,sizeof(szBuffer));


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

    datos_thread = (tjugador*) malloc(sizeof(tjugador) * (cantJugadores + 1));

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    id_cola_mensajes = creo_id_cola_mensajes();

    printf("PROCESO jugador LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    montoIncial = cargarInt("Cargue el monto inicial que tendran todos los jugadores, debe ser entre 500 y 800\n");
    while (montoIncial > MONTO_INICIAL_MAX || montoIncial < MONTO_INICIAL_MIN)
    {
        montoIncial = cargarInt("Cargue el monto inicial que tendran todos los jugadores, debe ser entre 500 y 800\n");
    }

    
    for (i = 1; i <= cantJugadores; i++)
    {
        datos_thread[i].numeroJugador = i;
        datos_thread[i].monto = montoIncial;
        datos_thread[i].posicion = 0;
        datos_thread[i].turnoActivo = 1;
        datos_thread[i].id_colamsg = id_cola_mensajes;
        cargarData("Ingrese nombre del jugador",nombreJugador);
        strcpy(datos_thread[i].nombre_jug, nombreJugador);
        if (pthread_create(&idHilo[i],&atributos,&funcionThread,&datos_thread[i])!= 0)
        {
            perror("No puedo crear el hilo\n");
            exit(-1);
        }
        printf("HILO JUGADOR %d CREADO\n",i);
    }
    
    printf("------------------------------\n");

    printf("JUGADOR: ESPERO A LOS HILOS\n");
    printf("------------------------------\n");
    
    for ( i = 1; i <= cantJugadores; i++)
    {
        pthread_join(idHilo[i],NULL);
    }
    
    return 0;
}




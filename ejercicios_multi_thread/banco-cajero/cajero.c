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

int main(int argc, char  *argv[])
{
    int id_cola_mensajes;

    int codCliente; 
    int *valorDevuelto;

    tjugador *datos_thread;
    pthread_t idHilo;
    pthread_attr_t atributos;/*struct para los atributos del hilo*/

    
    esperarArchivoSincronismo(FILE_SINCRONISMO,1);

    /*THREADS*/
    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/
    PTHREAD_CREATE_JOINABLE;
    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/
    
    datos_thread = (tjugador*) malloc(sizeof(tjugador));
    valorDevuelto = 0;
    
    id_cola_mensajes = creo_id_cola_mensajes();

    if (datos_thread == NULL)
        {
            perror("No puedo crear jugador\n");
            exit(-1);
    }
    printf("------------------------------\n");

    printf("PROCESO CAJERO LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    printf("MENU DEL CLINTE");

    codCliente= cargarInt("Ingrese codigo del clinte: \n");
    printf("Esta trabajando el cliente con el codigo %d",codCliente);

    datos_thread->numeroJugador = codCliente;
    datos_thread->id_colamsg = id_cola_mensajes;
    datos_thread->monto = SALDO_INICIAL;

    if (pthread_create(&idHilo, &atributos, &funcionThread, datos_thread) != 0)
    {
        perror("No puedo crear el hilo\n");
        exit(-1);
    }
    printf("HILO CLIENTE %d CREADO\n",codCliente);
        
    
    printf("CAJERO: ESPERO A LOS HILOS\n");

    /*pthread_join(idHilo[codCliente],(void **)&valorDevuelto[codCliente]);*/
    pthread_join(idHilo,NULL);
    printf("------------------------------\n");

    
    
    return 0;
}

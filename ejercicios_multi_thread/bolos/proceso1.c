#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
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
    pthread_t idHilo;
    pthread_attr_t atributos;/*struct para los atributos del hilo*/
    /*int error;*/
    int i;
    char *valorDevuelto;/*puntero para EL RESULTADO FINAL CUANDO CORTE EL HILO EN THREADS.C*/


    valorDevuelto = NULL;

    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/

    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/


    /*ESTAOD INICIAL DEL HILO*/
    PTHREAD_CREATE_JOINABLE;

    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/
    /*le estoy diciendo que los atributos son joinable*/

    if (pthread_create(&idHilo,&atributos,&funcionThread,NULL)!= 0)/*crea el hilo HIJO, con funcion en funcion*/
    /*uso el putero a funcion. Null es el parametro del puntero a funcion*/
    {
        perror("No puedo crear THREAD\n");
        exit(-1);
    }
    
    for (i = 0; i < CANTIDAD_THREADS; i++)
    {
        pthread_mutex_lock(&mutex);/*SOLICITA MUTEX, SI EL HIJO LO TIENE, EL PADRE SE BLOQUE ACA/ esta var del mutex esta en global.h*/
        printf("SOY EL PADRE Y TENGO EL MUTEX\n");
        usleep(1000*1000);
        pthread_mutex_unlock(&mutex);/*LIBERA EL MUTEX*/
        usleep(1000*1000);
    }

    printf("PADRE: ESPERO AL THREAD\n");

    pthread_join(idHilo,(void **)&valorDevuelto);/**/
    /*pthread_join(idHilo,NULL);*/

    printf("PADRE: TERMINE EL THREAD\n");
    printf("PADRE: DEVUELVE |%s|\n",valorDevuelto);
    
    /*free(valorDevuelto);*/

    return 0;
}

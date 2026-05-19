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
    int i;
    printf("HIJO\n");
    for (i = 0; i < CANTIDAD_THREADS; i++)
    {
        pthread_mutex_lock(&mutex);/*BLOQUEO DE MUTEX, SI UN HILO A QUIERE EL MUTEX Y  OTRO HILO B TIENE EL MUTE. HILO A ESPERA ACA */
        printf("SOY EL HIJO Y TENGO EL MUTEX\n");
        usleep(1000*1000);
        pthread_mutex_unlock(&mutex);/*LIBERA EL MUTEX, PARA QUE OTROS HILOS LO PUEDAN USAR*/
        usleep(1000*1000);
    
    }

    printf("SOY EL HIJO Y ESPERO 10 SEGUNDOS\n");
    usleep(10000*1000);
    printf("Hijo: termino\n");
    pthread_exit((void*)"LISTO");/*el thread sale y termino*/
}

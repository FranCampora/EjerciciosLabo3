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
    int id_memoria;
    char szBuffer[LARGO];

    mensaje msg;
    tmaritimo *datos_thread;
    tmaritimo *memoria; 
    
    pthread_t idHilo[MAX_MARITIMOS];
    pthread_attr_t atributos;

    /*THREADS*/
    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/
    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/

    datos_thread = (tmaritimo*) malloc(sizeof(tmaritimo) * MAX_MARITIMOS);
    
    if (datos_thread == NULL){
        printf("Error: al crear EL AUTO\n");
        exit(-1);
    }

    /*limpio vars y asigno espacio*/
    memset(&msg,0x00,sizeof(msg));
    memset(szBuffer,0x00,sizeof(szBuffer));
    memset(datos_thread, 0x00, sizeof(tmaritimo) * MAX_MARITIMOS);/*max_usuarios va a cambiar*/

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    id_cola_mensajes = creo_id_cola_mensajes();
    memoria = (tmaritimo*) creo_memoria(sizeof(tmaritimo) * MAX_MARITIMOS, &id_memoria);
    
    printf("PROCESO MARITIMO LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    /*logica*/
    /*cargar los saldos inciales de los autos*/
    /*lanzo hilos*/

    for ( i = 0; i < MAX_MARITIMOS; i++)
    {
        datos_thread[i].id_cola_msg = id_cola_mensajes;
        datos_thread[i].tipo_maritimo = memoria[i].tipo_maritimo;
        datos_thread[i].numero_maritimo = memoria[i].numero_maritimo;
        datos_thread[i].posicion = 0;
        datos_thread[i].estado = memoria[i].estado;
        datos_thread[i].uso_poder = NO_USO_PODER;
        datos_thread[i].cant_vuelta = 0;
        datos_thread[i].distancia_meta = memoria[i].distancia_meta;

        if (datos_thread[i].tipo_maritimo == PEZ)
        {
            cargarData("Ingrese el nombre del pez (opcional): ", datos_thread[i].nombre_maritimo);
        }
        else if (datos_thread[i].tipo_maritimo == TIBURON)
        {
            cargarData("Ingrese el nombre del tiburon (opcional): ", datos_thread[i].nombre_maritimo);
        }
        else
        {
            cargarData("Ingrese el nombre del pulpo (opcional): ", datos_thread[i].nombre_maritimo);
        }

        printf("Maritimo %d (%s): tipo asignado %d\n", datos_thread[i].numero_maritimo, datos_thread[i].nombre_maritimo, datos_thread[i].tipo_maritimo);
    }

    printf("--------------------------\n");
    printf("LANZO LOS HILOS\n");

    for ( i = 0; i < MAX_MARITIMOS; i++)
    {
        if (pthread_create(&idHilo[i],&atributos,&funcionThread,&datos_thread[i])!= 0)
        {
            printf("No puedo crear el hilo\n");
            exit(1);
        }
        printf("Hilo maritimo %d creado\n",i+1);
    }
    
    printf("--------------------------\n");
    printf("ESPERO A LOS HILOS\n");

    for ( i = 0; i < MAX_MARITIMOS; i++)
    {
        pthread_join(idHilo[i],NULL);
    }
    printf("Hilos finalizados\n");
    /*for si tengo que mostrar un dato de todos los hilos usando datos_thread[i]
    for ( i = 0; i < MAX_USUARIOS; i++){
        printf("El usuario %d: %s\n",datos_thread[i].numero,datos_thread[i].nombre_entidad_financiera);
    }
    */

    /*shmdt((char *)memoria);*/
    free(datos_thread);
    /*free(idHilo);*/
    return 0;
}

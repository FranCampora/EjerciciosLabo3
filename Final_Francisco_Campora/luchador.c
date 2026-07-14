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

    int opcion_menu;

    mensaje msg;
    tluchador *datos_thread;
    tluchador *memoria; 
    
    pthread_t idHilo[MAX_LUCHADORES];
    pthread_attr_t atributos;

    /*THREADS*/
    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/
    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/

    datos_thread = (tluchador*) malloc(sizeof(tluchador) * MAX_LUCHADORES);
    
    if (datos_thread == NULL){
        printf("Error: al crear EL LUCHADOR\n");
        exit(-1);
    }

    /*limpio vars y asigno espacio*/
    memset(&msg,0x00,sizeof(msg));
    memset(szBuffer,0x00,sizeof(szBuffer));
    memset(datos_thread, 0x00, sizeof(tluchador) * MAX_LUCHADORES);/*max_luchadores va a cambiar*/

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    id_cola_mensajes = creo_id_cola_mensajes();
    memoria = (tluchador*) creo_memoria(sizeof(tluchador) * MAX_LUCHADORES, &id_memoria);
    
    printf("PROCESO LUCHADOR LISTA PARA EMPEZAR. Enter para empezar\n");
    getchar();

    opcion_menu = cargarInt("\nMenu\n1. Cargar datos de luchadores y comenzar\n0. Salir del programa\nIngrese una opcion: ");
    
    while (opcion_menu != 1 && opcion_menu != 0)
    {
        printf("Error: Opcion invalida\n");
        opcion_menu = cargarInt("\nMenu\n1. Cargar datos de luchadores y comenzar\n0. Salir del programa\nIngrese una opcion: ");
    }

    if (opcion_menu == 0)
    {
        printf("Saliendo del programa de luchadore\n");
        
        /*para q coliseo tambien corte*/
        memset(szBuffer, 0x00, sizeof(szBuffer));
        sprintf(szBuffer, "0|0|0|0|0");
        enviar_mensaje(id_cola_mensajes, MSG_COLISEO, MSG_LUCHADOR, EVT_FIN, szBuffer);
        
        free(datos_thread);
        exit(0);
    }

    /*logica*/
    
    /*lanzo hilos*/

    for ( i = 0; i < MAX_LUCHADORES; i++)
    {
        datos_thread[i].id_cola_msg = id_cola_mensajes;
        datos_thread[i].numero_luchador = memoria[i].numero_luchador;
        datos_thread[i].tipo_luchador = memoria[i].tipo_luchador;
        
    

        if (datos_thread[i].tipo_luchador == IMPERIO)
        {
            cargarData("Ingrese el nombre del imperio: ", datos_thread[i].nombre_luchador);
        }
        else if (datos_thread[i].tipo_luchador == GLADIADOR)
        {
            cargarData("Ingrese el nombre del gladiador: ", datos_thread[i].nombre_luchador);
        }
        datos_thread[i].arma = cargarInt("Ingrese el arma del luchador (1: espada, 2: lanza, 3: otro): ");
        while (datos_thread[i].arma < 1 || datos_thread[i].arma > 3 )
        {
            datos_thread[i].arma = cargarInt("Ingrese el arma del luchador (1: espada, 2: lanza, 3: otro): ");
        
        }
        
        datos_thread[i].vida = memoria[i].vida;
        datos_thread[i].estado = memoria[i].estado;
        

        printf("Luchador %d (%s): tipo asignado %d\n", datos_thread[i].numero_luchador, datos_thread[i].nombre_luchador, datos_thread[i].tipo_luchador);
        /*chekeo con un print que se cargo todo bien*/
    }

    printf("--------------------------\n");
    printf("LANZO LOS HILOS\n");

    for ( i = 0; i < MAX_LUCHADORES; i++)
    {
        if (pthread_create(&idHilo[i],&atributos,&funcionThread,&datos_thread[i])!= 0)
        {
            printf("No puedo crear el hilo\n");
            exit(1);
        }
        printf("Hilo luchador %d creado\n",i+1);
    }
    
    printf("--------------------------\n");
    printf("ESPERO A LOS HILOS\n");

    for ( i = 0; i < MAX_LUCHADORES; i++)
    {
        pthread_join(idHilo[i],NULL);
    }
    printf("Hilos finalizados\n");
    

    /*shmdt((char *)memoria);*/
    free(datos_thread);
    /*free(idHilo);*/
    return 0;
}

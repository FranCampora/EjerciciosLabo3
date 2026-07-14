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
    int saldo_usuario; 





    mensaje msg;
    tusuario *datos_thread;     
    tusuario *memoria;     
    
    pthread_t idHilo[MAX_USUARIOS];
    pthread_attr_t atributos;

    /*THREADS*/
    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/
    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/

    /*id_hilo = (pthread_t* ) malloc(sizeof(pthread_t)*CANTIDAD_HILOS);*/
    datos_thread = (tusuario*) malloc(sizeof(tusuario) * MAX_USUARIOS);
    
    memoria = (tusuario*) creo_memoria(sizeof(tusuario) * MAX_USUARIOS, &id_memoria);
    if (datos_thread == NULL){
        printf("Error: al crear usuario\n");
        exit(-1);
    }

    /*limpio vars y asigno espacio*/
    memset(&msg,0x00,sizeof(msg));
    memset(szBuffer,0x00,sizeof(szBuffer));
    memset(datos_thread, 0x00, sizeof(tusuario) * MAX_USUARIOS);/*max_usuarios va a cambiar*/
    memset(memoria, 0x00, sizeof(tusuario) * MAX_USUARIOS);
    saldo_usuario = 0;



    /*copio los datos en vars locales de memoria compartida*/
    memoria[0].hora_minima_laboral = HORA_INICIAL_LABORAL;
    memoria[0].hora_maxima_laboral = HORA_MAXIMA_LABORAL;

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    id_cola_mensajes = creo_id_cola_mensajes();


    printf("PROCESO USUARIO LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    /*logica*/
    /*cargar los saldos inciales de los usuarios*/
    for ( i = 0; i < MAX_USUARIOS; i++)
    {
        cargarData("Ingrese el nombre del usuario: ", memoria[i].nombre_usuario);
        memoria[i].saldo_usuario = cargarInt("Ingrese el saldo inicial del usuario: ");
    }
    
    
    /*lanzo hilos*/

    for ( i = 0; i < MAX_USUARIOS; i++)
    {
        
        datos_thread[i].numero_usuario    = i + 1;
        datos_thread[i].id_cola_msg = id_cola_mensajes;
        datos_thread[i].saldo_usuario = memoria[i].saldo_usuario;
        datos_thread[i].dias_invertido = 0;
        datos_thread[i].ganancia = 0;
        datos_thread[i].hora_minima_laboral = memoria[0].hora_minima_laboral;
        datos_thread[i].hora_maxima_laboral = memoria[0].hora_maxima_laboral;
        strcpy(datos_thread[i].nombre_usuario, memoria[i].nombre_usuario);
        strcpy(datos_thread[i].nombre_entidad_financiera, memoria[0].nombre_entidad_financiera);
        



        
        if (pthread_create(&idHilo[i],&atributos,&funcionThread,&datos_thread[i])!= 0)
        {
            printf("No puedo crear el hilo\n");
            exit(1);
        }
        printf("Hilo usuario %d creado\n",i);
    }
    
    printf("--------------------------\n");
    printf("ESPERO A LOS HILOS\n");

    for ( i = 0; i < MAX_USUARIOS; i++)
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

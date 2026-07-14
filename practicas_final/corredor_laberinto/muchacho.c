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
    tcorredor *datos_thread;
    tcorredor *memoria; 
    
    pthread_t idHilo[MAX_CORREDORES];
    pthread_attr_t atributos;

    /*THREADS*/
    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/
    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/

    datos_thread = (tcorredor*) malloc(sizeof(tcorredor) * MAX_CORREDORES);
    if (datos_thread == NULL){
        printf("Error: al crear EL corredor\n");
        exit(-1);
    }

    /*limpio vars y asigno espacio*/
    memset(&msg,0x00,sizeof(msg));
    memset(szBuffer,0x00,sizeof(szBuffer));
    memset(datos_thread, 0x00, sizeof(tcorredor) * MAX_CORREDORES);/*max_usuarios va a cambiar*/

    /* orden AVANZADOS obligatorio: esperar archivo -> conectar cola -> conectar memoria */
    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    id_cola_mensajes = creo_id_cola_mensajes();
    memoria = (tcorredor*) creo_memoria(sizeof(tcorredor) * MAX_CORREDORES, &id_memoria);

    printf("PROCESO auto LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    /*logica*/
    /*cargar los saldos inciales de los autos*/
    /*lanzo hilos*/

    for ( i = 0; i < MAX_CORREDORES; i++)
    {
        datos_thread[i].id_cola_msg = id_cola_mensajes;
        datos_thread[i].tipo_corredor = memoria[i].tipo_corredor;
        datos_thread[i].metraje_entrada = memoria[i].metraje_entrada;
        datos_thread[i].posicion = 0;
        datos_thread[i].estado = ESTADO_CORRIENDO;
        datos_thread[i].msg_destino = memoria[i].msg_destino;
        datos_thread[i].msg_rival = memoria[i].msg_rival;

        if (datos_thread[i].tipo_corredor == TIPO_THOMAS)
        {
            strcpy(datos_thread[i].nombre_corredor, NOMBRE_THOMAS);
        }
        else if (datos_thread[i].tipo_corredor == TIPO_MUCHACHO)
        {
            cargarData("Ingrese el nombre del muchacho: ", datos_thread[i].nombre_corredor);
        }
        else
        {
            cargarData("Ingrese el nombre de la criatura: ", datos_thread[i].nombre_corredor);
        }

        printf("Corredor %d (%s): tipo %d, entrada %dmt, destino %d, rival %d\n",
            i, datos_thread[i].nombre_corredor, datos_thread[i].tipo_corredor,
            datos_thread[i].metraje_entrada, datos_thread[i].msg_destino, datos_thread[i].msg_rival);
    }
    
    printf("--------------------------\n");
    printf("LANZO LOS HILOS\n");

    for ( i = 0; i < MAX_CORREDORES; i++)
    {
        if (pthread_create(&idHilo[i],&atributos,&funcionThread,&datos_thread[i])!= 0)
        {
            printf("No puedo crear el hilo\n");
            exit(1);
        }
        printf("Hilo planeta %d creado\n",i+1);
    }
    
    printf("--------------------------\n");
    printf("ESPERO A LOS HILOS\n");

    for ( i = 0; i < MAX_CORREDORES; i++)
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

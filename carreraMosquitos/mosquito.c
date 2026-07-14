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
    char szBuffer[LARGO];


    mensaje msg;
    tmosquito *datos_thread; /*el tequipo va a varir depende del la consigna*/
    tmosquito *memoria = NULL;

    memoria = (tmosquito*)creo_memoria(sizeof(tmosquito)*MAX_MOSQUITOS, &id_memoria);

    pthread_t idHilo[MAX_MOSQUITOS];/*max_equipo va a variar en nombre y cantidad depende de la consigna*/
    pthread_attr_t atributos;

    /*THREADS*/
    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/
    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/

    datos_thread = (tmosquito*) malloc(sizeof(tmosquito) * MAX_MOSQUITOS);
    if (datos_thread == NULL){
        printf("Error: al crear jugador\n");
        exit(-1);
    }

    /*limpio vars y asigno espacio*/
    memset(&msg,0x00,sizeof(msg));
    memset(szBuffer,0x00,sizeof(szBuffer));
    memset(datos_thread, 0x00, sizeof(tequipo) * MAX_MOSQUITOS);/*max_equipo va a cambiar*/
    

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    id_cola_mensajes = creo_id_cola_mensajes();

    printf("PROCESO ... LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    /*logica*/
    recibir_mensaje(id_cola_mensajes,MSG_TABLERO,&msg);
    if (msg.int_evento == EVT_INICIO)
    {
        printf("MOSQUITO: Recibio inicio\n");
        enviar_mensaje(id_cola_mensajes,MSG_TABLERO,MSG_MOSQUITO,EVT_INICIO_ACK,"INICIO ACK");
        printf("MOSQUITO: Envio inicio ACK\n");
    }
    

    
    /*lanzo hilos*/

    for ( i = 0; i < MAX_MOSQUITOS; i++)/*max equipo va a cambiar*/
    {
        /* datos_thread[i]. todas los atributos del struct en el def.h */
        /*ejemplo para chars: strcpy(datos_thread[i].nombre_equipo, nom_equipo[i]);
        */
        /*como hago para leer la struct que tenia en memcompartida y ya le asigne atributos para ahora darselo a la sturct de datos thread*/
        
        datos_thread[i].id_cola_msg = id_cola_mensajes;
        datos_thread[i].num_mosquito = i;
        datos_thread[i].distancia_espiral = memoria[i].distancia_espiral;
        datos_thread[i].posicion_humo = memoria[i].posicion_humo;
        datos_thread[i].mov_humo = 0;
        datos_thread[i].mov_mosquito = 0;
        datos_thread[i].posicion_mosquito = 0;
        datos_thread[i].salud_mosquito = 1;
        if (pthread_create(&idHilo[i],&atributos,&funcionThread,&datos_thread[i])!= 0)
        {
            printf("No puedo crear el hilo\n");
            exit(1);
        }
        printf("Hilo ... %d creado\n",i);
    }
    
    printf("--------------------------\n");
    printf("ESPERO A LOS HILOS\n");

    for ( i = 0; i < MAX_MOSQUITOS; i++)/*si es 1 solo hilo, que no creo no seria un for*/
    {
        pthread_join(idHilo[i],NULL);
    }
    printf("Hilos finalizados\n");
    /*for si tengo que mostrar un dato de todos los hilos usando datos_thread[i]
    for ( i = 0; i < MAX_EQUIPO; i++){
        printf("El equipo %d(%s): METIO %d goles, umbral %d\n",datos_thread[i].numero_equipo,datos_thread[i].nombre_equipo,datos_thread[i].goles_acertados,datos_thread[i].goles_umbral);
    }
    */
    free(datos_thread);
    return 0;
}

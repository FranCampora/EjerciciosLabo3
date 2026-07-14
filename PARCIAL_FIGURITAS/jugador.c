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
    
    int cantJugadores;
    int ganador;
    int max_figuritas;


    mensaje msg;
    tjugador *datos_thread;
    tjugador *memoria;
    
    
    pthread_t idHilo[HASTA_JUGADORES];
    pthread_attr_t atributos;

    /*THREADS*/
    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/
    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/

    /*id_hilo = (pthread_t* ) malloc(sizeof(pthread_t)*CANTIDAD_HILOS);*/
    

    /*limpio vars y asigno espacio*/
    memset(&msg,0x00,sizeof(msg));
    memset(szBuffer,0x00,sizeof(szBuffer));
    memoria = NULL;
    id_memoria = 0;
    cantJugadores = 0;
    ganador = 0;
    max_figuritas = 0;

    /*la cantidad de jugadores se pone como parametro al ejecutar el proceso*/
    if(argc != 2)
	{
		printf("Ingrese la cantidad de jugadores:\n");
		exit(1);
	}
    cantJugadores = atoi(argv[1]);
    if (cantJugadores < DESDE_JUGADORES || cantJugadores > HASTA_JUGADORES)
    {
        printf("La cantidad de jugadores debe ser un número positivo, entre %d y %d jugadores\n",DESDE_JUGADORES,HASTA_JUGADORES);
        exit(1);
    }

    datos_thread = (tjugador*) malloc(sizeof(tjugador) * cantJugadores);
    if (datos_thread == NULL){
        printf("Error: al crear jugador\n");
        exit(-1);
    }
    memset(datos_thread, 0x00, sizeof(tjugador) * cantJugadores);
    
    /*comparto cant jugadores por memoria compartida*/
    memoria = (tjugador*) creo_memoria(sizeof(tjugador) * cantJugadores, &id_memoria);
    memoria[0].cantJugadores = cantJugadores;
    printf("La cantidad de jugadores es %d, esta escrito en memoria compartidas",cantJugadores);

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    id_cola_mensajes = creo_id_cola_mensajes();

    
    printf("PROCESO ... LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    /*lanzo hilos*/

    for ( i = 0; i < cantJugadores; i++)/*max equipo va a cambiar*/
    {
        /* datos_thread[i]. todas los atributos del struct en el def.h */
        /*ejemplo para chars: strcpy(datos_thread[i].nombre_equipo, nom_equipo[i]);
        */
        datos_thread[i].id_cola_msg = id_cola_mensajes;
        datos_thread[i].numero_jugador = i+1;
        datos_thread[i].saldo_jugador = memoria[0].saldo_jugador;
        
        
        memset(datos_thread[i].figuritas_jugador,0x00,sizeof(datos_thread[i].figuritas_jugador));
        

        if (pthread_create(&idHilo[i],&atributos,&funcionThread,&datos_thread[i])!= 0)
        {
            printf("No puedo crear el hilo\n");
            exit(1);
        }
        printf("Hilo ... %d creado\n",i);
    }
    
    printf("--------------------------\n");
    printf("ESPERO A LOS HILOS\n");

    for ( i = 0; i < cantJugadores; i++)/*si es 1 solo hilo, que no creo no seria un for*/
    {
        pthread_join(idHilo[i],NULL);
    }
    printf("Hilos finalizados\n");
    /*for si tengo que mostrar un dato de todos los hilos usando datos_thread[i]
    for ( i = 0; i < MAX_EQUIPO; i++){
        printf("El equipo %d(%s): METIO %d goles, umbral %d\n",datos_thread[i].numero_equipo,datos_thread[i].nombre_equipo,datos_thread[i].goles_acertados,datos_thread[i].goles_umbral);
    }
    */

    /*shmdt((char *)memoria);*/
    free(datos_thread);
    
    return 0;
}

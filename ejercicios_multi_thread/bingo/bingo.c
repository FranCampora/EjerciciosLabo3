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
    int id_cola_mensajes;
    int i;
    int cantJugadores;
    int idHilo;
    int jugadorGanador;
    char szBuffer[LARGO];
    tjugador *jugador;
    tbolillero datos_bolilleros;
    
    mensaje msg;

    pthread_t idHiloBolillero;
    pthread_attr_t atributos;

    srand(time(NULL));
    id_cola_mensajes = creo_id_cola_mensajes();

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
    memset(&datos_bolillero, 0x00, sizeof(datos_bolillero));
    memset(&msg,     0x00, sizeof(msg));
    memset(szBuffer, 0x00, sizeof(szBuffer));
    /*
    jugador = (tjugador *)malloc(sizeof(tjugador)*cantJugadores);
    idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantJugadores);*/
	pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/
    PTHREAD_CREATE_JOINABLE;
    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/

    if (jugador == NULL)
    {
        perror("No puedo crear jugador\n");
        exit(-1);
    }
    
    for (i = 0; i < cantJugadores; i++)
    {
        jugador[i].numeroJugador = i+1;
        jugador[i].carton
    }
    crearArchivoSincronismo(FILE_SINCRONISMO,1);
    borrar_cola_de_mensajes(id_cola_mensajes);

    jugadorGanador = 0;






    printf("PROCESO jugador LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    /*mensaje con la bolilla a los jugadores*/
    for ( i = 1; i <= cantJugadores; i++)
    {
        memset(szBuffer, 0x00, sizeof(szBuffer));
        sprintf(szBuffer, "%d", i);

        enviar_mensaje(id_cola_mensajes,MSG_JUGADOR + i,MSG_BINGO,EVT_INICIO,szBuffer);
        printf("BINGO:ENVIE EVENTO INICIO")
        
    }

    /*recibir ack de todos los jugadores*/
    for (i = 1; i <= cantJugadores; i++)
    {
        memset(&msg, 0x00, sizeof(msg));
        recibir_mensaje(id_cola_mensajes,MSG_BINGO,&msg);

        if (msg.int_evento == EVT_INICIO_ACK)
        {
            printf("Recibi ACK INIVIO DE JUGADOR\n");
        }
    }
    printf("Todos los jugadores tienen su thread creado");


    datos_bolilleros.cantJugadores = cantJugadores;
    datos_bolilleros.id_colamsg = id_cola_mensajes;
    

    /*crear los hilos del bolillero*/
    if (pthread_create(&idHiloBolillero,&atributos,funcionThreadBolillero,&datos_bolilleros)!= 0)
    {
        perror("No puedo crear el hilo\n");
        exit(-1);
    }
    printf("HILO BOLIIERO CREADO\n");
    
    /*espero recibir carton llleno*/
    memset(&msg, 0x00, sizeof(msg));
    recibir_mensaje(id_cola_mensajes, PROCESO_BOLERO, &msg);
    if (msg.int_evento == EVT_CARTON_LLENO)
    {
        jugadorGanador = atoi(msg.char_mensaje);
        printf("BINGO: Carton lleno,jugador %d\n", jugadorGanador);
    }

    /* Detener el bolillero y esperar que termine */
    datos_bolillero.activo = 0;
    pthread_join(idHiloBolillero, NULL);

    r (i = 1; i <= cantJugadores; i++)
    {
        memset(szBuffer, 0x00, sizeof(szBuffer));
        sprintf(szBuffer, "%d", jugadorGanador);
        enviar_mensaje(id_cola_mensajes,
                    MSG_JUGADOR + i,
                    MSG_BINGO,
                    EVT_CARTON_LLENO,
                    szBuffer);
    }
    borrar_cola_de_mensajes(id_cola_mensajes);
    return 0;
}

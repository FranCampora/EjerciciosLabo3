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

void *funcionThreadJugador(void *parametro){
/*
 * Thread de cada jugador.
 * Recibe parametro: puntero a tjugador con sus datos.
 * Logica:
 *   1. Genera carton (5 numeros aleatorios unicos del 1 al 99)
 *   2. Espera EVT_INICIO del proceso BINGO
 *   3. Envia EVT_INICIO_ACK al proceso BINGO
 *   4. Bucle: espera EVT_BOLILLA o EVT_CARTON_LLENO
 *      - EVT_BOLILLA: verifica si el numero esta en el carton,
 *                     si llega a 5 aciertos avisa a todos
 *      - EVT_CARTON_LLENO: finaliza el hilo devolviendo aciertos
 */


    int nro_jugador;
    int done;
    int i;
    int j;
    int carton;
    int *puntos_ret;
    int id_cola_mensajes;
    int numeroGenerado;
    int cantidadAciertos;
    int cantJugadores;
    int sizeCarton;
    char szBuffer[LARGO];
    int numeroRecibido;
    int ganadorEnviado;

    mensaje msg; 
    
    tjugador *datos_thread;
    datos_thread = (tjugador*)parametro;
    nro_jugador = datos_thread->numeroJugador;
    /*carton = datos_thread->carton;*/
    id_cola_mensajes = datos_thread->id_colamsg;
    cantJugadores = datos_thread->cantJugadores;

    done = 0;
    cantidadAciertos = 0;
    sizeCarton = 0;
    ganadorEnviado   = 0; 
    numeroRecibido   = 0;

    memset(datos_thread->carton,0x00,sizeof(datos_thread->carton));
    memset(&msg,0x00,sizeof(msg));
    memset(szBuffer,  0x00, sizeof(szBuffer));
    puntos_ret = (int *) malloc(sizeof(int));
    

    /*generar los numeros del carton para cada jugador*/
    pthread_mutex_lock(&mutex);

    for ( i = 0; i < 5; i++)
    {
        devolverAleatorioSinRepetir(DESDE, HASTA, datos_thread->carton, &sizeCarton);
    }
    pthread_mutex_unlock(&mutex);

    printf("HILO JUGADOR %d: Carton generado: ", nro_jugador);
    for (i = 0; i < 5; i++)
    {
        printf("[%d] ", datos_thread->carton[i]);
    }
    printf("\n");

    /*numero de cada carton ya listo, ahora arranca la logica*/
    printf("HILO JUGADOR %d iniciado. Esperando EVENTO INICIO\n",nro_jugador);
    
    pthread_mutex_lock(&mutex);
    recibir_mensaje(id_cola_mensajes, PROCESO_JUGADOR + nro_jugador, &msg);
    pthread_mutex_unlock(&mutex);
    
    if (msg.int_evento == EVT_INICIO)
    {
        printf("HILO JUGADOR %d: Recibi EVT_INICIO. Enviando ACK al BINGO\n", nro_jugador);
        memset(szBuffer, 0x00, sizeof(szBuffer));
        sprintf(szBuffer, "%d", nro_jugador);
        enviar_mensaje(id_cola_mensajes,PROCESO_BOLERO,PROCESO_JUGADOR + nro_jugador,EVT_INICIO_ACK,szBuffer);
    }
    while (done == 0)
    {
        pthread_mutex_lock(&mutex);/*lock del mutex, hago logica*/
        memset(&msg, 0x00, sizeof(msg));
        
        recibir_mensaje(id_cola_mensajes, PROCESO_JUGADOR + nro_jugador, &msg);
        pthread_mutex_unlock(&mutex);
        
        switch (msg.int_evento)
        {
            case EVT_BOLILLA:
                numeroRecibido = atoi(msg.char_mensaje);
                printf("HILO JUGADOR %d: Recibi bolilla %d\n", nro_jugador, numeroRecibido);

                /* Solo procesar si todavia no gano este jugador */
                if (ganadorEnviado == 0)
                {
                    for (j = 0; j < 5; j++)
                    {
                        if (datos_thread->carton[j] == numeroRecibido)
                        {
                            cantidadAciertos++;
                            printf("HILO JUGADOR %d: ACIERTO con numero %d! Aciertos: %d/5\n",
                                nro_jugador, numeroRecibido, cantidadAciertos);
                        }
                    }

                    /* Si completo el carton, avisar a todos */
                    if (cantidadAciertos == 5)
                    {
                        printf("HILO JUGADOR %d: BINGO! Carton completo! Avisando a todos\n",
                            nro_jugador);
                        ganadorEnviado = 1;

                        /* Avisar al proceso BINGO */
                        memset(szBuffer, 0x00, sizeof(szBuffer));
                        sprintf(szBuffer, "%d", nro_jugador);
                        pthread_mutex_lock(&mutex);
                        enviar_mensaje(id_cola_mensajes,
                                    PROCESO_BINGO,
                                    PROCESO_JUGADOR + nro_jugador,
                                    EVT_CARTON_LLENO,
                                    szBuffer);
                        pthread_mutex_unlock(&mutex);

                        /* Avisar a todos los jugadores, incluyendo a si mismo */
                        for (j = 1; j <= cantJugadores; j++)
                        {
                            memset(szBuffer, 0x00, sizeof(szBuffer));
                            sprintf(szBuffer, "%d", nro_jugador);
                            pthread_mutex_lock(&mutex);
                            enviar_mensaje(id_cola_mensajes,
                                        PROCESO_JUGADOR + j,
                                        PROCESO_JUGADOR + nro_jugador,
                                        EVT_CARTON_LLENO,
                                        szBuffer);
                            pthread_mutex_unlock(&mutex);
                        }
                    }
                }
                break;

        case EVT_CARTON_LLENO:
                printf("HILO JUGADOR %d: Recibi EVT_CARTON_LLENO. Jugador ganador: %s. Mis aciertos: %d\n",
                    nro_jugador, msg.char_mensaje, cantidadAciertos);
                done = 1;
                break;
        default:
                printf("HILO JUGADOR %d: Evento desconocido %d\n",
                    nro_jugador, msg.int_evento);
                break;
        }
        
    }
    

    *puntos_ret = cantidadAciertos;

    printf("HILO JUGADOR %d: Terminando. Aciertos finales: %d\n",
        nro_jugador, cantidadAciertos);

    pthread_exit((void *)puntos_ret);
}

void *funcionThreadBolillero(void *parametro)
{
    int i;
    int numeroGenerado;
    int vecGenerados[HASTA];/*para chekear los numeros q ya salieron del bolillero*/
    int sizeVec;
    int tiempoEspera;
    int id_cola_mensajes;
    int cantJugadores;
    char szBuffer[LARGO];
    tbolillero *datos;

    datos            = (tbolillero *) parametro;
    id_cola_mensajes = datos->id_colamsg;
    cantJugadores    = datos->cantJugadores;
    sizeVec          = 0;
    tiempoEspera     = 0;
    numeroGenerado   = 0;

    memset(vecGenerados,0x00,sizeof(vecGenerados));
    memset(szBuffer,0x00,sizeof(szBuffer));

    printf("HILO BOLILLERO: Iniciado. Comenzando a generar bolillas...\n");

    /* Generar todos los numeros del 1 al 99 en orden aleatorio */
    while (sizeVec < (HASTA - DESDE + 1) && datos->activo == 1)
    {
        /* Proteger rand() con mutex */
        pthread_mutex_lock(&mutex);
        numeroGenerado = devolverAleatorioSinRepetir(DESDE, HASTA, vecGenerados, &sizeVec);
        tiempoEspera   = devolverNumAleatorio(500, 5000);
        pthread_mutex_unlock(&mutex);

        memset(szBuffer, 0x00, sizeof(szBuffer));
        sprintf(szBuffer, "%d", numeroGenerado);

        printf("HILO BOLILLERO: Bolilla %d (numero %d de %d)\n",numeroGenerado, sizeVec, HASTA - DESDE + 1);

        /* Enviar la bolilla a cada jugador */
        for (i = 1; i <= cantJugadores; i++)
        {
            pthread_mutex_lock(&mutex);
            enviar_mensaje(id_cola_mensajes,
                        MSG_JUGADOR + i,
                        MSG_BINGO,
                        EVT_BOLILLA,
                        szBuffer);
            pthread_mutex_unlock(&mutex);
        }

        /* Esperar entre 500 y 5000 ms antes de la proxima bolilla */
        usleep(tiempoEspera * 1000);
    }

    printf("HILO BOLILLERO: Terminado\n");
    pthread_exit(NULL);
}
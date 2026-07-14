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

    
    int done;
    int id_cola_mensajes;
    char szBuffer[LARGO];

    int id_cola_msg;
    int num_mosquito;
    int distancia_espiral;
    int posicion_humo;
    int posicion_mosquito;
    int mov_mosquito;
    int mov_humo;
    int salud_mosquito;
    
    


    mensaje msg; 
    tmosquito *datos_thread;

    datos_thread = (tmosquito*) parametro;

    /*srand(time(NULL));*/

    
    id_cola_mensajes = datos_thread->id_cola_msg;
    num_mosquito = datos_thread->num_mosquito;
    distancia_espiral = datos_thread->distancia_espiral;
    posicion_humo = datos_thread->posicion_humo;
    posicion_mosquito = datos_thread->posicion_mosquito;
    mov_mosquito = datos_thread->mov_mosquito;
    mov_humo = datos_thread->mov_humo;
    salud_mosquito = datos_thread->salud_mosquito;

    
    done = 0;
    
    recibir_mensaje(id_cola_mensajes,MSG_EQUIPO+num_equipo,&msg);
    
    pthread_mutex_lock(&mutex);
    if (msg.int_evento == EVT_INICIO)
    {
        printf("HILO MOSQUITO %d iniciado. \n",num_equipo);

    }
    pthread_mutex_unlock(&mutex);
    usleep(1000*1000);

    while (done == 0 && goles_acertados < goles_umbral) 
    {
        pthread_mutex_lock(&mutex);
        printf("El equipo %d (%s), goles:%d de %d\n",num_equipo,nombre_equipo,goles_acertados,goles_umbral);
        /*ingreso num por pantalla*/
        numero_pantalla = cargarInt("Ingrese el numero de TIRO POR PANTALLA:");
        while (numero_pantalla < 3 || numero_pantalla > 6)/*el num de pantalla tiene que ser entre 3 y 6*/
        {
            printf("Numero de pantalla invalido. Ingrese un numero entre 3 y 6:\n");
            numero_pantalla = cargarInt("Ingrese el numero de TIRO POR PANTALLA:");
        }
        
        /*envio el numero ingreado x pantalla a la cancha.c*/
        memset(szBuffer, 0x00, sizeof(szBuffer));
        sprintf(szBuffer, "%d|%d", num_equipo, numero_pantalla);
        
        enviar_mensaje(id_cola_mensajes, MSG_CANCHA, MSG_EQUIPO + num_equipo, EVT_TIRO, szBuffer);
        
        printf("El equipo %d tiro y espera resultado\n", num_equipo);
        pthread_mutex_unlock(&mutex);
        usleep(1000*1000);

        /*recibe resultado de la cancha*/
        recibir_mensaje(id_cola_mensajes, MSG_EQUIPO + num_equipo, &msg);
        
        pthread_mutex_lock(&mutex);
        
        switch (msg.int_evento)
        {
            case EVT_GOL:
                goles_acertados++;
                printf("El equipo %d metio un GOL\n", num_equipo);
                
                break;
            case EVT_TIRO_LIBRE:
                goles_acertados++;
                printf("El equipo %d hizo un GOL tiro libre\n", num_equipo);
                
                break;
            case EVT_PALO:
                printf("El equipo %d LE PEGO AL PALO\n", num_equipo);
                break;
            case EVT_FUERA:
                printf("El equipo %d LE PEGO AFUERA\n", num_equipo);
                break;
            case EVT_LATERAL:
                printf("El equipo %d HIZO UN LATERAL\n", num_equipo);
                break;
            case EVT_FIN:
                printf("El equipo %d RECIBIO FIN CON %d goles. Fin del partido.\n", num_equipo,goles_acertados);
                done = 1;
                break;
            default:
                printf("ERROR:paso algo y no tomo ningun evento\n");
                break;
        }
        
        pthread_mutex_unlock(&mutex);
        usleep(1000*1000);
    }
    if (done == 0)
    {
        printf("El equipo %d alcanzo el umbral de %d goles. Avisando a cancha.\n",num_equipo, goles_umbral);
        memset(szBuffer, 0x00, sizeof(szBuffer));
        sprintf(szBuffer, "%d|%d", num_equipo, goles_acertados);
        enviar_mensaje(id_cola_mensajes, MSG_CANCHA, MSG_EQUIPO + num_equipo, EVT_FIN, szBuffer);
    }
    
    datos_thread->goles_acertados = goles_acertados;
    
    
    printf("Hijo equipo %d: termino con %d goles\n",num_equipo,goles_acertados);
    pthread_exit(NULL);
}

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
    
    int num_equipo;
    char nombre_equipo[LARGO];
    int numero_pantalla;
    int num_random;
    int goles_umbral;
    int goles_acertados;
    int id_cola_mensajes;
    char szBuffer[LARGO];


    mensaje msg; 
    tequipo *datos_thread;

    datos_thread = (tequipo*) parametro;

    /*srand(time(NULL));*/

    strcpy(nombre_equipo, datos_thread->nombre_equipo);
    num_equipo = datos_thread->numero_equipo;
    numero_pantalla = datos_thread->num_pantalla;
    num_random = datos_thread->num_random;
    goles_umbral = datos_thread->goles_umbral;
    id_cola_mensajes = datos_thread->id_cola_msg;

    
    done = 0;
    goles_acertados = 0;

    


    while (done == 0) 
    {
        recibir_mensaje(id_cola_mensajes,MSG_EQUIPO+num_equipo,&msg);
        pthread_mutex_lock(&mutex);
        switch (msg.int_evento)
        {
            case EVT_INICIO:
                printf("HILO EQUIPO %d (%s) iniciado\n",num_equipo,nombre_equipo);    
                
            case EVT_SEGUIR_PATEANDO:
                printf("Equipo %d no termino, sigue pateando\n",num_equipo);
                
                memset(szBuffer,0x00,sizeof(szBuffer));
                sscanf(msg.char_mensaje, "%d|%d", &num_equipo, &goles_acertados);
                printf("Equipo %d: sigue pateando (goles propios: %d / umbral: %d)\n",num_equipo, goles_acertados, goles_umbral);
                
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

                
                break;
            case EVT_FIN:
                memset(szBuffer,0x00,sizeof(szBuffer));
                sscanf(msg.char_mensaje, "%d|%d", &num_equipo, &goles_acertados);
                printf("Equipo %d (%s): recibio FIN con %d goles.\n",
                    num_equipo, nombre_equipo, goles_acertados);
                /* actualizo la estructura compartida con el main para el resumen */
                datos_thread->goles_acertados = goles_acertados;
                done = 1;
                break;
            default:
                break;
            
        } 
        pthread_mutex_unlock(&mutex);
        usleep(1000*1000);
        
    }
    
    
    printf("Hijo equipo %d: termino con %d goles\n",num_equipo,goles_acertados);
    pthread_exit(NULL);
    
}

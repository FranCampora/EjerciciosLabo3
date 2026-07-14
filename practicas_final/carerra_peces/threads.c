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

    
    int id_cola_mensajes;
    char szBuffer[LARGO];

    char nombre_maritimo_th[LARGO];
    int tipo_maritimo_th;
    int posicion_th;
    int estado_th;/*corriendo(1), muerto(2), meta(3)*/
    int numero_maritimo_th;
    int uso_poder_th;/*tierra 1 sola vez posr 500km, dinos 1 x vuelta, ovni no tiene. SI EL VALOR ES 1 LO USO Y SI ES 0 NO */
    int cant_vuelta_th;
    int distancia_meta_th;/*medido en km*/

    
    int numero_aleatorio_avanzar;
    int numero_aleatorio_aux;
    int evento_a_enviar;

    mensaje msg; 
    tmaritimo *datos_thread;

    datos_thread = (tmaritimo*) parametro;

    /*srand(time(NULL));*/

    id_cola_mensajes = datos_thread->id_cola_msg;
    strcpy(nombre_maritimo_th, datos_thread->nombre_maritimo);
    tipo_maritimo_th = datos_thread->tipo_maritimo;
    posicion_th = datos_thread->posicion;
    estado_th = datos_thread->estado;
    numero_maritimo_th = datos_thread->numero_maritimo;
    uso_poder_th = datos_thread->uso_poder;
    cant_vuelta_th = datos_thread->cant_vuelta;
    distancia_meta_th = datos_thread->distancia_meta;


    evento_a_enviar = EVT_NINGUNO;
    numero_aleatorio_avanzar = 0;
    numero_aleatorio_aux = 0;
    
    
    printf("Maritimo %d: esperando mensaje de inicio...\n", numero_maritimo_th);
    recibir_mensaje(id_cola_mensajes,MSG_MARITIMO+numero_maritimo_th,&msg);
    pthread_mutex_lock(&mutex);
    if (msg.int_evento == EVT_INICIO)
    {
        printf("Hilo maritimo numero %d: recibio INICIO\n",  numero_maritimo_th);
    }
    pthread_mutex_unlock(&mutex);
    usleep(1000*1000);

    while (estado_th == ESTADO_CORRIENDO) 
    {
        printf("El maritimo %d es del tipo %d\n",numero_maritimo_th, tipo_maritimo_th);

        
        memset(&msg,0x00,sizeof(msg));
        recibir_mensaje(id_cola_mensajes,MSG_MARITIMO+numero_maritimo_th,&msg);
        evento_a_enviar = EVT_NINGUNO; /* reseteo defensivo: evita reenviar el evento de la vuelta anterior si algun case nuevo se olvida de asignarlo */
        pthread_mutex_lock(&mutex);

        
        switch (msg.int_evento)/*aca chekeo los distintos eventos que puedo ir recibiendo cada maritimo osea thread*/
        {
            case EVT_TURNO:
                cant_vuelta_th++;
                switch (tipo_maritimo_th)
                {
                    case PEZ:

                        numero_aleatorio_avanzar = devolverNumAleatorio(AVANCE_PEZ_DESDE,AVANCE_PEZ_HASTA);
                        posicion_th = posicion_th + numero_aleatorio_avanzar;
                        
                        evento_a_enviar = EVT_AVANZA;
                        
                        break;
                    case PULPO:
                        numero_aleatorio_aux = devolverNumAleatorio(PULPO_PODER_DESDE,PULPO_PODER_HASTA);
                        printf("El pulpo %d tiro el dado de poder de ataque: %d\n", numero_maritimo_th, numero_aleatorio_aux);
                        if (numero_aleatorio_aux == NUM_PULPO_PODER)
                        {
                            printf("El pulpo %d puede matar a peces\n",numero_maritimo_th);
                            evento_a_enviar = EVT_ATAQUE_ENVIAR;
                            /*en el enviar mensaje pasa la data de posicion del pulpo y en base a eso que peces mueren*/
                        }else
                        {
                        
                            numero_aleatorio_avanzar = devolverNumAleatorio(AVANCE_PULPO_DESDE,AVANCE_PULPO_HASTA);
                            posicion_th = posicion_th + numero_aleatorio_avanzar;
                            evento_a_enviar = EVT_AVANZA;
                            /*el pulpo solo se mueve*/
                        }
                        
                        
                        break;
                    case TIBURON:
                        numero_aleatorio_aux = devolverNumAleatorio(TIBURON_PODER_DESDE,TIBURON_PODER_HASTA);
                        printf("El TIBURON %d tiro el dado de poder de ataque: %d\n", numero_maritimo_th, numero_aleatorio_aux);
                        if (numero_aleatorio_aux == NUM_TIBURON_PODER)
                        {
                            printf("El TIBURON %d puede matar a PECES\n",numero_maritimo_th);
                            evento_a_enviar = EVT_ATAQUE_ENVIAR;
                            /*en el enviar mensaje pasa la data de posicion del tiburon y en base a eso que peces mueren*/
                        }else
                        {
                        
                            numero_aleatorio_avanzar = devolverNumAleatorio(AVANCE_PULPO_DESDE,AVANCE_PULPO_HASTA);
                            posicion_th = posicion_th + numero_aleatorio_avanzar;
                            evento_a_enviar = EVT_AVANZA;
                            /*el tiburon solo se mueve*/
                        }
                        break;
                    default:
                        printf("No se pudo detectar el tipo del planeta\n");
                        break;
                    }
                if (posicion_th >= distancia_meta_th)
                {
                    estado_th = ESTADO_META;
                    evento_a_enviar = EVT_LLEGA_META;
                }
                    
                break;
            case EVT_ATAQUE_RECIBIR:
                
                estado_th = ESTADO_MUERTO;
                printf("El pez %d recibio un ataque y se murio\n",numero_maritimo_th);
                evento_a_enviar = EVT_MUERE;
                
                break;
            case EVT_FIN:
                printf("Maritimo %d, pista mando mensaje de FIN DE CARRERA\n",numero_maritimo_th);
                estado_th = ESTADO_MUERTO;
                evento_a_enviar = EVT_NINGUNO;
                /*Si un planeta gana, considero que los demas mueren por perder*/
                break;
            default:
                evento_a_enviar = EVT_NINGUNO;
                break;
            
            
        }
            if (evento_a_enviar != EVT_NINGUNO)
            {
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d|%d|%d|%d|%d", numero_maritimo_th, posicion_th, estado_th,uso_poder_th,tipo_maritimo_th);
                /*para los ataques la pista recibe y filtra: si uso el poder (1 ó 0) -> el maritimo (usando el tipo) q le mandó, el tipo
                me da la distancia de ataque. en base a eso leo todas las posiciones de los peces y los mato si entran en el rango de ataque */
                enviar_mensaje(id_cola_mensajes, MSG_PISTA, MSG_MARITIMO + numero_maritimo_th, evento_a_enviar, szBuffer);
                printf("Maritimo %d: reporte enviado a pista, posicion %d\n", numero_maritimo_th, posicion_th);
                
            }
        
        pthread_mutex_unlock(&mutex);
        usleep(1000*1000);


    
    }
    
    
    
    pthread_exit(NULL);
    
}

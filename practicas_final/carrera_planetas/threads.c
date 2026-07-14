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

    char nombre_planeta_th[LARGO];
    int tipo_planeta_th;
    int posicion_th;
    int estado_th;/*corriendo(1), muerto(2), meta(3)*/
    int numero_planeta_th;
    int uso_poder_th;/*tierra 1 sola vez posr 500km, dinos 1 x vuelta, ovni no tiene. SI EL VALOR ES 1 LO USO Y SI ES 0 NO */
    int cant_vuelta_th;
    int distancia_meta_th;/*medido en km*/

    
    int numero_aleatorio_avanzar;
    int numero_aleatorio_aux;
    int evento_a_enviar;

    mensaje msg; 
    tplaneta *datos_thread;

    datos_thread = (tplaneta*) parametro;

    /*srand(time(NULL));*/

    id_cola_mensajes = datos_thread->id_cola_msg;
    strcpy(nombre_planeta_th, datos_thread->nombre_planeta);
    tipo_planeta_th = datos_thread->tipo_planeta;
    posicion_th = datos_thread->posicion;
    estado_th = datos_thread->estado;
    numero_planeta_th = datos_thread->numero_planeta;
    uso_poder_th = datos_thread->uso_poder;
    cant_vuelta_th = datos_thread->cant_vuelta;
    distancia_meta_th = datos_thread->distancia_meta;


    evento_a_enviar = EVT_NINGUNO;
    numero_aleatorio_avanzar = 0;
    numero_aleatorio_aux = 0;
    
    
    printf("Planeta %d: esperando mensaje de inicio...\n", numero_planeta_th);
    recibir_mensaje(id_cola_mensajes,MSG_PLANETA+numero_planeta_th,&msg);
    pthread_mutex_lock(&mutex);
    if (msg.int_evento == EVT_INICIO)
    {
        printf("Hilo PLANETA numero %d: recibio INICIO\n",  numero_planeta_th);
    }
    pthread_mutex_unlock(&mutex);
    usleep(1000*1000);

    while (estado_th == ESTADO_CORRIENDO) 
    {
        printf("El planeta %d es del tipo %d\n",numero_planeta_th, tipo_planeta_th);

        
        memset(&msg,0x00,sizeof(msg));
        recibir_mensaje(id_cola_mensajes,MSG_PLANETA+numero_planeta_th,&msg);
        pthread_mutex_lock(&mutex);

        switch (msg.int_evento)/*aca chekeo los distintos eventos que puedo ir recibiendo cada planeta osea thread*/
        {
            case EVT_TURNO:
                cant_vuelta_th++;
                switch (tipo_planeta_th)
                {
                    case TIERRA:
                        if (posicion_th > CHEKEO_PROPU && uso_poder_th == NO_USO_PODER)
                        {
                            printf("El planeta %d puede usar el poder del propulsor\n",numero_planeta_th);
                            numero_aleatorio_aux = devolverNumAleatorio(PROPU_DESDE,PROPU_HASTA);
                            printf("El planeta %d tiro el dado del propulsor: %d\n", numero_planeta_th, numero_aleatorio_aux);
                            if (numero_aleatorio_aux != 1)
                            {
                                printf("El planeta %d activa propulsor\n",numero_planeta_th);
                                posicion_th = posicion_th + 100;
                                uso_poder_th = USO_PODER;
                            }else
                            {
                                printf("El planeta %d NO ACTIVO PROPULSOR\n",numero_planeta_th);

                            }
                        }else
                        {
                            numero_aleatorio_avanzar = devolverNumAleatorio(AVANCE_TIERRA_DESDE,AVANCE_TIERRA_HASTA);
                            posicion_th = posicion_th + numero_aleatorio_avanzar;
                            
                        }
                        evento_a_enviar = EVT_AVANZA;
                        
                        break;
                    case DINO:
                        numero_aleatorio_aux = devolverNumAleatorio(DINO_PODER_DESDE,DINO_PODER_HASTA);
                        printf("El planeta %d tiro el dado de poder de ataque: %d\n", numero_planeta_th, numero_aleatorio_aux);
                        if (numero_aleatorio_aux == NUM_DINO_PODER)
                        {
                            printf("El planeta %d puede matar a otro\n",numero_planeta_th);
                            evento_a_enviar = EVT_ATAQUE_ENVIAR;
                        }else
                        {
                        
                            numero_aleatorio_avanzar = devolverNumAleatorio(AVANCE_DINO_DESDE,AVANCE_DINO_HASTA);
                            posicion_th = posicion_th + numero_aleatorio_avanzar;
                            evento_a_enviar = EVT_AVANZA;
                            
                        }
                        
                        
                        break;
                    case OVNI:
                        numero_aleatorio_avanzar = devolverNumAleatorio(AVANCE_OVNI_DESDE,AVANCE_OVNI_HASTA);
                        posicion_th = posicion_th + numero_aleatorio_avanzar;
                        evento_a_enviar = EVT_AVANZA;
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
                if (tipo_planeta_th == DINO)
                {
                    printf("No puede recibir un ataque un planeta dino\n");
                }else
                {
                    estado_th = ESTADO_MUERTO;
                    printf("El planeta %d recibio un ataque y se destruyo\n",numero_planeta_th);
                    evento_a_enviar = EVT_MUERE;
                    
                }
                break;
            case EVT_FIN:
                printf("Planeta %d, pista mando mensaje de FIN DE CARRERA\n",numero_planeta_th);
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
                sprintf(szBuffer, "%d|%d|%d|%d|%d", numero_planeta_th, posicion_th, cant_vuelta_th, estado_th,uso_poder_th);
                enviar_mensaje(id_cola_mensajes, MSG_PISTA, MSG_PLANETA + numero_planeta_th, evento_a_enviar, szBuffer);
                printf("Planeta %d: reporte enviado a pista, posicion %d\n", numero_planeta_th, posicion_th);
                
            }
        
        pthread_mutex_unlock(&mutex);
        usleep(1000*1000);


    
    }
    
    
    
    pthread_exit(NULL);
    
}

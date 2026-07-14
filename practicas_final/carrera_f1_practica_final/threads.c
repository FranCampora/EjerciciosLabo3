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
    char nombre_pais_th[LARGO];
    char nombre_piloto_th[LARGO];
    int numero_auto_th;
    int escuderia_th;
    int posicion_th;
    int puntos_th;
    int estado_th;
    int vueltas_perdidas_th;

    
    int avance;
    int numero_aleatorio;
    int hito_ya_chequeado;
    int saltar_turno;
    int evento_a_enviar;

    
    int hitos_actual;
    mensaje msg; 
    tauto *datos_thread;

    datos_thread = (tauto*) parametro;

    /*srand(time(NULL));*/

    id_cola_mensajes = datos_thread->id_cola_msg;
    strcpy(nombre_pais_th, datos_thread->nombre_pais);
    strcpy(nombre_piloto_th, datos_thread->nombre_piloto);
    numero_auto_th = datos_thread->numero_auto;
    escuderia_th = datos_thread->escuderia;
    posicion_th = datos_thread->posicion;
    puntos_th = datos_thread->puntos;
    estado_th = ESTADO_CORRIENDO;/*todos comeinzas corriendo como base*/
    vueltas_perdidas_th = 0;
    hito_ya_chequeado = 0;
    hitos_actual = 0;
    saltar_turno = FALSE;
    evento_a_enviar = EVT_NINGUNO;
    
    
    
    done = 0;
    numero_aleatorio = 0;
    
    
    printf("Auto %d: esperando mensaje de inicio...\n", numero_auto_th);
    recibir_mensaje(id_cola_mensajes,MSG_AUTO+numero_auto_th,&msg);
    pthread_mutex_lock(&mutex);
    if (msg.int_evento == EVT_INICIO)
    {
        printf("Hilo auto numero %d: recibio INICIO\n",  numero_auto_th);
    }
    pthread_mutex_unlock(&mutex);
    usleep(1000*1000);

    while (estado_th == ESTADO_CORRIENDO) 
    {
        usleep(500*1000);
        memset(&msg,0x00,sizeof(msg));
        recibir_mensaje(id_cola_mensajes,MSG_AUTO+numero_auto_th,&msg);
        pthread_mutex_lock(&mutex);

        switch (msg.int_evento)/*aca chekeo los distintos eventos que puedo ir recibiendo cada auto osea thread*/
        {
            case EVT_TURNO:
                if (saltar_turno == TRUE)
                {
                    printf("El auto %d: va a boxes, pierde turno\n",numero_auto_th);
                    saltar_turno = FALSE;
                    evento_a_enviar = EVT_AVANZA;
                }
                else
                {
                    if (escuderia_th == FERRARI || escuderia_th == MCLAREN)
                    {
                        avance = devolverNumAleatorio(AVANCE_RAPIDO_DESDE,AVANCE_RAPIDO_HASTA);
                    }else
                    {
                        avance = devolverNumAleatorio(AVANCE_NORMAL_DESDE,AVANCE_NORMAL_HASTA);
                    }
                    posicion_th = posicion_th + avance;
                    numero_aleatorio = devolverNumAleatorio(1,2);/*chekea choque o salida d epista*/
                    
                    if (numero_aleatorio == 2)
                    {
                        estado_th = ESTADO_FUERA;
                        evento_a_enviar = EVT_SALE_PISTA;
                    }else
                    {
                        hitos_actual = 0;
                        switch (hito_ya_chequeado)
                        {
                            case 0:
                                if (posicion_th >= CHEKEO_1)
                                {
                                    hitos_actual = 1;
                                }
                                break;
                            case 1:
                                if (posicion_th >= CHEKEO_2)
                                {
                                    hitos_actual = 2;
                                }
                                break;
                            case 2:
                                if (posicion_th >= CHEKEO_3)
                                {
                                    hitos_actual = 3;
                                }
                                break;
                            case 3:
                                if (posicion_th >= CHEKEO_4)
                                {
                                    hitos_actual = 4;
                                }
                                break;
                            case 4:
                                if (posicion_th >= CHEKEO_5)
                                {
                                    hitos_actual = 5;
                                }
                                break;
                        
                        default:
                            break;
                        }
                        evento_a_enviar = EVT_AVANZA;

                        if (hitos_actual != 0)
                        {
                            hito_ya_chequeado = hitos_actual;
                            numero_aleatorio = devolverNumAleatorio(BOXES_DESDE,BOXES_HASTA);

                            if (numero_aleatorio == BOXES_VA_BOXES)
                            {
                                vueltas_perdidas_th++;
                                saltar_turno = TRUE;
                                evento_a_enviar = EVT_BOXES;
                            }
                            
                        }
                        if (posicion_th >= DISTANCIA_FIN)
                        {
                            estado_th = ESTADO_META;
                            evento_a_enviar = EVT_LLEGA_META;
                        }
                        
                    }
                    
                    
                }
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d|%d|%d|%d", numero_auto_th, posicion_th, vueltas_perdidas_th, puntos_th);
                enviar_mensaje(id_cola_mensajes, MSG_PISTA, MSG_AUTO + numero_auto_th, evento_a_enviar, szBuffer);
                printf("Auto %d: reporte enviado a pista, posicion %d\n", numero_auto_th, posicion_th);
                
                break;
            case EVT_FIN:
                printf("Auto %d, pista mando mensaje de FIN DE CARRERA\n",numero_auto_th);
                estado_th = ESTADO_FUERA;
                break;
            default:
                break;
        }
        pthread_mutex_unlock(&mutex);
        usleep(1000*1000);


    
    }
    
    
    
    pthread_exit(NULL);
    
}

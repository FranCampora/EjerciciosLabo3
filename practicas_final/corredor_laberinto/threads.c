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

    char nombre_corredor_th[LARGO];
    int tipo_corredor_th;
    int posicion_th;
    int estado_th;/*corriendo(0), meta(1), muerto(2)*/
    int msg_destino_th;

    int numero_aleatorio_avanzar;
    int ya_salto;

    int golpe_muchacho;
    int golpe_criatura;

    int salto_propio;
    int msg_rival_th;

    mensaje msg; 
    tcorredor *datos_thread;

    datos_thread = (tcorredor*) parametro;

    /*srand(time(NULL));*/

    id_cola_mensajes = datos_thread->id_cola_msg;
    strcpy(nombre_corredor_th, datos_thread->nombre_corredor);
    tipo_corredor_th = datos_thread->tipo_corredor;
    posicion_th = datos_thread->posicion;
    estado_th = datos_thread->estado;
    msg_destino_th = datos_thread->msg_destino;
    msg_rival_th = datos_thread->msg_rival;

    numero_aleatorio_avanzar = 0;
    ya_salto = FALSE;
    golpe_muchacho = 0;
    golpe_criatura = 0;
    salto_propio = 0;
    
    
    printf("%s: esperando mensaje de inicio...\n", nombre_corredor_th);
    recibir_mensaje(id_cola_mensajes, msg_destino_th, &msg);
    pthread_mutex_lock(&mutex);
    if (msg.int_evento == EVT_INICIO)
    {
        printf("Hilo %s: recibio INICIO\n", nombre_corredor_th);
    }
    pthread_mutex_unlock(&mutex);
    usleep(1000 * 1000);

    while (estado_th == ESTADO_CORRIENDO) 
    {
        
        memset(&msg,0x00,sizeof(msg));
        recibir_mensaje(id_cola_mensajes, msg_destino_th, &msg);
        pthread_mutex_lock(&mutex);

        if (estado_th == ESTADO_CORRIENDO)
        {
            switch (msg.int_evento)
            {
                case EVT_TURNO:
                    switch (tipo_corredor_th)
                    {
                        case TIPO_THOMAS:
                            numero_aleatorio_avanzar = devolverNumAleatorio(AVANCE_THOMAS_DESDE, AVANCE_THOMAS_HASTA);
                            posicion_th = posicion_th + numero_aleatorio_avanzar;
                            printf("%s avanza a la posicion %d\n", nombre_corredor_th, posicion_th);

                            if (posicion_th >= CHEKEO_METRAJE1 && ya_salto == FALSE)
                            {
                                ya_salto = TRUE;
                                memset(szBuffer, 0x00, sizeof(szBuffer));
                                sprintf(szBuffer, "%d|%d", msg_destino_th, tipo_corredor_th);
                                enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_SALTA, szBuffer);
                            }else if (posicion_th >= DISTANCIA_FIN)
                            {
                                estado_th = ESTADO_META;
                                printf("%s LLEGO A LA META!\n", nombre_corredor_th);
                                memset(szBuffer, 0x00, sizeof(szBuffer));
                                sprintf(szBuffer, "%d|%d", msg_destino_th, posicion_th);
                                enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_LLEGA_META, szBuffer);
                            }else
                            {
                                
                            /* pido arbitraje de combate contra mi criatura rival */
                                memset(szBuffer, 0x00, sizeof(szBuffer));
                                sprintf(szBuffer, "%d", msg_destino_th);
                                enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_GOLPE, szBuffer);
                            }
                            break;
                            
                        case TIPO_MUCHACHO:
                            numero_aleatorio_avanzar = devolverNumAleatorio(AVANCE_MUCHACHO_DESDE, AVANCE_MUCHACHO_HASTA);
                            posicion_th = posicion_th + numero_aleatorio_avanzar;
                            printf("%s avanza a la posicion %d\n", nombre_corredor_th, posicion_th);

                            if (posicion_th >= CHEKEO_METRAJE1 && ya_salto == FALSE)
                            {
                                ya_salto = TRUE;
                                memset(szBuffer, 0x00, sizeof(szBuffer));
                                sprintf(szBuffer, "%d|%d", msg_destino_th, tipo_corredor_th);
                                enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_SALTA, szBuffer);
                            }else if (posicion_th >= DISTANCIA_FIN)
                            {
                                estado_th = ESTADO_META;
                                printf("%s LLEGO A LA META!\n", nombre_corredor_th);
                                memset(szBuffer, 0x00, sizeof(szBuffer));
                                sprintf(szBuffer, "%d|%d", msg_destino_th, posicion_th);
                                enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_LLEGA_META, szBuffer);
                            }
                            else
                            {
                                /* pido arbitraje de combate contra mi criatura rival */
                                memset(szBuffer, 0x00, sizeof(szBuffer));
                                sprintf(szBuffer, "%d", msg_destino_th);
                                enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_GOLPE, szBuffer);
                            }
                            
                            break;
                        case TIPO_CRIATURA:
                            numero_aleatorio_avanzar = devolverNumAleatorio(AVANCE_CRIATURA_DESDE, AVANCE_CRIATURA_HASTA);
                            posicion_th = posicion_th + numero_aleatorio_avanzar;
                            printf("%s avanza a la posicion %d\n", nombre_corredor_th, posicion_th);

                            if (posicion_th >= CHEKEO_METRAJE1 && ya_salto == FALSE)
                            {
                                ya_salto = TRUE;
                                memset(szBuffer, 0x00, sizeof(szBuffer));
                                sprintf(szBuffer, "%d|%d", msg_destino_th, tipo_corredor_th);
                                enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_SALTA, szBuffer);
                            }else if (posicion_th >= DISTANCIA_FIN)
                            {
                                estado_th = ESTADO_META;
                                printf("%s llego a la meta y mata a su rival!\n", nombre_corredor_th);
                                memset(szBuffer, 0x00, sizeof(szBuffer));
                                sprintf(szBuffer, "%d", msg_destino_th);
                                enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_MATA_RIVAL, szBuffer);
                            }else
                            {
                                /* la criatura pide su proximo turno directo a pista, no combate */
                                enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_TURNO, "turno");
                            }
                            
                            break;
                        default:
                            printf("No se pudo detectar el tipo de corredor\n");
                            estado_th = ESTADO_MUERTO;
                            break;
                    }
                    break;
                
                case EVT_DEFENSA:
                    /* resultado del combate que pidio pista con EVT_GOLPE */
                    sscanf(msg.char_mensaje, "%d|%d", &golpe_muchacho, &golpe_criatura);
                    printf("%s combate: mi golpe %d, golpe criatura %d\n", nombre_corredor_th, golpe_muchacho, golpe_criatura);

                    if (golpe_muchacho == GOLPE_MORTAL && golpe_criatura == GOLPE_MORTAL)
                    {
                        estado_th = ESTADO_MUERTO;
                        printf("%s y su criatura mueren juntos en combate\n", nombre_corredor_th);
                        memset(szBuffer, 0x00, sizeof(szBuffer));
                        sprintf(szBuffer, "%d|%d", msg_destino_th, posicion_th);
                        enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_MUERE, szBuffer);
                    }else if (golpe_muchacho == golpe_criatura)
                    {
                        printf("%s devuelve el golpe y mata a su criatura\n", nombre_corredor_th);
                        /* le aviso DIRECTO a la criatura rival, si no su hilo queda esperando
                           EVT_TURNO para siempre y pthread_join no termina nunca */
                        enviar_mensaje(id_cola_mensajes, msg_rival_th, msg_destino_th, EVT_FIN, "combate");
                        enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_TURNO, "turno");
                    }else
                    {
                        /* no paso nada, pido el proximo turno */
                        enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_TURNO, "turno");
                    }
                    
                    
                    break;
                case EVT_SALTA:
                    /* resultado del salto del pozo que pidio pista con EVT_SALTA */
                    sscanf(msg.char_mensaje, "%d", &salto_propio);
                    printf("%s salta el pozo: tiro %d\n", nombre_corredor_th, salto_propio);

                    if ((tipo_corredor_th == TIPO_CRIATURA && salto_propio == SALTO_MUERTE) ||
                        (tipo_corredor_th == TIPO_THOMAS && salto_propio == SALTO_MUERTE) ||
                        (tipo_corredor_th == TIPO_MUCHACHO && salto_propio == SALTO_MUERTE))
                    {
                        estado_th = ESTADO_MUERTO;
                        printf("%s murio saltando el pozo\n", nombre_corredor_th);
                        memset(szBuffer, 0x00, sizeof(szBuffer));
                        sprintf(szBuffer, "%d|%d", msg_destino_th, posicion_th);
                        enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_MUERE, szBuffer);
                    }else
                    {
                         /* salto con exito, pido el proximo turno */
                        enviar_mensaje(id_cola_mensajes, MSG_PISTA, msg_destino_th, EVT_TURNO, "turno");
                    }
                    
                    break;
                case EVT_FIN:
                    printf("%s: recibio FIN, termina el hilo\n", nombre_corredor_th);
                    estado_th = ESTADO_MUERTO;
                    break;
                default:
                    break;
            }
        }
        

        pthread_mutex_unlock(&mutex);
        usleep(1000*1000);


    
    }
    
    
    printf("%s: termino en la posicion %d\n", nombre_corredor_th, posicion_th);
    pthread_exit(NULL);
    
}

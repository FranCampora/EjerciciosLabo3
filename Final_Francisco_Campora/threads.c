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

    char nombre_luchador_th[LARGO];
    int tipo_luchador_th;
    int estado_th;
    int numero_luchador_th;
    int vida_th;
    int arma_th;

    int numero_aleatorio_ataque;
    int evento_a_enviar;

    int numero_msg;
    int vida_msg;
    int estado_msg;
    int tipo_msg;
    int dato_msg;

    mensaje msg; 
    tluchador *datos_thread;

    datos_thread = (tluchador*) parametro;

    id_cola_mensajes = datos_thread->id_cola_msg;
    strcpy(nombre_luchador_th, datos_thread->nombre_luchador);
    tipo_luchador_th = datos_thread->tipo_luchador;
    estado_th = datos_thread->estado;
    numero_luchador_th = datos_thread->numero_luchador;
    vida_th = datos_thread->vida;
    arma_th = datos_thread->arma;
    

    evento_a_enviar = EVT_NINGUNO;
    numero_aleatorio_ataque = 0;
    numero_msg = 0;
    vida_msg = 0;
    estado_msg = 0;
    tipo_msg = 0;
    dato_msg = 0;
    
    
    printf("Luchador %d (%s): espera inicio\n", numero_luchador_th, nombre_luchador_th);
    recibir_mensaje(id_cola_mensajes, MSG_LUCHADOR + numero_luchador_th, &msg);
    pthread_mutex_lock(&mutex);
    if (msg.int_evento == EVT_INICIO)
    {
        printf("Hilo luchador numero %d: recibio INICIO\n", numero_luchador_th);
    }
    pthread_mutex_unlock(&mutex);
    usleep(1000 * 1000);

    while (estado_th == ESTADO_VIVO)
    {
        printf("El luchador %d es del tipo %d\n", numero_luchador_th, tipo_luchador_th);

        memset(&msg, 0x00, sizeof(msg));
        recibir_mensaje(id_cola_mensajes, MSG_LUCHADOR + numero_luchador_th, &msg);
        
        pthread_mutex_lock(&mutex);
        evento_a_enviar = EVT_NINGUNO;
        

        
        switch (msg.int_evento)
        {
            case EVT_TURNO:
                
                numero_aleatorio_ataque = devolverNumAleatorio(ATAQUE_VALOR_DESDE, ATAQUE_VALOR_HASTA);
                printf("Luchador %d %s con arma %d: ataca y hace %d de ataque\n", numero_luchador_th, nombre_luchador_th, arma_th, numero_aleatorio_ataque);
                evento_a_enviar = EVT_ATAQUE_ENVIAR;
                
                break;
            case EVT_ATAQUE_RECIBIR:
                sscanf(msg.char_mensaje, "%d|%d|%d|%d|%d", &numero_msg, &vida_msg, &estado_msg, &tipo_msg, &dato_msg);
                printf("Luchador %d (%s) recibe un ataque de %d \n", numero_luchador_th, nombre_luchador_th, dato_msg);
                vida_th = vida_th - dato_msg;

                /*chekeo si sobrevivo o no*/
                if (vida_th <= 0)
                {
                    vida_th = 0;
                    estado_th = ESTADO_MUERTO;
                    printf("Luchador %d (%s) murio\n", numero_luchador_th, nombre_luchador_th);
                    evento_a_enviar = EVT_MUERE;
                }
                else
                {
                    printf("Luchador %d (%s) sobrevivio al ataque Y le queda  de vida: %d\n", numero_luchador_th, nombre_luchador_th, vida_th);
                    evento_a_enviar = EVT_NINGUNO;
                }
                
                break;
            case EVT_FIN:
                sscanf(msg.char_mensaje, "%d|%d|%d|%d|%d", &numero_msg, &vida_msg, &estado_msg, &tipo_msg, &dato_msg);
                if (tipo_luchador_th == GLADIADOR && estado_th == ESTADO_VIVO)
                {
                    if (dato_msg == EMPERADOR_DECISION_VIVO_1 || dato_msg == EMPERADOR_DECISION_VIVO_2)
                    {
                        printf("Gladiador %d (%s): el emperador le concede la libertaad\n", numero_luchador_th, nombre_luchador_th);
                    }
                    else
                    {
                        printf("Gladiador %d (%s): el emperador decide que muera\n", numero_luchador_th, nombre_luchador_th);
                    }
                }
                else
                {
                    printf("Luchador %d (%s): coliseo aviso FIN DE COMBATE\n", numero_luchador_th, nombre_luchador_th);
                }
                estado_th = ESTADO_MUERTO;
                evento_a_enviar = EVT_NINGUNO;
                break;
            default:
                evento_a_enviar = EVT_NINGUNO;
                break;
            
            
        }
        if (evento_a_enviar != EVT_NINGUNO)
        {
            memset(szBuffer, 0x00, sizeof(szBuffer));
            sprintf(szBuffer, "%d|%d|%d|%d|%d", numero_luchador_th, vida_th, estado_th, tipo_luchador_th, numero_aleatorio_ataque);
            enviar_mensaje(id_cola_mensajes, MSG_COLISEO, MSG_LUCHADOR + numero_luchador_th, evento_a_enviar, szBuffer);
            printf("Luchador %d: envia al coliseo\n", numero_luchador_th);
        }
        
        pthread_mutex_unlock(&mutex);
        usleep(200*1000);


    
    }
    
    
    
    pthread_exit(NULL);
    
}

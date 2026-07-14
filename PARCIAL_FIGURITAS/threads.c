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

    int i;
    int done;
    int id_cola_mensajes;
    char szBuffer[LARGO];
    
    int num_jugador;
    int saldo_jugador;
    int numero_random_vuelta;
    int figurita_jugador[CANTIDAD_FIGURITAS];/*son 70 posiciones donde en la posicion 1 esta la figu 1, si la tiene es 1, si le sale otra vez es 2 y así*/
    
    
    int figuritas_pegadas;
    int album_listo;
    int saldo_nuevo;
    int figu1,figu2,figu3,figu4,figu5;

    mensaje msg; 
    tjugador *datos_thread;

    datos_thread = (tjugador*) parametro;

    /*srand(time(NULL));*/

    memset(szBuffer,0x00, sizeof(szBuffer));
	memset(&msg,0x00, sizeof(msg));
    memset(figurita_jugador,0x00, sizeof(figurita_jugador));
    done = 0;
    album_listo = 0;
    figuritas_pegadas = 0;
    numero_random_vuelta = 0;
    saldo_nuevo = 0;
    i = 0;
    figu1 = 0;
    figu2 = 0;
    figu3 = 0;
    figu4 = 0;
    figu5 = 0;


    id_cola_mensajes = datos_thread->id_cola_msg;
    num_jugador = datos_thread->numero_jugador;
    saldo_jugador = datos_thread->saldo_jugador;
    memcpy(figurita_jugador,datos_thread->figuritas_jugador,sizeof(int)*CANTIDAD_FIGURITAS);

    

    /*espera evento inicio de kioscccko*/
    recibir_mensaje(id_cola_mensajes,MSG_JUGADOR+num_jugador,&msg);
    pthread_mutex_lock(&mutex);
    if (msg.int_evento == EVT_INICIO)
    {
        printf("Hilo jugador %d: recibio INICIO.\n", num_jugador);
    }
    pthread_mutex_unlock(&mutex);
    usleep(1000*1000);

    /*es un falso comprar va a ser sin paquetes y le va devolver sin figuritas*/
    memset(szBuffer,0x00, sizeof(szBuffer));
    sprintf(szBuffer,"%d|%d|%d",num_jugador,0,saldo_jugador);
    enviar_mensaje(id_cola_mensajes,MSG_KIOSCKO,MSG_JUGADOR+num_jugador,EVT_COMPRO,szBuffer);
    printf("Jugador %d: primer EVT_COMPRO de arranque enviado al kioscko\n",num_jugador);
    
    while (done == 0) 
    {
        memset(&msg,0x00,sizeof(msg));/*limpio el mensaje x las dudas*/
        
        /* justo antes del recibir_mensaje del while */
        recibir_mensaje(id_cola_mensajes,MSG_JUGADOR+num_jugador,&msg);
        pthread_mutex_lock(&mutex);

        switch (msg.int_evento)
        {
            case EVT_SEGUIR_COMPRANDO:/*le da el nuevo saldo osea -50 y las figuritas*/
                /*calcular numero vuelta jugador*/
                saldo_nuevo = 0;
                memset(szBuffer,0x00,sizeof(szBuffer));
                sscanf(msg.char_mensaje, "%d|%d|%d|%d|%d|%d", &saldo_nuevo, &figu1,&figu2,&figu3,&figu4,&figu5);/*el 2do argumento son las nuevas figuritas*/
                
                /*actualizo saldo*/
                saldo_jugador = saldo_nuevo;

                /*pego las figus, le pongo 1 en la posicion de la figu*/
                if (figu1 > 0) { figurita_jugador[figu1]++; }
                if (figu2 > 0) { figurita_jugador[figu2]++; }
                if (figu3 > 0) { figurita_jugador[figu3]++;}
                if (figu4 > 0) { figurita_jugador[figu4]++;}
                if (figu5 > 0) { figurita_jugador[figu5]++;  }

                /*chekear si termine el album*/
                album_listo = 1;
                for ( i = FIGURITAS_NORMAL_DESDE; i <= FIGURITAS_NORMAL_HASTA; i++)
                {
                    if (figurita_jugador[i]==0)
                    {
                        album_listo = 0;
                        break;
                    }
                    
                }
                figuritas_pegadas = 0;
                if (album_listo == 1)
                {
                    
                    for ( i = 0; i < CANTIDAD_FIGURITAS; i++)
                    {
                        if (figurita_jugador[i]>0)
                        {
                            figuritas_pegadas++;
                            
                        }
                        
                    }
                    
                }
                /*cuento figuritas pra saber como viene el album*/

                for ( i = 0; i < CANTIDAD_FIGURITAS; i++)
                {
                    if (figurita_jugador[i]>0)
                    {
                        figuritas_pegadas++;
                    }
                    
                }
                printf("Jugador %d, figuritas distintas pegadas %d\n",num_jugador,figuritas_pegadas);

                if (album_listo == 1)
                {
                    printf("Jugador %d, album completo, saldo final %d\n",num_jugador,saldo_jugador);
                    done = 1;
                    break;
                }
                
                /*chekear saldo*/
                if (saldo_jugador < COSTO_PAQUETE)
                {
                    printf("Jugador %d, sin saldo, espera evt fin\n",num_jugador);
                    printf("--------------------------------\n");
                }else
                {
                    numero_random_vuelta = devolverNumAleatorio(DESDE_VUELTA,HASTA_VUELTA);
                    printf("Jugador %d tira vuelta%d\n",num_jugador,numero_random_vuelta);
                    printf("--------------------------------\n");
                    if (numero_random_vuelta == 1 || numero_random_vuelta == 2)
                    {
                        memset(szBuffer,0x00,sizeof(szBuffer));
                        sprintf(szBuffer,"%d|%d|%d",num_jugador,1,saldo_jugador);
                        enviar_mensaje(id_cola_mensajes,MSG_KIOSCKO,MSG_JUGADOR+num_jugador,EVT_COMPRO,szBuffer);

                        printf("Jugador %d, envio EVT COMPRO, salio el %d\n",num_jugador,numero_random_vuelta);
                        printf("--------------------------------\n");
                        if (numero_random_vuelta == 2)
                        {
                            memset(szBuffer,0x00,sizeof(szBuffer));
                            sprintf(szBuffer,"%d|%d|%d",num_jugador,1,saldo_jugador-COSTO_PAQUETE);
                            enviar_mensaje(id_cola_mensajes,MSG_KIOSCKO,MSG_JUGADOR+num_jugador,EVT_COMPRO,szBuffer);
                            printf("Jugador %d, envio 2do evento compro\n",num_jugador);
                            printf("--------------------------------\n");

                        }
                        
                    }else
                    {
                        memset(szBuffer,0x00,sizeof(szBuffer));
                        sprintf(szBuffer,"%d|0|0|0|0|0",saldo_jugador);
                        enviar_mensaje(id_cola_mensajes,MSG_JUGADOR+num_jugador,MSG_NADIE,EVT_SEGUIR_COMPRANDO,szBuffer);
                        printf("Jugador %d paso el turno\n",num_jugador);
                        printf("--------------------------------\n");

                    }
                    
                    
                }
                usleep(2000*1000);
                break;
                
            case EVT_FIN:
                printf("Jugador %d, recibio FIN del kioscko\n",num_jugador);
                done = 1;
                break;

            default:
                printf("ERROR EN JUGADOR al recibir evento de kioscko\n");
                break;
        }
        
        pthread_mutex_unlock(&mutex);
        usleep(1000*1000);   
    }
    figuritas_pegadas = 0;
    for ( i = 0; i < CANTIDAD_FIGURITAS; i++)
    {
        if (figurita_jugador[i] > 0)
        {
            figuritas_pegadas++;
        }
        
    }
    pthread_mutex_lock(&mutex);
    datos_thread->saldo_jugador = saldo_jugador;
    memcpy(datos_thread->figuritas_jugador,figurita_jugador,sizeof(int)*CANTIDAD_FIGURITAS);
    printf("Jugador %d termino, saldo:$%d\n",num_jugador,saldo_jugador);


    pthread_mutex_unlock(&mutex);
    usleep(1000*1000); 

    printf("Hijo jugador %d: termino \n",num_jugador);
    pthread_exit(NULL);
    
}

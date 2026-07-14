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

int main(int argc, char  *argv[])
{
    int i;
    int id_cola_mensajes;
    int id_memoria;
    char szBuffer[LARGO];
    int terminado;
    
    int numero_auto;
    int posicion_auto;
    int vueltas_perdidas_auto;
    int puntos_auto;
    int estado_autos[MAX_AUTOS + 1];
    int autos_en_carrera;

    tauto *memoria;
    mensaje msg;

    /*inicializo variables y limpio*/
    memoria = NULL;
    memset(&msg,     0x00, sizeof(msg));
    memset(szBuffer, 0x00, sizeof(szBuffer));
    numero_auto = 0;
    posicion_auto = 0;
    vueltas_perdidas_auto = 0;
    puntos_auto = 0;
    terminado = FALSE;
    

    id_cola_mensajes = creo_id_cola_mensajes();
    srand(time(NULL));
    
    memoria = (tauto*)creo_memoria(sizeof(tauto)*MAX_AUTOS, &id_memoria);
    memset(memoria, 0x00, sizeof(tauto) * MAX_AUTOS);
    
    crearArchivoSincronismo(FILE_SINCRONISMO,1);
    borrar_mensajes(id_cola_mensajes);

    
    printf("PROCESO PISTA LISTA PARA EMPEZAR. Enter para empezar...\n");
    getchar();



    autos_en_carrera = MAX_AUTOS;
    for ( i = 0; i < MAX_AUTOS; i++)
    {
        /*envio evento inicio a los hilos usuarios*/
        memoria[i].numero_auto = i +1;
        estado_autos[i + 1] = ESTADO_CORRIENDO;
        enviar_mensaje(id_cola_mensajes, MSG_AUTO + (i + 1), MSG_PISTA, EVT_INICIO, "inicio");
        /*Enviamos el primer turno para arrancar*/
        enviar_mensaje(id_cola_mensajes, MSG_AUTO + (i + 1), MSG_PISTA, EVT_TURNO, "0|0|0|0");
    }
    printf("Entidad: inicio enviado a los %d autos\n", MAX_AUTOS);

    while (terminado == FALSE)
    {
        /*loop de logica*/
        printf("Pista: esperando mensajes...\n");
        recibir_mensaje(id_cola_mensajes,MSG_PISTA,&msg);
        memset(szBuffer,0x00,sizeof(szBuffer));
        sscanf(msg.char_mensaje,"%d|%d|%d|%d",&numero_auto,&posicion_auto,&vueltas_perdidas_auto,&puntos_auto);
            

        switch (msg.int_evento)
        {
            case EVT_AVANZA:
                printf("Auto %d avanza a la posicion %d",numero_auto,posicion_auto);

                memset(szBuffer,0x00,sizeof(szBuffer));
                sprintf(szBuffer,"%d|%d|%d|%d",numero_auto,posicion_auto,vueltas_perdidas_auto,puntos_auto);

                enviar_mensaje(id_cola_mensajes,MSG_AUTO+numero_auto,MSG_PISTA,EVT_TURNO,szBuffer);
                break;
            case EVT_SALE_PISTA:
                printf("Auto %d ha salido de la pista en la posicion %d y queda fuera de carrera.\n", numero_auto, posicion_auto);
                estado_autos[numero_auto] = ESTADO_FUERA;
                autos_en_carrera--;
                if (autos_en_carrera == 0)
                {
                    printf("Todos los autos salieron de la pista. No hay ganador.\n");
                    terminado = TRUE;
                }
                break;
            case EVT_BOXES:
                printf("Auto %d entra a boxes\n", numero_auto);
                memset(szBuffer,0x00,sizeof(szBuffer));
                sprintf(szBuffer,"%d|%d|%d|%d",numero_auto,posicion_auto,vueltas_perdidas_auto,puntos_auto);

                /* Le devolvemos el turno para que siga avanzando*/
                enviar_mensaje(id_cola_mensajes, MSG_AUTO + numero_auto, MSG_PISTA,EVT_TURNO, szBuffer);
                break;
            case EVT_LLEGA_META:
                printf("GANADOR: Auto %d!\n", numero_auto);
                estado_autos[numero_auto] = ESTADO_META;
                terminado = TRUE;
                break;
            
            default:
                break;
        }

    }

    /* le aviso a los autos que sigan en ESTADO_CORRIENDO que la carrera termino */
    for ( i = 1; i <= MAX_AUTOS; i++)
    {
        if (estado_autos[i] == ESTADO_CORRIENDO)
        {
            memset(szBuffer, 0x00, sizeof(szBuffer));
            sprintf(szBuffer, "FIN");
            enviar_mensaje(id_cola_mensajes, MSG_AUTO + i, MSG_PISTA, EVT_FIN, szBuffer);
            printf("Aviso FIN al auto %d, que seguia en carrera\n", i);
        }
    }

    printf("MUESTRO DATOS FINALES\n");
    usleep(5000*1000);

    shmdt((char *)memoria);
    shmctl(id_memoria, IPC_RMID, (struct shmid_ds *)NULL);

    borrar_cola_de_mensajes(id_cola_mensajes);
        
    return 0;
}

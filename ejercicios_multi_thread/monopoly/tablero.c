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

int main(int argc, char *argv[])
{
    int i;
    
    int id_cola_mensajes;
    int pista[41];
    int cantJugadores;
    int nro_jugador;
    int posicion;
    int monto;
    int turnoActivo;
    char szBuffer[LARGO];

    mensaje msg;
    

    memset(&msg,     0x00, sizeof(msg));
    memset(szBuffer, 0x00, sizeof(szBuffer));
    posicion = 0;
    nro_jugador = 0;
    cantJugadores = 3;

    id_cola_mensajes = creo_id_cola_mensajes();

    crearArchivoSincronismo(FILE_SINCRONISMO,1);
    borrar_mensajes(id_cola_mensajes);
    
    for ( i = 0; i <= 40; i++)
    {
        pista[i] = i;
    }
    

    printf("PROCESO TABLERO LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    for ( i = 1; i <= cantJugadores; i++)
    {
        enviar_mensaje(id_cola_mensajes,MSG_JUGADOR1+i,MSG_PISTA,EVT_INICIO,"inicio");
        
    }
    
    while (1)
    {
        
        memset(&msg,0x00, sizeof(msg));
        recibir_mensaje(id_cola_mensajes,MSG_PISTA,&msg);

        if (msg.int_evento == EVT_CORRO)
        {
            memset(szBuffer,0x00,sizeof(szBuffer));
            sscanf(msg.char_mensaje,"%d|%d|%d|%d",&nro_jugador,&posicion,&monto,&turnoActivo);
            
            printf("El jugador numero %d, tiene esta en la posicion %d",nro_jugador,pista[posicion]);
            /**/
        
            switch (pista[posicion])
            {
            case 2:
                monto = monto - 50;
                break;
            case 3:
                monto = monto-80;
                break;
            case 6:
                monto = monto - 150;
                break;
            case 7:
                monto = monto + 160;
                break;
            case 9:
                monto = monto -50;

                break;
            case 12:
                turnoActivo = 0 ;
                break;
            case 13:
                monto = monto -450;
                break;
            case 15:
                monto = monto +35;
                break;
            case 17:
                turnoActivo = 0 ;
                break;
            case 20:
                monto = monto +800;
                break;
            case 22:
                monto = monto +115;
                break;
            case 23:
                monto = monto -390;
                break;
            case 25:
                turnoActivo = 0 ;
                break;
            case 27:
                monto =monto -150;
                break;
            case 29 :
                monto =monto +150;
                break;
            case 30:
                monto = monto -900;
                break;
            case 32:
                monto =monto +500;
                break;
            case 33:
                monto =monto -450;
                break;
            case 35:
                monto = monto -15;
                break;
            case 36:
                monto = monto +60;
                break;
            case 38:
                turnoActivo = 0 ;
                break;
            case 39:
                monto = monto +300;
                break;
            case 40:
                monto = monto +100;
                break;
            default:
                break;
            }

            memset(szBuffer, 0x00, sizeof(szBuffer));
            sprintf(szBuffer,"%d|%d|%d|%d",nro_jugador,monto,turnoActivo,posicion);
            enviar_mensaje(id_cola_mensajes,MSG_JUGADOR1+nro_jugador,MSG_PISTA,EVT_RTA_TABLERO,szBuffer);

        
        }
        else if (msg.int_evento == EVT_FIN)
        {
            printf("HUBO GANADOR\n");
            memset(szBuffer,0x00,sizeof(szBuffer));
            sscanf(msg.char_mensaje,"%d|%d",&nro_jugador,&i);
            if (pista[i] > 40)
            {
                printf("El jugador %d llego a la posicion %d con un monto de \n",nro_jugador,pista[posicion],monto);
            }
            break;
        }
        
        
    }
    

    borrar_cola_de_mensajes(id_cola_mensajes);
    return 0;
}



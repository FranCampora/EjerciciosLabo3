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
    char szBuffer[LARGO];
    int id_memoria;
    int terminado;
    int numero_jugador;
    int cantJugadores;
    int cant_paquetes;/*1 o 2*/
    int paquetes_globales;/*contador paquetes*/
    int saldo_jugador;
    int saldo_nuevo;
    int figus[FIGURITAS_SOBRE];/*las 5 del paquete*/
    
    int opcion_especial_normal;/*1 normal, 2 espcecial, por cada figurita*/
    
    tjugador *memoria;
    /*tequipo *memoria*/

    mensaje msg;

    /*inicializo variables y limpio*/
    memoria = NULL;
    memset(&msg,0x00,sizeof(msg));
    memset(&figus,0x00,sizeof(figus));
    memset(szBuffer,0x00,sizeof(szBuffer));
    cantJugadores = 0;
    paquetes_globales = 0;
    cant_paquetes = 0;
    terminado = FALSE;
    numero_jugador = 0;
    saldo_jugador = 0;
    saldo_nuevo = 0;
    opcion_especial_normal = 0;


    id_cola_mensajes = creo_id_cola_mensajes();

    srand(time(NULL));
    
    memoria = (tjugador*)creo_memoria(sizeof(tjugador)*4, &id_memoria);
    
    
    memoria[0].saldo_jugador = MONTO_INICIAL;
    paquetes_globales = CANTIDAD_PAQUETES;
    
    
    crearArchivoSincronismo(FILE_SINCRONISMO,1);
    borrar_mensajes(id_cola_mensajes);

    printf("PROCESO cancha LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    cantJugadores = memoria[0].cantJugadores;
    while (cantJugadores == 0)
    {
        printf("KIOSCKO:No me llegaron cant de jugadores\n");
        usleep(1000*1000);
        cantJugadores = memoria[0].cantJugadores;
    }
    
    paquetes_globales = CANTIDAD_PAQUETES;

    printf("Jugadores %d, paquetes%d\n",cantJugadores,paquetes_globales);
    /*enviar evt_inicio a los hilos*/
    for ( i = 0; i < cantJugadores; i++)
    {
        /*envio evento inicio a los hilos*/
        enviar_mensaje(id_cola_mensajes, MSG_JUGADOR + (i + 1), MSG_KIOSCKO, EVT_INICIO, "inicio");
    }
    printf("KIOSCKO: inicio enviado a los %d jugadores\n", cantJugadores);
    

    while (terminado == FALSE)
    {
        /*loop de logica*/
        memset(&msg,0x00,sizeof(msg));/*limpio el mensaje x las dudas*/
        memset(&figus,0x00,sizeof(figus));
        
        recibir_mensaje(id_cola_mensajes,MSG_KIOSCKO,&msg);
        if (msg.int_evento == EVT_COMPRO)
        {
            memset(szBuffer,0x00,sizeof(szBuffer));
            sscanf(msg.char_mensaje,"%d|%d|%d",&numero_jugador,&cant_paquetes,&saldo_jugador);
            
            printf("KIOSCKO: jugador %d quiere comprar %d paquetes y tiene de saldo %d\n",numero_jugador,cant_paquetes,saldo_jugador);
            if (cant_paquetes == 0)/*el primer compor entra aca*/
            {
                
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d|0|0|0|0|0", saldo_jugador);
                printf("KIOSCKO: jugador %d paso turno o arranque, respondo sin figus\n", numero_jugador);
                enviar_mensaje(id_cola_mensajes,MSG_JUGADOR+numero_jugador,MSG_KIOSCKO,EVT_SEGUIR_COMPRANDO,szBuffer);
            
            }
            
            else if (paquetes_globales <= 0)
            {
                printf("Kioscko: no quedan mas paquetes\n");
                enviar_mensaje(id_cola_mensajes, MSG_JUGADOR + numero_jugador, MSG_KIOSCKO, EVT_FIN, "fin");
                
            }else
            {
                paquetes_globales--;
                saldo_nuevo = saldo_jugador - (COSTO_PAQUETE * cant_paquetes);
            
                /* calculo las figuritas */
                /*primero calculo si van a ser espciales o no*/
                /*chekea en cada figurita osea las 5 del sobre*/
                for ( i = 0; i < FIGURITAS_SOBRE; i++)
                {
                    opcion_especial_normal = devolverNumAleatorio(OPCION_NORMAL,OPCION_ESPECIAL);
                    if (opcion_especial_normal == OPCION_NORMAL)/*es normal, calculo num normal*/
                    {
                        figus[i] = devolverNumAleatorio(FIGURITAS_NORMAL_DESDE,FIGURITAS_NORMAL_HASTA);
                        printf("KIOSCKO: FIGURTIA NORMAL :%d\n",figus[i]);
                    }else if (opcion_especial_normal == OPCION_ESPECIAL)/*es especial*/
                    {
                        figus[i] = devolverNumAleatorio(FIGURITAS_ESPECIAL_DESDE,FIGURITAS_ESPECIAL_HASTA);/*valido especial*/
                        if (figus[i] > 10)/*del 11 al 30 no las quiero*/
                        {
                            printf("KIOSCKO: No quiero esta especial %d\n",figus[i]);
                            figus[i] = 0;
                        }else
                        {
                            printf("KIOSCKO: Quiero esta especial %d\n",figus[i]);
                        }
                        
                        
                    }
                }
                
                /*envio las figuritas y el nuevo saldo al jugador*/
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d|%d|%d|%d|%d|%d", saldo_nuevo, figus[0],figus[1],figus[2],figus[3],figus[4]);/*numero de jugador y cantidad de paquetes*/
                /*primero lo voy a hacer con solo 1 paquete*/
                printf("KIOSCKO: jugador %d, tiene un nuevo saldo de %d, sus figus son %d , %d , %d , %d y %d. paquetes globales %d\n",numero_jugador,saldo_nuevo,figus[0],figus[1],figus[2],figus[3],figus[4],paquetes_globales);
                printf("DEBUG kioscko szBuffer antes de enviar: [%s]\n", szBuffer);
                enviar_mensaje(id_cola_mensajes,MSG_JUGADOR+numero_jugador,MSG_KIOSCKO,EVT_SEGUIR_COMPRANDO,szBuffer);
                
                if (paquetes_globales == 0)/*chekeo post enviar paquete porque sino el ultimo paquete no se manda*/
                {
                    for ( i = 0; i < cantJugadores; i++)
                    {
                        /*mandarles fin a todos los jugadores q no hay mas paquetes*/
                        enviar_mensaje(id_cola_mensajes,MSG_JUGADOR + (i + 1),MSG_KIOSCKO,EVT_FIN,"fin");
                    }
                    terminado = TRUE;
                }
                
            }
            
            
        }
        usleep(2000*1000);
        
    }
    
    printf("No hay mas paquetes\n");
    usleep(5000*1000);

    shmdt((char *)memoria);
    shmctl(id_memoria, IPC_RMID, (struct shmid_ds *)NULL);

    borrar_cola_de_mensajes(id_cola_mensajes);
        
    return 0;
}

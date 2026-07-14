#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
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
    int id_cola_mensajes;
    int i;
    int done;
    int evento;
    int contador;
    char szBuffer[LARGO];
    int golesTotales;
    int pateadoresGol[MAX_USERS];
    int nroJugador;
    int cantGoles;
    /*tjugador *jugador;*/
    mensaje msg;

    srand(time(NULL));

    id_cola_mensajes = creo_id_cola_mensajes();
    /*jugador = (tjugador *)malloc(sizeof(tjugador)*MAX_USERS);
    if (jugador == NULL)
    {
        perror("No puedo crear jugador\n");
        exit(-1);
    }*/

    contador = 0;
    done = 0;
    cantGoles = 0;
    golesTotales = 0;
    memset(pateadoresGol,0x00,sizeof(pateadoresGol));

    crearArchivoSincronismo(FILE_SINCRONISMO,1);
    borrar_mensajes(id_cola_mensajes);

    

    printf("Proceso ARQUERO LISTO. Enter para empezar ...");
    getchar();

    while (done == 0)
    {
        recibir_mensaje(id_cola_mensajes,MSG_ARQUERO,&msg);
        memset(szBuffer,0x00,sizeof(szBuffer));
        sscanf(msg.char_mensaje,"%d",&nroJugador);
            
        if (msg.int_evento == EVT_TIRO)
        {
            contador++;
            evento = devolverNumAleatorio(DESDE,HASTA);
            usleep(500*1000);
            switch (evento)
            {
            
                case 2:
                    enviar_mensaje(id_cola_mensajes,MSG_PATEADOR+nroJugador,MSG_ARQUERO,EVT_GOL,"gol");
                    if (cantGoles < MAX_USERS)
                        pateadoresGol[cantGoles++] = msg.int_rte;
                    golesTotales++;
                    
                    break;
                case 3:
                    enviar_mensaje(id_cola_mensajes,MSG_PATEADOR+nroJugador,MSG_ARQUERO,EVT_PALO,"palo");
                    break;
                case 4:
                    enviar_mensaje(id_cola_mensajes,MSG_PATEADOR+nroJugador,MSG_ARQUERO,EVT_TRAVESANO,"travesaño");
                    break;
                case 5:
                    enviar_mensaje(id_cola_mensajes,MSG_PATEADOR+nroJugador,MSG_ARQUERO,EVT_FUERA,"fuera");
                    break;
                default:
                    enviar_mensaje(id_cola_mensajes,MSG_PATEADOR+nroJugador,MSG_ARQUERO,EVT_NINGUNO,"errro");
                    printf("ERORR INESPERADO");
                    break;
            }
        if (contador >= 5)
            {
                done = 1;
            }
        
        }
    

        
    }
    printf("\n=== RESULTADOS FINALES DEL ARQUERO ===\n");
    printf("Goles totales recibidos: %d\n", golesTotales);
    for (i = 0; i < cantGoles; i++){
        printf("  Gol del pateador numero: %d\n", pateadoresGol[i]);
    }
        
    borrar_cola_de_mensajes(id_cola_mensajes);
    
    
    
    return 0;
}

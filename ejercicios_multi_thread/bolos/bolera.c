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

int main(int argc, char  *argv[])
{
    int id_cola_mensajes;
    int destino_jugador;
    int i;
    int done;
    int jugadores_terminados;
    int nro_jugador;      /* 1 o 2 */
    int id_jugador;      /* 0 o 1  (indice en el vector) */
    int pinos_tirados;
    int puntos[MAX_USERS];
    char str_dato[LARGO];
    tjugador *jugador;
    mensaje msg;

    srand(time(NULL));

    done = 0;
    jugadores_terminados = 0;
    id_cola_mensajes = creo_id_cola_mensajes();

    jugador = (tjugador *)malloc(sizeof(tjugador)*MAX_USERS);
    if (jugador == NULL)
    {
        perror("No puedo crear jugador\n");
        exit(-1);
    }
    
    for (i = 0; i < MAX_USERS; i++)
    {
        jugador[i].numeroJugador = i+1;
        jugador[i].cantidadTiros = 0;
        jugador[i].cantidadPinosEnPie[0] = 10;/*en la pisicion 0 hay 10 para el J1, en la posicion 1 para el J2 hay 10 */
        jugador[i].cantidadPinosEnPie[1] = 10;
        puntos[i] = 0;
    }
    
    
    crearArchivoSincronismo(FILE_SINCRONISMO,1);
    borrar_mensajes(id_cola_mensajes);

    

    printf("Proceso BOLERO LISTO. Enter para empezar ...");
    getchar();

    /*le mando EVENTO INICIO A LOS JUGADORES*/
    for ( i = 0; i < MAX_USERS; i++)
    {
        destino_jugador = PROCESO_JUGADOR + (i + 1);
        enviar_mensaje(id_cola_mensajes,destino_jugador,PROCESO_BOLERO,EVT_INICIO,"INICIO");
        printf("BOLERO ENVIO MENSAJE DE INICIO AL JUGADOR %d\n",i+1);
        
    }
    printf("BOLERO: ESPERO A LOS JUGADORES\n");

    while (done == 0)
    {
        recibir_mensaje(id_cola_mensajes,PROCESO_BOLERO,&msg);
        switch (msg.int_evento)
        {
        case EVT_TIRO:
            nro_jugador = msg.int_rte - PROCESO_JUGADOR;
            id_jugador = nro_jugador - 1;
            pinos_tirados = rand() % (jugador[id_jugador].cantidadPinosEnPie[id_jugador]) + 1;/*tira entre 1 y la cantidad de pinos que quedan en pie*/
            jugador[id_jugador].cantidadPinosEnPie[id_jugador] -= pinos_tirados;/*actualizo la cantidad de pinos en pie*/
            jugador[id_jugador].cantidadTiros += 1;/*actualizo la cantidad de tiros que hizo el jugador*/
            printf("BOLERO RECIBI EVENTO TIRO DEL JUGADOR %d y tire %d pinos\n",nro_jugador, pinos_tirados);

            if (jugador[id_jugador].cantidadPinosEnPie[id_jugador] == 0)
            {
                printf("CHUZA! EL JUGADOR %d SE QUEDO SIN PINOS EN PIE\n",nro_jugador);
                puntos[id_jugador] += 20;
            }else
            {
                puntos[id_jugador] += pinos_tirados;
            }
            
            if (jugador[id_jugador].cantidadPinosEnPie[id_jugador]==0 || jugador[id_jugador].cantidadTiros>= 3)
            {
                sprintf(str_dato, "%d", puntos[id_jugador]);
                enviar_mensaje(id_cola_mensajes,
                            PROCESO_JUGADOR + nro_jugador,
                            PROCESO_BOLERO,
                            EVT_FIN,
                            str_dato);
                printf("BOLERO: Envio EVT_FIN al jugador %d con %d puntos\n",
                    nro_jugador, puntos[id_jugador]);

                jugadores_terminados++;
                if (jugadores_terminados >= MAX_USERS)
                    done = 1;
            }else
            {
                sprintf(str_dato, "%d", pinos_tirados);
                enviar_mensaje(id_cola_mensajes,
                            PROCESO_JUGADOR + nro_jugador,
                            PROCESO_BOLERO,
                            EVT_RESULTADO,
                            str_dato);
                printf("BOLERO: Envio EVT_RESULTADO al jugador %d: %d pinos\n",
                    nro_jugador, pinos_tirados);
            }
            
            break;
        
        default:
            break;
        }
    }
    printf("BOLERO: TERMINO EL JUEGO\n");
    
    free(jugador);
    return 0;
}

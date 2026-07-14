#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdlib.h>

#include "def.h"
#include "global.h"
#include "semaforo.h"
#include "archivos.h"
#include "funciones.h"
#include "clave.h"
#include "memoria.h"
#include "mensajes.h"

int main(int argc, char  *argv[])
{
    int i;
    int j;
    int id_cola_mensajes;
    int id_memoria;
    char szBuffer[LARGO];
    int terminado;
    
    int numero_maritimo_msg;
    int posicion_maritimo_msg;
    int estado_maritimo_msg;
    int uso_poder_maritimo_msg;
    int tipo_maritimo_msg;


    int estados_maritimos[MAX_MARITIMOS + 1];
    int posiciones_maritimo[MAX_MARITIMOS + 1];
    int tipos_maritimo[MAX_MARITIMOS + 1];

    int peces_vivos;
    int cercania;
    int distancia;

    int ganador_tipo;
    int ganador_numero;

    tmaritimo *memoria;
    mensaje msg;

    /*inicializo variables y limpio*/
    memoria = NULL;
    memset(&msg,     0x00, sizeof(msg));
    memset(szBuffer, 0x00, sizeof(szBuffer));
    terminado = FALSE;
    
    numero_maritimo_msg = 0;
    posicion_maritimo_msg = 0;
    estado_maritimo_msg = 0;
    uso_poder_maritimo_msg = 0;
    tipo_maritimo_msg = 0;

    memset(estados_maritimos, 0x00, sizeof(estados_maritimos));
    memset(posiciones_maritimo, 0x00, sizeof(posiciones_maritimo));
    memset(tipos_maritimo, 0x00, sizeof(tipos_maritimo));

    peces_vivos = 3;
    cercania = 0;
    distancia = 0;

    ganador_tipo = 0;
    ganador_numero = 0;

    
    id_cola_mensajes = creo_id_cola_mensajes();
    srand(time(NULL));
    
    memoria = (tmaritimo*)creo_memoria(sizeof(tmaritimo)*MAX_MARITIMOS, &id_memoria);
    memset(memoria, 0x00, sizeof(tmaritimo) * MAX_MARITIMOS);
    
    crearArchivoSincronismo(FILE_SINCRONISMO,1);
    borrar_mensajes(id_cola_mensajes);

    
    printf("PROCESO PISTA LISTA PARA EMPEZAR. Enter para empezar...\n");
    getchar();



    for (i = 0; i < MAX_MARITIMOS; i++)
    {
        memoria[i].numero_maritimo = i + 1;
        memoria[i].distancia_meta = DISTANCIA_FIN;
        memoria[i].estado = ESTADO_CORRIENDO;
        
        if (i >= 0 && i <= 2)
        {
            memoria[i].tipo_maritimo = PEZ;
        }
        else if (i == 3)
        {
            memoria[i].tipo_maritimo = TIBURON;
        }
        else
        {
            memoria[i].tipo_maritimo = PULPO;
        }

        estados_maritimos[i + 1] = ESTADO_CORRIENDO;
        tipos_maritimo[i + 1] = memoria[i].tipo_maritimo;

        enviar_mensaje(id_cola_mensajes, MSG_MARITIMO + (i + 1), MSG_PISTA, EVT_INICIO, "inicio");
        enviar_mensaje(id_cola_mensajes, MSG_MARITIMO + (i + 1), MSG_PISTA, EVT_TURNO, "0|0|0|0|0");
    
    }
    printf("Pista: inicio enviado a los %d TODOS LOS MARITIMOS \n", MAX_MARITIMOS);
    while (terminado == FALSE)
    {
        /*loop de logica*/
        printf("Pista: esperando mensajes...\n");
        memset(&msg, 0x00, sizeof(msg));
        recibir_mensaje(id_cola_mensajes,MSG_PISTA,&msg);
        memset(szBuffer, 0x00, sizeof(szBuffer));
        sscanf(msg.char_mensaje, "%d|%d|%d|%d|%d", &numero_maritimo_msg, &posicion_maritimo_msg, &estado_maritimo_msg, &uso_poder_maritimo_msg, &tipo_maritimo_msg);

        
        switch (msg.int_evento)
        {
            case EVT_AVANZA:
                printf("Maritimo %d avanza a la posicion %d\n", numero_maritimo_msg, posicion_maritimo_msg);
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d|%d|%d|%d|%d", numero_maritimo_msg, posicion_maritimo_msg, estado_maritimo_msg, uso_poder_maritimo_msg, tipo_maritimo_msg);

                enviar_mensaje(id_cola_mensajes, MSG_MARITIMO + numero_maritimo_msg, MSG_PISTA, EVT_TURNO, szBuffer);
                break;
            case EVT_MUERE:
                printf("Maritimo %d murio en la posicion %d\n", numero_maritimo_msg, posicion_maritimo_msg);
                if (estados_maritimos[numero_maritimo_msg] == ESTADO_CORRIENDO)
                {
                    estados_maritimos[numero_maritimo_msg] = ESTADO_MUERTO;
                    if (tipos_maritimo[numero_maritimo_msg] == PEZ)
                    {
                        peces_vivos--;
                    }
                }
                if (peces_vivos == 0)
                {
                    printf("PISTA: no quedan peces vivos. Fin de la carrera, ganan los depredadores.\n");
                    terminado = TRUE;
                }
                break;
            case EVT_ATAQUE_ENVIAR:
                if (tipo_maritimo_msg == TIBURON)
                {
                    cercania = CERCANIA_TIBURON;
                    printf("PISTA: el tiburon %d ataca desde la posicion %d\n", numero_maritimo_msg, posicion_maritimo_msg);
                }
                else
                {
                    cercania = CERCANIA_PULPO;
                    printf("PISTA: el pulpo %d ataca desde la posicion %d\n", numero_maritimo_msg, posicion_maritimo_msg);
                }
                

                for (j = 1; j <= MAX_MARITIMOS; j++)
                {
                    if (tipos_maritimo[j] == PEZ && estados_maritimos[j] == ESTADO_CORRIENDO)
                    {
                        distancia = posiciones_maritimo[j] - posicion_maritimo_msg;
                        if (distancia < 0)
                        {
                            distancia = -distancia;
                        }
                        if (distancia <= cercania)
                        {
                            printf("PISTA: el pez %d estaba a %d mt, muere en el ataque\n", j, distancia);
                            estados_maritimos[j] = ESTADO_MUERTO;
                            peces_vivos--;

                            memset(szBuffer, 0x00, sizeof(szBuffer));
                            sprintf(szBuffer, "ataque");
                            enviar_mensaje(id_cola_mensajes, MSG_MARITIMO + j, MSG_PISTA, EVT_ATAQUE_RECIBIR, szBuffer);
                        }
                    }
                }

                if (peces_vivos == 0)
                {
                    printf("PISTA: no quedan peces vivos. Fin de la carrera, ganan los depredadores.\n");
                    terminado = TRUE;
                }

                /* le devuelvo el turno al atacante */
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d|%d|%d|%d|%d", numero_maritimo_msg, posicion_maritimo_msg, estado_maritimo_msg, uso_poder_maritimo_msg, tipo_maritimo_msg);
                enviar_mensaje(id_cola_mensajes, MSG_MARITIMO + numero_maritimo_msg, MSG_PISTA, EVT_TURNO, szBuffer);
                break;
            case EVT_LLEGA_META:
                printf("GANADOR: maritimo %d, en la posicion %d\n", numero_maritimo_msg, posicion_maritimo_msg);
                estados_maritimos[numero_maritimo_msg] = ESTADO_META;
                ganador_tipo = tipo_maritimo_msg;
                ganador_numero = numero_maritimo_msg;
                terminado = TRUE;
                break;
            
            default:
                break;
        }

    }
    /*quiero saber quien gano la carrera*/
    if (ganador_numero != 0)
    {
        if (ganador_tipo == PEZ)
        {
            printf("PISTA: gano un pez. Los peces sobrevivientes se salvan!\n");
        }
        else
        {
            printf("PISTA: gano un depredador. Los peces sobrevivientes perecen!\n");
        }
    }
    else
    {
        printf("PISTA: no quedan peces vivos. Ganan los depredadores.\n");
    }

    /* aviso FIN a los que sigan corriendo, para que sus hilos terminen */
    for (i = 1; i <= MAX_MARITIMOS; i++)
    {
        if (estados_maritimos[i] == ESTADO_CORRIENDO)
        {
            memset(szBuffer, 0x00, sizeof(szBuffer));
            sprintf(szBuffer, "FIN");
            enviar_mensaje(id_cola_mensajes, MSG_MARITIMO + i, MSG_PISTA, EVT_FIN, szBuffer);
            printf("Aviso FIN al maritimo %d, que seguia en carrera\n", i);
        }
    }
    
    /* EXTRA */
    if (abrirArchivo("resultados.txt", "w") == TRUE)
    {
        for (i = 1; i <= MAX_MARITIMOS; i++)
        {
            memset(szBuffer, 0x00, sizeof(szBuffer));

            if (tipos_maritimo[i] == PEZ)
            {
                sprintf(szBuffer, "Pez %d: ", i);
            }else if (tipos_maritimo[i] == TIBURON)
            {
                sprintf(szBuffer, "Tiburon %d: ", i);
            }
            else{
                sprintf(szBuffer, "Pulpo %d: ", i);
                }
            escribirArchivo(szBuffer);

            memset(szBuffer, 0x00, sizeof(szBuffer));
            if (estados_maritimos[i] == ESTADO_META)
            {
                sprintf(szBuffer, "GANADOR\n");
            }else if (estados_maritimos[i] == ESTADO_MUERTO)
            {
                sprintf(szBuffer, "MURIO\n");
            }else
            {
                sprintf(szBuffer, "SOBREVIVIO\n");
            }
            escribirArchivo(szBuffer);
        }
        cerrarArchivo();
        printf("Resultados guardados en resultados.txt\n");
    }

    printf("MUESTRO DATOS FINALES\n");
    usleep(5000*1000);

    shmdt((char *)memoria);
    shmctl(id_memoria, IPC_RMID, (struct shmid_ds *)NULL);

    borrar_cola_de_mensajes(id_cola_mensajes);
        
    return 0;
}

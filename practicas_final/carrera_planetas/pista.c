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
    
    int numero_planeta;
    int posicion_planeta;
    int vueltas_planeta;
    int estado_planeta_msg;
    int uso_poder_planeta;
    
    int planetas_vivos;
    int estado_planetas[MAX_PLANETAS + 1];

    int candidatos[MAX_PLANETAS];
    int cant_candidatos;
    int indice_victima;

    tplaneta *memoria;
    mensaje msg;

    /*inicializo variables y limpio*/
    memoria = NULL;
    memset(&msg,     0x00, sizeof(msg));
    memset(szBuffer, 0x00, sizeof(szBuffer));
    terminado = FALSE;
    
    numero_planeta = 0;
    posicion_planeta = 0;
    vueltas_planeta = 0;
    estado_planeta_msg = 0;
    uso_poder_planeta = 0;
    
    memset(estado_planetas, 0x00, sizeof(estado_planetas));
    planetas_vivos = MAX_PLANETAS;

    cant_candidatos = 0;
    indice_victima = 0;
    
    id_cola_mensajes = creo_id_cola_mensajes();
    srand(time(NULL));
    
    memoria = (tplaneta*)creo_memoria(sizeof(tplaneta)*MAX_PLANETAS, &id_memoria);
    memset(memoria, 0x00, sizeof(tplaneta) * MAX_PLANETAS);
    
    crearArchivoSincronismo(FILE_SINCRONISMO,1);
    borrar_mensajes(id_cola_mensajes);

    
    printf("PROCESO PISTA LISTA PARA EMPEZAR. Enter para empezar...\n");
    getchar();


    for (i = 0; i < MAX_PLANETAS; i++)
    {
        memoria[i].numero_planeta = i + 1;
        memoria[i].distancia_meta = DISTANCIA_FIN;
        memoria[i].estado = ESTADO_CORRIENDO;
        estado_planetas[i + 1] = ESTADO_CORRIENDO;

        enviar_mensaje(id_cola_mensajes, MSG_PLANETA + (i + 1), MSG_PISTA, EVT_INICIO, "inicio");
        enviar_mensaje(id_cola_mensajes, MSG_PLANETA + (i + 1), MSG_PISTA, EVT_TURNO, "0|0|0|0|0");
    
    }
    printf("Pista: inicio enviado a los %d planetas\n", MAX_PLANETAS);
    while (terminado == FALSE)
    {
        /*loop de logica*/
        printf("Pista: esperando mensajes...\n");
        recibir_mensaje(id_cola_mensajes,MSG_PISTA,&msg);
        memset(szBuffer, 0x00, sizeof(szBuffer));
        sscanf(msg.char_mensaje, "%d|%d|%d|%d|%d", &numero_planeta, &posicion_planeta, &vueltas_planeta, &estado_planeta_msg, &uso_poder_planeta);
    

        switch (msg.int_evento)
        {
            case EVT_AVANZA:
                printf("Planeta %d avanza a la posicion %d\n", numero_planeta, posicion_planeta);
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d|%d|%d|%d|%d", numero_planeta, posicion_planeta, vueltas_planeta, estado_planeta_msg, uso_poder_planeta);
                enviar_mensaje(id_cola_mensajes, MSG_PLANETA + numero_planeta, MSG_PISTA, EVT_TURNO, szBuffer);
                break;
            case EVT_MUERE:
                printf("Planeta %d fue destruido en la posicion %d.\n", numero_planeta, posicion_planeta);
                estado_planetas[numero_planeta] = ESTADO_MUERTO;
                planetas_vivos--;
                if (planetas_vivos == 0)
                {
                    printf("Todos los planetas fueron destruidos. No hay ganador.\n");
                    terminado = TRUE;
                }
                break;
            case EVT_ATAQUE_ENVIAR:
                printf("Planeta %d (dino) lanza su poder de ataque\n", numero_planeta);

                cant_candidatos = 0;
                for (j = 1; j <= MAX_PLANETAS; j++)
                {
                    if (estado_planetas[j] == ESTADO_CORRIENDO && memoria[j - 1].tipo_planeta != DINO)
                    {
                        candidatos[cant_candidatos] = j;
                        cant_candidatos++;
                    }
                }
                
                if (cant_candidatos > 0)
                {
                    indice_victima = devolverNumAleatorio(0, cant_candidatos - 1);
                    printf("Pista elige como victima al planeta %d\n", candidatos[indice_victima]);

                    memset(szBuffer, 0x00, sizeof(szBuffer));
                    sprintf(szBuffer, "%d|0|0|0|0", candidatos[indice_victima]);
                    enviar_mensaje(id_cola_mensajes, MSG_PLANETA + candidatos[indice_victima], MSG_PISTA, EVT_ATAQUE_RECIBIR, szBuffer);
                }
                else
                {
                    printf("No hay planetas validos para atacar\n");
                }

                /* el dino uso su turno en atacar, le devuelvo el proximo turno */
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d|%d|%d|%d|%d", numero_planeta, posicion_planeta, vueltas_planeta, estado_planeta_msg, uso_poder_planeta);
                enviar_mensaje(id_cola_mensajes, MSG_PLANETA + numero_planeta, MSG_PISTA, EVT_TURNO, szBuffer);
                break;
            case EVT_LLEGA_META:
                printf("GANADOR: Planeta %d, en la posicion %d, en %d vueltas!\n", numero_planeta, posicion_planeta, vueltas_planeta);
                estado_planetas[numero_planeta] = ESTADO_META;
                terminado = TRUE;
                break;
            
            default:
                break;
        }

    }

    /* aviso FIN a los planetas que sigan corriendo, para que sus hilos terminen */
    for (i = 1; i <= MAX_PLANETAS; i++)
    {
        if (estado_planetas[i] == ESTADO_CORRIENDO)
        {
            memset(szBuffer, 0x00, sizeof(szBuffer));
            sprintf(szBuffer, "FIN");
            enviar_mensaje(id_cola_mensajes, MSG_PLANETA + i, MSG_PISTA, EVT_FIN, szBuffer);
            printf("Aviso FIN al planeta %d, que seguia en carrera\n", i);
        }
    }

    /* EXTRA: guardo en archivo el ganador y los planetas destruidos */
    if (abrirArchivo("resultados.txt", "w") == TRUE)
    {
        for ( i = 0; i <= MAX_PLANETAS; i++)
        {
            
            if (estado_planetas[i] == ESTADO_META)
            {
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "Planeta %d: GANADOR\n", i);
            }else if (estado_planetas[i] == ESTADO_MUERTO)
            {
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "Planeta %d: DESTRUIDO\n", i);
            }else
            {
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "Planeta %d: SIGUE EN CARRERA\n", i);
            }
            escribirArchivo(szBuffer);
            
            
        }
        cerrarArchivo();
        printf("Resultado guardados\n");
    }
    printf("MUESTRO DATOS FINALES\n");
    usleep(5000*1000);

    shmdt((char *)memoria);
    shmctl(id_memoria, IPC_RMID, (struct shmid_ds *)NULL);

    borrar_cola_de_mensajes(id_cola_mensajes);
        
    return 0;
}

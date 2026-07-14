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

    int numero_luchador_msg;
    int vida_luchador_msg;
    int estado_luchador_msg;
    int tipo_luchador_msg;
    int dato_luchador_msg;

    int estados_luchadores[MAX_LUCHADORES + 1];
    int tipos_luchadores[MAX_LUCHADORES + 1];
    int destino_gladiador[MAX_LUCHADORES + 1];

    int gladiadores_vivos;
    int imperio_vivos;

    int candidatos[MAX_LUCHADORES];
    int cant_candidatos;
    int indice_victima;
    int tipo_rival;
    int decision_emperador;

    tluchador *memoria;
    mensaje msg;

    /*inicializo variables y limpio*/
    memoria = NULL;
    memset(&msg,     0x00, sizeof(msg));
    memset(szBuffer, 0x00, sizeof(szBuffer));
    terminado = FALSE;
    
    numero_luchador_msg = 0;
    vida_luchador_msg = 0;
    estado_luchador_msg = 0;
    tipo_luchador_msg = 0;
    dato_luchador_msg = 0;

    memset(estados_luchadores, 0x00, sizeof(estados_luchadores));
    memset(tipos_luchadores, 0x00, sizeof(tipos_luchadores));
    memset(destino_gladiador, 0x00, sizeof(destino_gladiador));

    gladiadores_vivos = 3;
    imperio_vivos = 5;

    cant_candidatos = 0;
    indice_victima = 0;
    tipo_rival = 0;

    decision_emperador = 0;

    
    id_cola_mensajes = creo_id_cola_mensajes();
    srand(time(NULL));
    
    memoria = (tluchador*)creo_memoria(sizeof(tluchador)*MAX_LUCHADORES, &id_memoria);
    memset(memoria, 0x00, sizeof(tluchador) * MAX_LUCHADORES);
    
    crearArchivoSincronismo(FILE_SINCRONISMO,1);
    borrar_mensajes(id_cola_mensajes);

    
    printf("PROCESO coliseo LISTA PARA EMPEZAR. Enter para empezar\n");
    getchar();



    for (i = 0; i < MAX_LUCHADORES; i++)
    {
        memoria[i].numero_luchador = i + 1;
        memoria[i].vida = VIDA_LUCHADOR;/*140*/
        memoria[i].estado = ESTADO_VIVO;
        /*las primeras 5 posiciones son imperio y las ultimas 3 son gladiadores*/
        if (i < 5)
        {
            memoria[i].tipo_luchador = IMPERIO;
        }
        else
        {
            memoria[i].tipo_luchador = GLADIADOR;
        }

        estados_luchadores[i + 1] = ESTADO_VIVO;
        tipos_luchadores[i + 1] = memoria[i].tipo_luchador;

        memset(szBuffer, 0x00, sizeof(szBuffer));
        sprintf(szBuffer, "inicio");
        /*le mando incio a los threades*/
        enviar_mensaje(id_cola_mensajes, MSG_LUCHADOR + (i + 1), MSG_COLISEO, EVT_INICIO, szBuffer);

        memset(szBuffer, 0x00, sizeof(szBuffer));
        sprintf(szBuffer, "0|0|0|0|0");
        /*les mando 1 primer turno fantasma para q comienzen*/
        enviar_mensaje(id_cola_mensajes, MSG_LUCHADOR + (i + 1), MSG_COLISEO, EVT_TURNO, szBuffer);
    }
    printf("Inicio enviado a los %d TODOS LOS LUCHADORES \n", MAX_LUCHADORES);
    while (terminado == FALSE)
    {
        /*loop de logica*/
        printf("Esperando mensajes\n");
        memset(&msg, 0x00, sizeof(msg));
        recibir_mensaje(id_cola_mensajes, MSG_COLISEO, &msg);

        memset(szBuffer, 0x00, sizeof(szBuffer));
        sscanf(msg.char_mensaje, "%d|%d|%d|%d|%d", &numero_luchador_msg, &vida_luchador_msg, &estado_luchador_msg, &tipo_luchador_msg, &dato_luchador_msg);

        
        switch (msg.int_evento)
        {
            case EVT_ATAQUE_ENVIAR:
                cant_candidatos = 0;/*en cada vuelta*/
                memset(candidatos, 0x00, sizeof(candidatos));
                if (tipo_luchador_msg == GLADIADOR)
                {
                    /*el gladiador lucha contra imperio*/
                    tipo_rival = IMPERIO;
                }
                else
                {
                    /*el imperio lucha contra gladiador*/
                    tipo_rival = GLADIADOR;
                }
                for (j = 1; j <= MAX_LUCHADORES; j++)
                {
                    if (tipos_luchadores[j] == tipo_rival && estados_luchadores[j] == ESTADO_VIVO)
                    {
                        candidatos[cant_candidatos] = j;
                        /*armo los luchadores para atacar*/
                        /*tienen q ser del otro tipo y estar vivos*/
                        /*se van guardando en distitnas posicione*/
                        cant_candidatos++;
                    }
                    
                }
                if (cant_candidatos > 0)
                {
                    indice_victima = devolverNumAleatorio(0, cant_candidatos - 1);/*decidir contra quien va a pelear, lo hace random*/
                    printf("Luchador %d ataca al luchador %d con %d de ataque\n", numero_luchador_msg, candidatos[indice_victima], dato_luchador_msg);

                    memset(szBuffer, 0x00, sizeof(szBuffer));
                    sprintf(szBuffer, "%d|0|0|0|%d", candidatos[indice_victima], dato_luchador_msg);
                    enviar_mensaje(id_cola_mensajes, MSG_LUCHADOR + candidatos[indice_victima], MSG_COLISEO, EVT_ATAQUE_RECIBIR, szBuffer);
                
                }else
                {
                    printf("Luchador %d quiere atacar pero no hay nadie vivo del tipo contrario%d\n", numero_luchador_msg, tipo_rival);
                }
                
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d|%d|%d|%d|%d", numero_luchador_msg, vida_luchador_msg, estado_luchador_msg, tipo_luchador_msg, dato_luchador_msg);
                
                enviar_mensaje(id_cola_mensajes, MSG_LUCHADOR + numero_luchador_msg, MSG_COLISEO, EVT_TURNO, szBuffer);/*el turno es el ataque*/
                break;
            case EVT_MUERE:
                printf("Luchador %d murio\n", numero_luchador_msg);
                
                if (estados_luchadores[numero_luchador_msg] == ESTADO_VIVO)
                {
                    estados_luchadores[numero_luchador_msg] = ESTADO_MUERTO;/*le cambia el estado a muerto*/
                    /*resta la cantidad de gladiadores o imperio vivos, depende de su tipo*/
                    if (tipos_luchadores[numero_luchador_msg] == GLADIADOR)
                    {
                        gladiadores_vivos--;
                    }
                    else
                    {
                        imperio_vivos--;
                    }
                }
                /*chekea para atacar*/
                if (gladiadores_vivos == 0)
                {
                    printf("No quedan gladiadores vivos. Fin de la pelea, ganan los imperio\n");
                    terminado = TRUE;
                }
                else if (imperio_vivos == 0)
                {
                    printf("No quedan imperio vivos. Fin de la pelea, ganan los gladiadores\n");
                    terminado = TRUE;
                }
                break;
            
            default:
                break;
        }

    }
    /*chekeo para fin y chekear decision de emperador*/
    if (imperio_vivos == 0)
    {
        printf("No quedan luchadores del imperio. Ganan los gladiadores esclavos\n");
        printf("El emperador decide\n");
        for ( i = 1; i <= MAX_LUCHADORES; i++)
        {
            if (tipos_luchadores[i] == GLADIADOR && estados_luchadores[i] == ESTADO_VIVO)
            {
                decision_emperador = devolverNumAleatorio(EMPERADOR_DECISION_DESDE, EMPERADOR_DECISION_HASTA);
                if (decision_emperador == EMPERADOR_DECISION_VIVO_1 || decision_emperador == EMPERADOR_DECISION_VIVO_2)/*1 o 2*/
                {
                    printf("El emperador decide que el gladiador %d viva\n", i);
                    destino_gladiador[i] = EMPERADOR_DECISION_VIVO_1;
                }
                else
                {
                    printf("El emperador decide que el gladiador %d muera\n", i);
                    destino_gladiador[i] = ESTADO_MUERTO;
                }

                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d|0|0|%d|%d", i, GLADIADOR, decision_emperador);
                enviar_mensaje(id_cola_mensajes, MSG_LUCHADOR + i, MSG_COLISEO, EVT_FIN, szBuffer);/*le manda la decision del emperador con el fin*/
            
            }
            
        }
        

    }
    else
    {
        printf("No quedan gladiadores vivos. Ganan los luchadores imperio\n");
        for ( i = 1; i <= MAX_LUCHADORES; i++)
        {
            if (tipos_luchadores[i] == IMPERIO && estados_luchadores[i] == ESTADO_VIVO)
            {
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d|0|0|%d|%d", i, IMPERIO, 0);
                enviar_mensaje(id_cola_mensajes, MSG_LUCHADOR + i, MSG_COLISEO, EVT_FIN, szBuffer);
            
            }
            
        }
    }

    /*escribir el file de los resultados finales en resultados.txt*/
    if (abrirArchivo("resultados.txt", "w") == TRUE)
    {
        for (i = 1; i <= MAX_LUCHADORES; i++)
        {
            memset(szBuffer, 0x00, sizeof(szBuffer));

            if (tipos_luchadores[i] == GLADIADOR)
            {
                sprintf(szBuffer, "Gladiador %d: ", i);
            }
            else
            {
                sprintf(szBuffer, "Imperio %d: ", i);
            }
            escribirArchivo(szBuffer);

            memset(szBuffer, 0x00, sizeof(szBuffer));
            if (tipos_luchadores[i] == GLADIADOR && destino_gladiador[i] == EMPERADOR_DECISION_VIVO_1)
            {
                sprintf(szBuffer, "LIBRE POR DECISION DEL EMPERADOR\n");
            }else if (tipos_luchadores[i] == GLADIADOR && destino_gladiador[i] == ESTADO_MUERTO)
            {
                sprintf(szBuffer, "Lo mato el emperador\n");
            }else if (estados_luchadores[i] == ESTADO_MUERTO)
            {
                sprintf(szBuffer, "MURIO PELEANDO\n");
            }else{
                sprintf(szBuffer,"Sobrevivo ganando\n");
            }
            escribirArchivo(szBuffer);
            

        }
        cerrarArchivo();
        printf("Resultados guardados en resultados.txt\n");
    }

    
    usleep(5000*1000);

    shmdt((char *)memoria);
    shmctl(id_memoria, IPC_RMID, (struct shmid_ds *)NULL);

    borrar_cola_de_mensajes(id_cola_mensajes);
        
    return 0;
}

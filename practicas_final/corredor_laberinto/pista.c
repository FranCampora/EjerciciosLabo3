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
    
    int id_cola_mensajes;
    int id_memoria;
    char szBuffer[LARGO];
    int terminado;
    
    int numero_corredor_msg;
    int posicion_msg;
    int estado_msg;

    int corredores_vivos;
    int metraje_camino;
    int rival_destino;

    int golpe_muchacho;
    int golpe_criatura;

    int salto_muchacho;
    int salto_criatura;

    tcorredor *memoria;
    mensaje msg;

    /*inicializo variables y limpio*/
    memoria = NULL;
    memset(&msg,     0x00, sizeof(msg));
    memset(szBuffer, 0x00, sizeof(szBuffer));
    terminado = FALSE;
    
    numero_corredor_msg = 0;
    posicion_msg = 0;
    estado_msg = 0;

    corredores_vivos = MAX_CORREDORES;
    metraje_camino = 0;

    golpe_muchacho = 0;
    golpe_criatura = 0;
    salto_muchacho = 0;
    salto_criatura = 0;
    rival_destino = 0;

    
    id_cola_mensajes = creo_id_cola_mensajes();
    srand(time(NULL));
    
    memoria = (tcorredor *) creo_memoria(sizeof(tcorredor) * MAX_CORREDORES, &id_memoria);
    memset(memoria, 0x00, sizeof(tcorredor) * MAX_CORREDORES);

    crearArchivoSincronismo(FILE_SINCRONISMO, 1);
    borrar_mensajes(id_cola_mensajes);

    printf("PROCESO PISTA LISTA PARA EMPEZAR. Enter para empezar...\n");
    getchar();

    /* camino 1: Thomas vs Criatura1 */
    metraje_camino = devolverNumAleatorio(METRAJE_ENTRADA_DESDE, METRAJE_ENTRADA_HASTA);
    memoria[0].tipo_corredor = TIPO_THOMAS;
    memoria[0].metraje_entrada = metraje_camino;
    memoria[0].estado = ESTADO_CORRIENDO;
    memoria[0].msg_destino = MSG_THOMAS;
    memoria[0].msg_rival = MSG_CRIATURA1;

    memoria[1].tipo_corredor = TIPO_CRIATURA;
    memoria[1].metraje_entrada = metraje_camino;
    memoria[1].estado = ESTADO_CORRIENDO;
    memoria[1].msg_destino = MSG_CRIATURA1;
    memoria[1].msg_rival = MSG_THOMAS;

    /* camino 2: Muchacho1 vs Criatura2 */
    metraje_camino = devolverNumAleatorio(METRAJE_ENTRADA_DESDE, METRAJE_ENTRADA_HASTA);
    memoria[2].tipo_corredor = TIPO_MUCHACHO;
    memoria[2].metraje_entrada = metraje_camino;
    memoria[2].estado = ESTADO_CORRIENDO;
    memoria[2].msg_destino = MSG_MUCHACHO1;
    memoria[2].msg_rival = MSG_CRIATURA2;

    memoria[3].tipo_corredor = TIPO_CRIATURA;
    memoria[3].metraje_entrada = metraje_camino;
    memoria[3].estado = ESTADO_CORRIENDO;
    memoria[3].msg_destino = MSG_CRIATURA2;
    memoria[3].msg_rival = MSG_MUCHACHO1;

    /* camino 3: Muchacho2 vs Criatura3 */
    metraje_camino = devolverNumAleatorio(METRAJE_ENTRADA_DESDE, METRAJE_ENTRADA_HASTA);
    memoria[4].tipo_corredor = TIPO_MUCHACHO;
    memoria[4].metraje_entrada = metraje_camino;
    memoria[4].estado = ESTADO_CORRIENDO;
    memoria[4].msg_destino = MSG_MUCHACHO2;
    memoria[4].msg_rival = MSG_CRIATURA3;

    memoria[5].tipo_corredor = TIPO_CRIATURA;
    memoria[5].metraje_entrada = metraje_camino;
    memoria[5].estado = ESTADO_CORRIENDO;
    memoria[5].msg_destino = MSG_CRIATURA3;
    memoria[5].msg_rival = MSG_MUCHACHO2;


    /* aviso INICIO a los 6 corredores */
    enviar_mensaje(id_cola_mensajes, MSG_THOMAS, MSG_PISTA, EVT_INICIO, "inicio");
    enviar_mensaje(id_cola_mensajes, MSG_MUCHACHO1, MSG_PISTA, EVT_INICIO, "inicio");
    enviar_mensaje(id_cola_mensajes, MSG_MUCHACHO2, MSG_PISTA, EVT_INICIO, "inicio");
    enviar_mensaje(id_cola_mensajes, MSG_CRIATURA1, MSG_PISTA, EVT_INICIO, "inicio");
    enviar_mensaje(id_cola_mensajes, MSG_CRIATURA2, MSG_PISTA, EVT_INICIO, "inicio");
    enviar_mensaje(id_cola_mensajes, MSG_CRIATURA3, MSG_PISTA, EVT_INICIO, "inicio");

    /* FIX: sin mandar el primer turno, los 6 hilos quedan esperando para siempre
    apenas procesan EVT_INICIO, y pista tambien queda esperando un mensaje
       que nunca les va a llegar. */
    enviar_mensaje(id_cola_mensajes, MSG_THOMAS, MSG_PISTA, EVT_TURNO, "turno");
    enviar_mensaje(id_cola_mensajes, MSG_MUCHACHO1, MSG_PISTA, EVT_TURNO, "turno");
    enviar_mensaje(id_cola_mensajes, MSG_MUCHACHO2, MSG_PISTA, EVT_TURNO, "turno");
    enviar_mensaje(id_cola_mensajes, MSG_CRIATURA1, MSG_PISTA, EVT_TURNO, "turno");
    enviar_mensaje(id_cola_mensajes, MSG_CRIATURA2, MSG_PISTA, EVT_TURNO, "turno");
    enviar_mensaje(id_cola_mensajes, MSG_CRIATURA3, MSG_PISTA, EVT_TURNO, "turno");

    printf("PISTA: inicio enviado a los %d corredores\n", MAX_CORREDORES);


    while (terminado == FALSE)
    {
        /*loop de logica*/
        printf("Pista: esperando mensajes...\n");
        recibir_mensaje(id_cola_mensajes,MSG_PISTA,&msg);
        

        switch (msg.int_evento)
        {
            case EVT_TURNO:
                /* el hilo pide su proximo turno; pista simplemente se lo reenvia */
                enviar_mensaje(id_cola_mensajes, msg.int_rte, MSG_PISTA, EVT_TURNO, "turno");
                break;
            case EVT_GOLPE:
                 /* formato pedido: "num_corredor" (el muchacho pide arbitraje del combate) */
                sscanf(msg.char_mensaje, "%d", &numero_corredor_msg);

                /* busco cual es el destino de mi rival (criatura de mi mismo camino) */
                for (i = 0; i < MAX_CORREDORES; i++)
                {
                    if (memoria[i].msg_destino == numero_corredor_msg)
                    {
                        rival_destino = memoria[i].msg_rival;
                        break;
                    }
                }

                /* chequeo si esa criatura sigue viva */
                estado_msg = ESTADO_CORRIENDO;
                for (i = 0; i < MAX_CORREDORES; i++)
                {
                    if (memoria[i].msg_destino == rival_destino)
                    {
                        estado_msg = memoria[i].estado;
                        break;
                    }
                }

                if (estado_msg != ESTADO_CORRIENDO)
                {
                    /* la criatura de este camino ya no existe: no hay combate, sigue corriendo libre */
                    printf("PISTA: %d ya no tiene rival, sigue corriendo sin combate\n", numero_corredor_msg);
                    enviar_mensaje(id_cola_mensajes, msg.int_rte, MSG_PISTA, EVT_TURNO, "turno");
                }
                else
                {
                    golpe_muchacho = devolverNumAleatorio(GOLPE_MUCHACHO_DESDE, GOLPE_MUCHACHO_HASTA);
                    golpe_criatura = devolverNumAleatorio(GOLPE_CRIATURA_DESDE, GOLPE_CRIATURA_HASTA);
                    printf("PISTA: combate en direccion %d -> golpe muchacho %d, golpe criatura %d\n",
                        numero_corredor_msg, golpe_muchacho, golpe_criatura);

                    memset(szBuffer, 0x00, sizeof(szBuffer));
                    sprintf(szBuffer, "%d|%d", golpe_muchacho, golpe_criatura);
                    enviar_mensaje(id_cola_mensajes, msg.int_rte, MSG_PISTA, EVT_DEFENSA, szBuffer);
                }
                break;
            case EVT_SALTA:
                /* formato pedido: "num_corredor|tipo_corredor" */
                sscanf(msg.char_mensaje, "%d|%d", &numero_corredor_msg, &estado_msg);

                if (estado_msg == TIPO_CRIATURA)
                {
                    salto_criatura = devolverNumAleatorio(SALTO_CRIATURA_DESDE, SALTO_CRIATURA_HASTA);
                    printf("PISTA: salto de criatura en direccion %d -> tiro %d\n", numero_corredor_msg, salto_criatura);
                    memset(szBuffer, 0x00, sizeof(szBuffer));
                    sprintf(szBuffer, "%d", salto_criatura);
                }
                else if (estado_msg == TIPO_THOMAS)
                {
                    salto_muchacho = devolverNumAleatorio(SALTO_THOMAS_DESDE, SALTO_THOMAS_HASTA);
                    printf("PISTA: salto de Thomas en direccion %d -> tiro %d\n", numero_corredor_msg, salto_muchacho);
                    memset(szBuffer, 0x00, sizeof(szBuffer));
                    sprintf(szBuffer, "%d", salto_muchacho);
                }
                else{
                    salto_muchacho = devolverNumAleatorio(SALTO_MUCHACHO_DESDE, SALTO_MUCHACHO_HASTA);
                    printf("PISTA: salto de muchacho en direccion %d -> tiro %d\n", numero_corredor_msg, salto_muchacho);
                    memset(szBuffer, 0x00, sizeof(szBuffer));
                    sprintf(szBuffer, "%d", salto_muchacho);
                }
                enviar_mensaje(id_cola_mensajes, numero_corredor_msg, MSG_PISTA, EVT_SALTA, szBuffer);
                break;
            case EVT_MATA_RIVAL:
                /* formato pedido: "num_corredor" (num_corredor = la propia criatura, informativo).
                la criatura llego a la meta antes: le avisa a pista que su corredor rival muere.
                no se actualiza posicion del corredor muerto (murio por la criatura, no por avance propio).
                   aca SI necesito filtrar por remitente para saber que corredor matar */
                sscanf(msg.char_mensaje, "%d", &numero_corredor_msg);
                switch (msg.int_rte)
                {
                    case MSG_CRIATURA1:
                        printf("PISTA: Criatura1 llego a la meta. Thomas muere.\n");
                        memoria[0].estado = ESTADO_MUERTO;
                        enviar_mensaje(id_cola_mensajes, MSG_THOMAS, MSG_PISTA, EVT_FIN, "fin");
                        break;
                    case MSG_CRIATURA2:
                        printf("PISTA: Criatura2 llego a la meta. Muchacho1 muere.\n");
                        memoria[2].estado = ESTADO_MUERTO;
                        enviar_mensaje(id_cola_mensajes, MSG_MUCHACHO1, MSG_PISTA, EVT_FIN, "fin");
                        break;
                    case MSG_CRIATURA3:
                        printf("PISTA: Criatura3 llego a la meta. Muchacho2 muere.\n");
                        memoria[4].estado = ESTADO_MUERTO;
                        enviar_mensaje(id_cola_mensajes, MSG_MUCHACHO2, MSG_PISTA, EVT_FIN, "fin");
                        break;
                    default:
                        printf("PISTA: EVT_MATA_RIVAL recibido de un remitente inesperado\n");
                        break;
                }
                corredores_vivos--;
                if (corredores_vivos <= 0)
                {
                    printf("PISTA: no quedan corredores. Fin de la carrera sin ganador.\n");
                    terminado = TRUE;
                }
                break;
            case EVT_MUERE:
                /* formato pedido: "num_corredor|posicion" */
                sscanf(msg.char_mensaje, "%d|%d", &numero_corredor_msg, &posicion_msg);

                printf("PISTA: corredor en direccion %d murio en la posicion %d\n",
                    numero_corredor_msg, posicion_msg);

                /* marco en la struct compartida cual corredor murio, filtrando por remitente */
                for (i = 0; i < MAX_CORREDORES; i++)
                {
                    if (memoria[i].msg_destino == msg.int_rte)
                    {
                        memoria[i].estado = ESTADO_MUERTO;
                        memoria[i].posicion = posicion_msg;
                        break;
                    }
                }

                corredores_vivos--;
                if (corredores_vivos <= 0)
                {
                    printf("PISTA: no quedan corredores. Fin de la carrera sin ganador.\n");
                    terminado = TRUE;
                }
                break;

                
            case EVT_LLEGA_META:
                /* formato pedido: "num_corredor|posicion" */
                sscanf(msg.char_mensaje, "%d|%d", &numero_corredor_msg, &posicion_msg);

                printf("PISTA: GANADOR en direccion %d, llego a la meta en la posicion %d\n",
                    numero_corredor_msg, posicion_msg);

                for (i = 0; i < MAX_CORREDORES; i++)
                {
                    if (memoria[i].msg_destino == msg.int_rte)
                    {
                        memoria[i].estado = ESTADO_META;
                        memoria[i].posicion = posicion_msg;
                        break;
                    }
                }

                terminado = TRUE;
                break;
            default:
                break;
        }

    }

    /* aviso FIN a los planetas que sigan corriendo, para que sus hilos terminen */
    for (i = 0; i < MAX_CORREDORES; i++)
    {
        if (memoria[i].estado == ESTADO_CORRIENDO)
        {
            enviar_mensaje(id_cola_mensajes, memoria[i].msg_destino, MSG_PISTA, EVT_FIN, "fin");
            printf("PISTA: aviso FIN a %s\n", memoria[i].nombre_corredor);
        }
    }


    printf("MUESTRO DATOS FINALES\n");
    usleep(5000*1000);

    shmdt((char *)memoria);
    shmctl(id_memoria, IPC_RMID, (struct shmid_ds *)NULL);

    borrar_cola_de_mensajes(id_cola_mensajes);
        
    return 0;
}

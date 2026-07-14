#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <errno.h>

#include "def.h"
#include "global.h"
#include "semaforo.h"
#include "archivos.h"
#include "funciones.h"
#include "clave.h"
#include "memoria.h"
#include "mensajes.h"

/*proceso 1 es el inicializador*/
int main(int argc, char *argv[])
{
    int id_semaforo;
    int id_cola_mensajes;
    int res;

    id_semaforo = creo_semaforo();
    inicia_semaforo(id_semaforo, VERDE);

    id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);

    crearArchivoSincronismo(FILE_SINCRONISMO, 1);
    
    borrar_mensajes(id_cola_mensajes);/*limpio la cola de mensajes*/

    printf("PROCESO 1 DE COLA DE MENSAJES listo. Enter para empezar.\n");
    getchar();

    /*quiero que proceso 1 envíe un mensaje al proceso 2*/
    /*proceso 2 reciba, proceso 2 responda enviando un mensaje*/
    /*proceso 1 recibe el mensaje y corta del while*/


    while (1)
    {
        espera_semaforo(id_semaforo);
        enviar_mensaje(id_cola_mensajes, PROCESO_2,PROCESO_1);
        printf("PROCESO 1 DE COLA DE MENSAJES, ENVIÓ MENAJE al proceso 2\n");
        levanta_semaforo(id_semaforo);
        usleep(1000*1000);

        /*recibo fuera del semaforo
        msgrcv es bloqueante */

        res = recibir_mensaje(id_cola_mensajes,PROCESO_1);
        printf("PROCESO 1: RECIBI RESPUTES DEL PROCESO 2");
        usleep(1000*1000);
        
        if (res >= 0)
        {
            printf("PROCESO 1: INTERCAMBIO EXITOSO, SALIENDO");
            break;
        }
        
    }

    return 0;
}


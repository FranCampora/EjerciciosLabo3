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

int main(int argc, char *argv[])
{
    int id_semaforo;
    int id_cola_mensajes;
    
    esperarArchivoSincronismo(FILE_SINCRONISMO, 1);
    
    id_semaforo = creo_semaforo();
    id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);


    printf("PROCESO 2 DE COLA DE MENSAJES listo. Enter para empezar.\n");
    getchar();
    
    while (1)
    {
        /*espera_semaforo(id_semaforo);*/
        printf("PROCESO 2 DE COLA DE MENSAJES, espero para recibir un mensaje\n");
        recibir_mensaje(id_cola_mensajes,PROCESO_2);
        printf("PROCESO 2 DE COLA DE MENSAJES, recibí un mensaje DEL PROCESO 1\n");

        espera_semaforo(id_semaforo);
        enviar_mensaje(id_cola_mensajes,PROCESO_1,PROCESO_2);
        printf("PROCESO 2: ENVIO RESPUESTA AL PROCESO 1");
        /*borrar_cola_de_mensajes(id_cola_mensajes);limpio la cola de mensajes*/
        levanta_semaforo(id_semaforo);
        usleep(1000*1000);
        break;
    }
    /*borrar_cola_de_mensajes(id_cola_mensajes);limpio la cola al salir*/
    return 0;
}

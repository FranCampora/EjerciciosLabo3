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
    int distancia_espiral;
    int posicion_espiral;
    int mov_mosquito;
    int mov_humo;
    int mosquitos[MAX_MOSQUITOS];
    int salud_mosquito;

    tmosquito *memoria = NULL;

    mensaje msg;

    /*inicializo variables y limpio*/
    memset(szBuffer, 0x00, sizeof(szBuffer));
    memset(&msg,     0x00, sizeof(msg));
    
    id_cola_mensajes = creo_id_cola_mensajes();
    memoria = (tmosquito*)creo_memoria(sizeof(tmosquito)*MAX_MOSQUITOS, &id_memoria);

    srand(time(NULL));/*puedo no necesitarlo*/
    
    crearArchivoSincronismo(FILE_SINCRONISMO,1);
    borrar_mensajes(id_cola_mensajes);

    printf("PROCESO cancha LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    /*logica previa, capaz no se pone*/

    /*actualizo con mem compartida la distancia del espiral*/
    for ( i = 0; i < MAX_MOSQUITOS; i++)
    {
        memoria[i]->distancia_espiral = 95;
        memoria[i]->posicion_humo = 0;
    }
    
    

    /*enviar evento de inicio a los threads mosquito*/
    for ( i = 0; i < MAX_MOSQUITOS; i++)
    {
        enviar_mensaje(id_cola_mensajes, MSG_MOSQUITO+i, MSG_TABLERO, EVT_INICIO, "INICIO");
    }
    
    
    while (/*...*/)
    {
        /*loop de logica*/
    }
    printf("MUESTRO DATOS FINALES\n");
    usleep(5000*1000);

    borrar_cola_de_mensajes(id_cola_mensajes);
        
    return 0;
}

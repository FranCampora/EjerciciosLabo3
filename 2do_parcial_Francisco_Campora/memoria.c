#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>


#include "def.h"
#include "global.h"
#include "semaforo.h"
#include "archivos.h"
#include "funciones.h"
#include "clave.h"

void* creo_memoria(int size, int* r_id_memoria){
    void* ptr_memoria;
    int id_memoria;
    id_memoria = shmget(creo_clave(), size, 0777 | IPC_CREAT);/*estoy creando la clave y despues
    creo la zona compartida usando shmget*/

    if (id_memoria == -1)
    {
        printf("Error al crear la memoria compartida. No se cosegio ID.\n");
        exit(1);
    }
    
    ptr_memoria = (void *)shmat (id_memoria, (char *)0, 0);

    if (ptr_memoria == NULL)
    {
        printf("Error no cosigo memoria compartida.\n");
        exit(1);
    }
    
    *r_id_memoria = id_memoria;
    return ptr_memoria;

}




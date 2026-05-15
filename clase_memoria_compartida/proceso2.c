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
#include "memoria.h"

int main(int argc, char  *argv[])
{
    dato *memoria = NULL;
	int id_memoria;
	int i;

	int id_semaforo;

	id_semaforo = creo_semaforo();

	memoria = (dato*)creo_memoria(sizeof(dato)*LOTE, &id_memoria);
	/*uso la zona de memoria compartida creada por el proceso 1*/

    printf("Proceso 2 listo. Enter para empezar.\n");
    getchar();

	while(1)
	{
		espera_semaforo(id_semaforo);	
    
			printf("leemos la ZONA DE MEMORIA COMPARTIDA\n");
			for (i=0; i<LOTE; i++)
			{
					printf("Leido %d %c\n", memoria[i].numero, memoria[i].letra);
			}
			printf("Dejamos de leer la ZONA DE MEMORIA COMPARTIDA\n");
		levanta_semaforo(id_semaforo);
		usleep(1000*1000);
	}

	shmdt ((char *)memoria);
	shmctl (id_memoria, IPC_RMID, (struct shmid_ds *)NULL);

    return 0;
}

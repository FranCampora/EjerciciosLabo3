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
    int aleatorio;
	int id_semaforo;

	srand(time(NULL));

	id_semaforo = creo_semaforo();
	
	inicia_semaforo(id_semaforo, VERDE);

	memoria = (dato*)creo_memoria(sizeof(dato)*LOTE, &id_memoria);
	/*creo la zona de memoria compartida*/
	
    printf("Proceso 1 listo. Enter para empezar.\n");
    getchar();

	while(LOTE)
	{
		espera_semaforo(id_semaforo);		
			printf("Escribimos en la memoria\n");
			for (i=0; i<LOTE; i++)
			{
				aleatorio = rand()%(HASTA-DESDE)+DESDE;
				memoria[i].numero = aleatorio ;
				memoria[i].letra  = 0x41 + aleatorio ;
				printf("Escrito %d %c\n", memoria[i].numero, memoria[i].letra);
				usleep(1000*1000);
			}
			printf("Dejamos de escribir en la memoria\n");
		levanta_semaforo(id_semaforo);
		usleep(2000*1000);
	}
	
	shmdt ((char *)memoria);
	shmctl (id_memoria, IPC_RMID, (struct shmid_ds *)NULL);


    return 0;
}

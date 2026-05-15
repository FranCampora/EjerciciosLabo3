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

int main(int argc, char *argv[])
{
    int id_semaforo;
    via *memoria = NULL;
    int id_memoria;
    int menorCantidadAutos;
    int i;
    int tiempoEspera;
    int CANTIDAD_DE_VIAS;
    
    int viaSeleccionada;


    if(argc != 2)
		{
			printf("Ingrese LA CANTIDAD DE VIAS\n");
			exit(1);
		}
    
    CANTIDAD_DE_VIAS = atoi(argv[1]);
    if (CANTIDAD_DE_VIAS <= 0)
    {
        printf("La cantidad de vias debe ser un número positivo.\n");
        exit(1);
    }

    esperarArchivoSincronismo(FILE_SINCRONISMO, 1);

    id_semaforo = creo_semaforo();
    memoria = (via*)creo_memoria(sizeof(via)*CANTIDAD_DE_VIAS, &id_memoria);

    srand(time(NULL));
    menorCantidadAutos = 0;
    printf("Autos listo. Enter para empezar.\n");
    getchar();

    while (1)
    {
        espera_semaforo(id_semaforo);

        menorCantidadAutos = memoria[0].cantidadAutos;/*le asigno el valor de la primera via*/
        viaSeleccionada = 0;/*le asigno el numero de la primera via*/


        /*busco el menor*/
        for ( i = 1; i < CANTIDAD_DE_VIAS; i++)
        {
            if (memoria[i].cantidadAutos < menorCantidadAutos)
            {
                menorCantidadAutos = memoria[i].cantidadAutos;
                viaSeleccionada = i;
            }
        }
        
        memoria[viaSeleccionada].cantidadAutos++;
        printf("Auto paso por la via %d\n", memoria[viaSeleccionada].numeroVia);

        levanta_semaforo(id_semaforo);
        tiempoEspera = (rand() % (MAX_TIEMPO_AUTOS - MIN_TIEMPO_AUTOS)) + MIN_TIEMPO_AUTOS;
        usleep(tiempoEspera*1000);
    }
    shmdt ((char *)memoria);
	shmctl (id_memoria, IPC_RMID, (struct shmid_ds *)NULL);

    return 0;
}


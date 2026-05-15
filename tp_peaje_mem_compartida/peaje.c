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

    int id_memoria;
    int i;
    int buffer;
    int CANTIDAD_DE_VIAS;
    int tiempoEspera;
    char nombreArchivo[LARGO];
    char szBuffer[LARGO];
    int actualVia;

    
    
    via *memoria = NULL;
    

    /*ingreso de vias */
    if(argc != 2)
		{
			printf("Ingrese LA CANTIDAD DE VIAS DEL PEAJE\n");
			exit(1);
		}
    
    CANTIDAD_DE_VIAS = atoi(argv[1]);
    if (CANTIDAD_DE_VIAS <= 0)
    {
        printf("La cantidad de vias debe ser un número positivo.\n");
        exit(1);
    }
    

    id_semaforo = creo_semaforo();
    inicia_semaforo(id_semaforo, VERDE);

    memoria = (via*)creo_memoria(sizeof(via)*CANTIDAD_DE_VIAS, &id_memoria);

    /*creo el total de las vias q le pase pre while*/
    for ( i = 0; i < CANTIDAD_DE_VIAS; i++)
    {
        memoria[i].cantidadAutos = 0;
        memoria[i].numeroVia = i+1;
    }
    crearArchivoSincronismo(FILE_SINCRONISMO, 1);
    
    
    actualVia = 0;
    buffer = 0;


    srand(time(NULL));
    memset(nombreArchivo,0x00,sizeof(nombreArchivo));
    sprintf(nombreArchivo, "peajesLiberados.dat");
    actualizarArchivo(nombreArchivo, buffer);

    

    printf("PEAJE listo. Enter para empezar.\n");
    getchar();

    while (1)
    {
        espera_semaforo(id_semaforo);

        

        /*leo el archivo  para saber la cantidad de peajes liberados*/
        
        if (abrirArchivo(nombreArchivo,"r") == TRUE)
        {
            if (leerArchivo(szBuffer) == TRUE)
            {
                sscanf(szBuffer, "%d\n", &buffer);
                printf("Peajes liberados por ahora son: %d\n", buffer);
            }
            cerrarArchivo();
        }
        
        /*RECORRO LAS VIAS*/

        for (i = 0; i < CANTIDAD_DE_VIAS; i++)
        {
            printf("Via %d tiene %d autos en cola\n", memoria[i].numeroVia, memoria[i].cantidadAutos);
            
            }
        
        if (memoria[actualVia].cantidadAutos > LIMITE_LIBERACION)/*10 ES el limite*/
        {
            printf("Via %d paso un lote de vehiculos\n", memoria[actualVia].numeroVia);
            memoria[actualVia].cantidadAutos = 0;

            buffer++;
        
            actualizarArchivo(nombreArchivo, buffer);
            printf("Peajes liberados por ahora son: %d\n", buffer);
            
        }
        else if (memoria[actualVia].cantidadAutos > 0)
        {
            printf("Via %d paso un vehiculo\n", memoria[actualVia].numeroVia);
            memoria[actualVia].cantidadAutos--;
        }
    
        actualVia++;
        if (actualVia >= CANTIDAD_DE_VIAS)
        {
            actualVia = 0;

        }

        levanta_semaforo(id_semaforo);
        tiempoEspera = (rand() % (MAX_TIEMPO_PEAJE - MIN_TIEMPO_PEAJE)) + MIN_TIEMPO_PEAJE;
        usleep(tiempoEspera*1000);
    }

    shmdt ((char *)memoria);
	shmctl (id_memoria, IPC_RMID, (struct shmid_ds *)NULL);


    return 0;
}


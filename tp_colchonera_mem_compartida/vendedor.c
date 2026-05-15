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
    colchon *memoria = NULL;
    int id_memoria;
    int codigoAux;
    int cantidadAux;
    int indice;/*para q me posicione bien el array*/

    
    
    id_semaforo = creo_semaforo();
    memoria = (colchon*)creo_memoria(sizeof(colchon)*5, &id_memoria);

    esperarArchivoSincronismo(FILE_SINCRONISMO, 1);
    printf("Vendedor listo. Enter para empezar.\n");
    getchar();
    while (1)
    {
        espera_semaforo(id_semaforo);

        printf("Vendedor revisa la colchonera...\n");

        codigoAux = cargarInt("Ingrese codigo de colchon a vender (0 para terminar):");
        
        if (codigoAux == 0)
        {
            printf("Vendedor se retira.\n");
            memoria[0].codigo = -1; /*para que el repositor sepa q el vendedor se retiro*/
            levanta_semaforo(id_semaforo);
            usleep(1000*1000);
            break;
        }

        indice = codigoAux - 1; /*para q me posicione bien el array*/
        if (indice < 0 || indice > 4)
        {
            printf("Codigo de colchon invalido.\n");
        }else
        {
            printf("Colchon seleccionado: Codigo: %d, Cantidad: %d, Nombre: %s\n", memoria[indice].codigo, memoria[indice].cantidad, memoria[indice].nombre);
            if (memoria[indice].cantidad > 1)
            {
                cantidadAux = cargarInt("Ingrese cantidad de colchones a vender (max 5): ");
                if (cantidadAux >= 1 && cantidadAux <= 5 && memoria[indice].cantidad >= cantidadAux)
                {
                    memoria[indice].cantidad -= cantidadAux;
                    printf("Vendido %d colchones del modelo %s. Quedan %d disponibles.\n", cantidadAux, memoria[indice].nombre, memoria[indice].cantidad);
                }
                
            }
            
        
        
        }
        levanta_semaforo(id_semaforo);
        usleep(1500*1000);

    }
    
    shmdt ((char *)memoria);
	shmctl (id_memoria, IPC_RMID, (struct shmid_ds *)NULL);

    return 0;
}

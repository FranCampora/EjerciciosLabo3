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
    int opcion = 0;
    int id_memoria;
    int contadorColchones = 0;
    int i;
    int faltante;
    char *nombres[LARGO] = {"PIERO", "SUAVESTAR", "CANON", "SIMMONS", "BELMO"};

    id_semaforo = creo_semaforo();
    inicia_semaforo(id_semaforo, VERDE);

    memoria = (colchon*)creo_memoria(sizeof(colchon)*5, &id_memoria);

    crearArchivoSincronismo(FILE_SINCRONISMO, 1);
    
    printf("colchonera lista. Enter para empezar.\n");
    getchar();

    printf("cargar datos de colchonera\n");
    while (opcion != 3 && contadorColchones < 5)
    {
        printf("1. proximo colchon a cargar: %s con el codigo %d\n", nombres[contadorColchones], contadorColchones + 1);
        printf("2.  Ver colchones disponibles\n");
        printf("3.  Salir\n");

        opcion = cargarInt("Ingrese una opcion: ");
        

        if (opcion == 1)
        {
            memoria[contadorColchones].codigo = contadorColchones + 1;
            memoria[contadorColchones].cantidad = 10;
            
            /*cargarData("Ingrese nombre de colchon: ", memoria[contadorColchones].nombre);*/
            strcpy(memoria[contadorColchones].nombre, nombres[contadorColchones]);
            
            printf("Colchon %d cargado: Codigo: %d, Cantidad: %d, Nombre: %s\n", contadorColchones+1, memoria[contadorColchones].codigo, memoria[contadorColchones].cantidad, memoria[contadorColchones].nombre);
            contadorColchones++;
        }else if (opcion == 2)
        {
            if (contadorColchones == 0)
            {
                printf("No hay colchones disponibles.\n");
            }else
            {
                printf("Colchones disponibles:\n");
                for (i = 0; i < contadorColchones; i++)
                {
                    printf("Codigo: %d, Cantidad: %d, Nombre: %s\n", memoria[i].codigo, memoria[i].cantidad, memoria[i].nombre);
                }
            }
            
            
        }
        
        
    }
    

    while (1)
    {
        espera_semaforo(id_semaforo);

        if (memoria[0].codigo == -1)
        {
            levanta_semaforo(id_semaforo);
            usleep(800*1000);
            printf("Vendedor se retiro, repositor se retira tambien.\n");
            break;
        }
        
        for(i = 0; i < contadorColchones; i++)
        {
            if (memoria[i].cantidad < 10)
            {
                faltante = 10 - memoria[i].cantidad;
                memoria[i].cantidad += faltante; /*para q me muestre la cantidad correcta*/
                printf("El colchon con Codigo: %d y Nombre: %s, ahora tiene %d unidades disponibles.\n", memoria[i].codigo, memoria[i].nombre, memoria[i].cantidad);
            }
            
        }
        
        
        levanta_semaforo(id_semaforo);
        usleep(800*1000);

    }
    
    /*shmdt ((char *)memoria);
	shmctl (id_memoria, IPC_RMID, (struct shmid_ds *)NULL);*/

    return 0;
}

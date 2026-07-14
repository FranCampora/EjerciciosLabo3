#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

#include "def.h"
#include "global.h"
#include "semaforo.h"
#include "archivos.h"
#include "funciones.h"

int main(int argc, char  *argv[])
{
    int id_semaforo;
    char nombreArchivo[LARGO];
    char buffer_lectura[LARGO];
    char buffer_escritura[LARGO];
    int numero_proceso;
    int leido;
    int unidades_medicamento;
    
    

    int cant_productos_distintos;


    memset(buffer_lectura,   0x00, sizeof(buffer_lectura));
    memset(buffer_escritura, 0x00, sizeof(buffer_escritura));
    memset(nombreArchivo,0x00,sizeof(nombreArchivo));
    
    numero_proceso = 0;
    cant_productos_distintos = 0;
    leido = 0;
    unidades_medicamento = 0;

    if(argc != 2)
		{
			printf("Ingrese el numero del proceso (1, 2 o 3)\n");
			exit(1);
		}

    numero_proceso = atoi(argv[1]);

    if (numero_proceso < 1 || numero_proceso > MAX_PROCESOS)
    {
        printf("Numero de caja invalido. Debe ser 1, 2 o 3.\n");
        exit(1);
    }

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    id_semaforo = creo_semaforo();

    printf("Proceso %d: listo. Enter para empezar...\n", numero_proceso);
    getchar();

     /* armar el nombre del archivo de esta caja (no cambia nunca) */
    memset(nombreArchivo, 0x00, sizeof(nombreArchivo));
    sprintf(nombreArchivo, "caja%d.dat", numero_proceso);


    while (1)
    {
        espera_semaforo(id_semaforo);
        memset(buffer_lectura, 0x00, sizeof(buffer_lectura));
    
        if (abrirArchivo(nombreArchivo,"r") == TRUE)
        {
            if (leerArchivo(buffer_lectura) == TRUE)
            {
                
                sscanf(buffer_lectura, "%d|%d",&leido,&unidades_medicamento);

            }
            cerrarArchivo();
        }
        if (leido == SENIAL_FIN)
        {
            printf("Proceso %d: recibio senal de fin. Saliendo...\n", numero_proceso);
            levanta_semaforo(id_semaforo); /* libera el semaforo para los demas */
            usleep(2000*1000);
            break;
        }
        
        else if (leido == NO_LEIDO)
        {
            printf("\n*** CAJA %d: nuevo comprador ***\n", numero_proceso);
                    printf("Cantidad de productos : %d\n", unidades_medicamento);
                    
                    /* marcar como leido en el archivo */
                    leido = LEIDO;
            
            memset(buffer_escritura, 0x00, sizeof(buffer_escritura));
            sprintf(buffer_escritura, "%d|%d\n",leido,unidades_medicamento);
        
            if (abrirArchivo(nombreArchivo, "w") == TRUE)
            {
                escribirArchivo(buffer_escritura);
                cerrarArchivo();
            }
            else
            {
                printf("Caja %d: error al marcar como leido.\n", numero_proceso);
            }
        
        }
        

        

        printf("Soy el proceso %d\n",numero_proceso);
        levanta_semaforo(id_semaforo);
        usleep(2000*1000);
    }
    
    
    return 0;
}

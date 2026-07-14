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
    int done;
    int opcion;
    int leido;
    
    int num_lote_bebidas;
    int cantidad_bebidas;
    int alcohol;
    char nombre_bebida[LARGO];
    char descripcion_bebida[LARGO];



    memset(buffer_lectura,0x00,sizeof(buffer_lectura));
    memset(buffer_escritura,0x00,sizeof(buffer_escritura));
    memset(nombreArchivo,0x00,sizeof(nombreArchivo));
    numero_proceso = 0;
    num_lote_bebidas = 0;
    cantidad_bebidas = 0;
    alcohol = 0;
    memset(nombre_bebida,0x00,sizeof(nombre_bebida));
    memset(descripcion_bebida,0x00,sizeof(descripcion_bebida));
    done = 0;
    opcion = 0;
    leido = 0;

    if(argc != 2)
		{
			printf("Ingrese el numero del proceso (1, 2 o 3)\n");
			exit(1);
		}

    numero_proceso = atoi(argv[1]);

    if (numero_proceso < 1 || numero_proceso > MAX_CONTROLES)
    {
        printf("Numero de caja invalido. Debe ser 1 (10 bebidas), 2 (20 bebidas) o 3 (30 bebidas)\n");
        exit(1);
    }

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    id_semaforo = creo_semaforo();

    printf("Proceso %d: listo. Enter para empezar...\n", numero_proceso);
    getchar();

     /* armar el nombre del archivo de esta caja (no cambia nunca) */
    memset(nombreArchivo, 0x00, sizeof(nombreArchivo));
    sprintf(nombreArchivo, "control%d.dat", numero_proceso);


    while (done == 0)
    {
        espera_semaforo(id_semaforo);

        
        leido = LEIDO;                            
        
        memset(nombre_bebida, 0x00, sizeof(nombre_bebida));
        memset(descripcion_bebida, 0x00, sizeof(descripcion_bebida));
        
        memset(buffer_lectura, 0x00, sizeof(buffer_lectura));




        if (abrirArchivo(nombreArchivo,"r") == TRUE)
        {
            if (leerArchivo(buffer_lectura) == TRUE)
            {
                memset(nombre_bebida, 0x00, sizeof(nombre_bebida));
                memset(descripcion_bebida, 0x00, sizeof(descripcion_bebida));
                sscanf(buffer_lectura, "%d|%d|%d|%d|%[^|]|%[^\n]",&leido, &num_lote_bebidas, &cantidad_bebidas, &alcohol, nombre_bebida, descripcion_bebida);
            }
            cerrarArchivo();
        }
        if (leido == NO_LEIDO)/*para no volver a mostrar datos ya leidos*/
        {
            printf("Nuevo lote de bebidas detectado en proceso %d\n", numero_proceso);
            printf("Numero de lote: %d\n", num_lote_bebidas);
            printf("Cantidad de bebidas: %d\n", cantidad_bebidas);
            printf("Nombre de la bebida: %s\n", nombre_bebida);
            printf("Descripcion de la bebida: %s\n", descripcion_bebida);
            if (alcohol == 1)
            {
                printf("La bebida tiene alcohol\n");
            }else
            {
                printf("La bebida no tiene alcohol\n");
            }
            
            /*tengo que escribir en el archivo que ahora tiene leido*/
            memset(buffer_escritura, 0x00, sizeof(buffer_escritura));
            sprintf(buffer_escritura, "%d|%d|%d|%d|%s|%s\n", LEIDO, num_lote_bebidas, cantidad_bebidas, alcohol, nombre_bebida, descripcion_bebida);
            if (abrirArchivo(nombreArchivo, "w") == TRUE)
            {
                escribirArchivo(buffer_escritura);
                cerrarArchivo();
                
            }
        }
        

        levanta_semaforo(id_semaforo);
        usleep(1000*1000);
    }
    
    
    return 0;
}

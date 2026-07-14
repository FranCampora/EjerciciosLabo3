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
    int numero_proceso;
    char nombre_archivo[LARGO];
    char buffer_lectura[LARGO];
    char buffer_escritura[LARGO];

    tproducto comprador; /* usamos el struct para guardar lo leido del archivo */

    int cant_productos_distintos;


    memset(nombre_archivo,   0x00, sizeof(nombre_archivo));
    memset(buffer_lectura,   0x00, sizeof(buffer_lectura));
    memset(buffer_escritura, 0x00, sizeof(buffer_escritura));
    memset(&comprador,       0x00, sizeof(tproducto));
    
    numero_proceso = 0;
    cant_productos_distintos = 0;

    
    if(argc != 2)
		{
			printf("Ingrese el numero del proceso (1, 2 o 3)\n");
			exit(1);
		}

    numero_proceso = atoi(argv[1]);
    
    if (numero_proceso < 1 || numero_proceso > MAX_CAJAS)
    {
        printf("Numero de caja invalido. Debe ser 1, 2 o 3.\n");
        exit(1);
    }

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    id_semaforo = creo_semaforo();

    printf("Proceso %d: listo. Enter para empezar...\n", numero_proceso);
    getchar();

    /* armar el nombre del archivo de esta caja (no cambia nunca) */
    memset(nombre_archivo, 0x00, sizeof(nombre_archivo));
    sprintf(nombre_archivo, "caja%d.dat", numero_proceso);

    
    while (1)
    {
        espera_semaforo(id_semaforo);
        printf("Soy el proceso %d\n",numero_proceso);

        memset(buffer_lectura, 0x00, sizeof(buffer_lectura));
        memset(&comprador,     0x00, sizeof(tproducto));
        cant_productos_distintos = 0;
        
        if (abrirArchivo(nombre_archivo, "r") == TRUE)
        {
            if (leerArchivo(buffer_lectura) == TRUE)
            {
                /* parsear: formato "<leido>|<precio_total>|<cant_productos_distintos>" */
                memset(buffer_lectura, 0x00, sizeof(buffer_lectura));
                sscanf(buffer_lectura, "%d|%d|%d",&comprador.leido,&comprador.precio_producto,&cant_productos_distintos);

                cerrarArchivo();
                
                if (comprador.leido == NO_LEIDO)
                {
                    /* nuevo comprador: mostrar precio */
                    printf("\n*** CAJA %d: nuevo comprador ***\n", numero_proceso);
                    printf("Cantidad de productos : %d\n", cant_productos_distintos);
                    printf("PRECIO TOTAL A PAGAR  : $%d\n", comprador.precio_producto);

                    /* marcar como leido en el archivo */
                    comprador.leido = LEIDO;

                    memset(buffer_escritura, 0x00, sizeof(buffer_escritura));
                    sprintf(buffer_escritura, "%d|%d|%d\n",
                            comprador.leido,
                            comprador.precio_producto,
                            cant_productos_distintos);

                    if (abrirArchivo(nombre_archivo, "w") == TRUE)
                    {
                        escribirArchivo(buffer_escritura);
                        cerrarArchivo();
                    }
                    else
                    {
                        printf("Caja %d: error al marcar como leido.\n", numero_proceso);
                    }
                }
                else
                {
                    printf("Caja %d: sin nuevos compradores.\n", numero_proceso);
                }
            }
            else
            {
                /* archivo existe pero esta vacio */
                cerrarArchivo();
                printf("Caja %d: archivo vacio, esperando...\n", numero_proceso);
            }
        }
        else
        {
            /* archivo todavia no fue creado por proceso1 */
            printf("Caja %d: archivo %s no encontrado, esperando...\n",numero_proceso, nombre_archivo);
        }
        
        levanta_semaforo(id_semaforo);
        usleep(2000*1000);
    }
    
    
    return 0;
}

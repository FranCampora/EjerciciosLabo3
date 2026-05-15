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

int main(int argc, char *argv[])
{
    int id_semaforo;

    char buffer[LARGO];
    int numeroCajero;
    int montoTotal;

    Producto productoLeido;/*el struct de Producto esta en def.h*/

    /*chekeo de args para el numero del cajero*/
    if(argc != 2)
		{
			printf("Ingrese el numero del cajero (1 o 2)\n");
			exit(1);
		}

    numeroCajero = atoi(argv[1]);/*lo hago int*/

    if (numeroCajero < 1 || numeroCajero > 2)
    {
        printf("Numero de cajero invalido.\n");
        exit(1);
    }

    id_semaforo = creo_semaforo();


    printf("Cajero %d listo. Enter para comenzar a trabajar.\n", numeroCajero);
    getchar();

    while (1)
    {
        
        espera_semaforo(id_semaforo);
        /*logica*/
        if (abrirArchivo(NOMBRE_FILE, "r") == TRUE){
            if (leerArchivo(buffer) == TRUE)
            {
                /*1ro precio, cant, descripcion*/
                /*en el file de buffer esta escrito tal cual con los guiones (-) */
                sscanf(buffer, "%d-%d-%s\n", &productoLeido.precio, &productoLeido.cantidad, productoLeido.descripcion);
                /*primero leo nu cajero y despues depende la cant y numero de cajero es el caso del if*/
                /*DEPENEDE EL VALOR TOTAL OSEA PRODUCTO * CANTIDAD ES LA CAJA QUE DECIDE*/
                montoTotal = (productoLeido.precio * productoLeido.cantidad);

                if (numeroCajero == 1 && montoTotal < UMBRAL_PRECIO)
                {
                    printf("Cajero: %d, Producto: %s, Cantidad: %d, Precio: %d, Monto total: %d\n", numeroCajero, productoLeido.descripcion, productoLeido.cantidad, productoLeido.precio, montoTotal);
            
                }
                else if (numeroCajero == 2 && montoTotal >= UMBRAL_PRECIO)
                {
                    printf("Cajero: %d, Producto: %s, Cantidad: %d, Precio: %d, Monto total: %d\n", numeroCajero, productoLeido.descripcion, productoLeido.cantidad, productoLeido.precio, montoTotal);
                }
            } 
            cerrarArchivo();
        } else {
            printf("esperando el archivo del cajero %d\n", numeroCajero);
        }
        /*sale logica*/
        levanta_semaforo(id_semaforo);
        usleep(500*1000);/*osea 500ms*/
    }   
    /*usleep(2000*1000);*/
    return 0;
}

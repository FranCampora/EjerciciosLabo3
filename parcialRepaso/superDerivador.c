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

    
    char nombreArchivo[LARGO];
    char buffer[LARGO];

    Producto productoIngresar;
    
    id_semaforo = creo_semaforo();
    inicia_semaforo(id_semaforo, VERDE);/*semaforo en 1*/
    while (1)
    {
    
        /*carga accediendo a los datos del obj llamado productoIngresar*/
        printf("Va a ingresar un producto \n");
        cargarProducto("Ingrese producto a cargar: \n", productoIngresar.descripcion);
        productoIngresar.precio = cargarInt("Ingrese precio a cargar: \n");
        productoIngresar.cantidad = cargarInt("Ingrese cantidad a cargar: \n");

        memset(nombreArchivo,0x00,sizeof(nombreArchivo));
        sprintf(buffer, "%d-%d-%s\n", productoIngresar.precio, productoIngresar.cantidad, productoIngresar.descripcion);
        /*lo escribo con guiones (-) asi despues lo puedo leer bien en caja.c*/
        
        espera_semaforo(id_semaforo);/*xq voy a abrir el file*/

        if (abrirArchivo(NOMBRE_FILE, "w") == TRUE)
        {
            if (escribirArchivo(buffer) == TRUE)
            {
                printf("Producto cargado bien.\n");
            }else
            {
                printf("Error al escribir el producto.\n");
            }
            cerrarArchivo();
            
        }else
        {
            printf("Error al abrir el archivo para escribir el producto.\n");
        }
        levanta_semaforo(id_semaforo);
    }
    
    usleep(2000*1000);
    return 0;
}

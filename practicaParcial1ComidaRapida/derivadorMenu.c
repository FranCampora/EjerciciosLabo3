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

    Producto productoIngresar;
    
    id_semaforo = creo_semaforo();
    inicia_semaforo(id_semaforo, VERDE);/*semaforo en 1*/
    
    printf("Super Derivador listo. Enter para comenzar a cargar menus.\n");
    getchar();
    while (1)
    {
        espera_semaforo(id_semaforo);
        /*logica*/
        printf("Va a ingresar un menu \n");
        cargarProducto("Ingrese menu a cargar (A,B,C): \n", productoIngresar.menu);



        /*
        while (productoIngresar.menu[1] != '\0' || (productoIngresar.menu[0] != 'A' && productoIngresar.menu[0] != 'B' && productoIngresar.menu[0] != 'C'))
        {
            printf("Menu invalido. Intente de nuevo.\n");
            cargarProducto("Ingrese menu a cargar (A,B,C): \n", productoIngresar.menu);
        }*/
        
        productoIngresar.precio = cargarInt("Ingrese precio del menu: \n");
        productoIngresar.postre = cargarInt("Ingrese 1 si desea postre, 0 si no: \n");


        memset(buffer,0x00,sizeof(buffer));
        sprintf(buffer, "%s-%d-%d\n", productoIngresar.menu, productoIngresar.precio, productoIngresar.postre);
        /*lo escribo con guiones (-) asi despues lo puedo leer bien en caja.c*/
        
        

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
        /*sale logica*/
        levanta_semaforo(id_semaforo);
        usleep(2000*1000);
    }
    return 0;
}

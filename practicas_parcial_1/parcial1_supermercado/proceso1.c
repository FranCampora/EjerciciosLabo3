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

/*el proceso 1 es el panel*/
int main(int argc, char  *argv[])
{
    int id_semaforo;
    int i;
    int opcion;
    int cant_productos_distintos;   /* cuantos productos distintos cargo el comprador */
    int totalUnidades;   /* suma de unidades_producto de todos los productos */
    int precioTotal;     /* suma de precio*cantidad de todos los productos */
    int numeroCaja;      /* caja asignada: 1, 2 o 3 */
    char nombre_archivo[LARGO];
    char buffer[LARGO];

    tproducto productos[100];

    i = 0;
    opcion= 0;
    cant_productos_distintos = 0;
    totalUnidades = 0;
    precioTotal= 0;
    numeroCaja= 0;
    

    memset(buffer,         0x00, sizeof(buffer));
    memset(nombre_archivo, 0x00, sizeof(nombre_archivo));
    memset(productos,      0x00, sizeof(productos));

    id_semaforo = creo_semaforo();
    inicia_semaforo(id_semaforo,VERDE);
    crearArchivoSincronismo(FILE_SINCRONISMO,1);

    printf("Proceso ... LISTO PARA EMPEZAR. Enter para empezar...\n");
    getchar();

    while (1)
    {
        espera_semaforo(id_semaforo);
        /*sc*/
        cant_productos_distintos = 0;
        totalUnidades = 0;
        precioTotal   = 0;
        memset(productos, 0x00, sizeof(productos));
        
        printf("Soy el proceso super derivador\n");
        /*seleccionar opcion*/
        opcion = menu();
        while (opcion != 0)
            {
                /*queda loop cargando datos hasta que eliga la opcion de salir*/
                /*cargar producto*/
                cargarData("Ingrese descripcion del producto: ",
                        productos[cant_productos_distintos].descripcion_producto);

                productos[cant_productos_distintos].precio_producto =
                    cargarInt("Ingrese precio unitario del producto: ");

                productos[cant_productos_distintos].unidades_producto =
                    cargarInt("Ingrese cantidad del producto: ");

                productos[cant_productos_distintos].leido = NO_LEIDO;

                cant_productos_distintos++;/*muevo el array de productos*/
                printf("Producto agregado. Total de productos distintos cargados: %d\n", cant_productos_distintos);

                opcion = menu();
            }
        
        if (cant_productos_distintos == 0)
        {
            printf("No se cargaron productos. Volviendo a empezar...\n");
            levanta_semaforo(id_semaforo);
            usleep(1000);
            continue;
        }

        
        
        /* calcular totales recorriendo el array */
        totalUnidades = 0;/*contando todos los productos*/
        precioTotal   = 0;/*de toda la compra*/
        for (i = 0; i < cant_productos_distintos; i++)
        {
            totalUnidades = totalUnidades + productos[i].unidades_producto;
            precioTotal   = precioTotal   + (productos[i].precio_producto * productos[i].unidades_producto);
        }

        /* determinar la caja segun el total de unidades */
        if (totalUnidades >= 1 && totalUnidades <= 5)
        {
            numeroCaja = 1;
        }
        else if (totalUnidades >= 6 && totalUnidades <= 10)
        {
            numeroCaja = 2;
        }
        else
        {
            numeroCaja = 3;
        }

        printf("Total unidades: %d -> Caja %d | Precio total: $%d\n",
            totalUnidades, numeroCaja, precioTotal);

        /* escribir en el archivo de la caja correspondiente */
        memset(nombre_archivo, 0x00, sizeof(nombre_archivo));
        sprintf(nombre_archivo, "caja%d.dat", numeroCaja);

        /* formato: <leido>|<precio_total>|<total_unidades> */
        memset(buffer, 0x00, sizeof(buffer));
        sprintf(buffer, "%d|%d|%d\n", NO_LEIDO, precioTotal, totalUnidades);
            
        
        /*escribe en el file que va*/

        if (abrirArchivo(nombre_archivo, "w") == TRUE)
        {
            if (escribirArchivo(buffer) == TRUE)
            {
                printf("Datos escritos en %s\n", nombre_archivo);
            }
            else
            {
                printf("Error al escribir en %s\n", nombre_archivo);
            }
            cerrarArchivo();
        }
        else
        {
            printf("Error al abrir %s\n", nombre_archivo);
        }

        /*se va sc*/
        levanta_semaforo(id_semaforo);
        usleep(8000*1000);
    }
    

    return 0;
}

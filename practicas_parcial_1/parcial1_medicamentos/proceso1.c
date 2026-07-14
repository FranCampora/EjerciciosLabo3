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
    char nombreArchivo[LARGO];
    char buffer[LARGO];
    int i;
    int opcion;
    int cant_productos_distintos;
    int totalUnidades;
    int numero_proceso;
    int done;
    

    tmedicamento medicamentos[100];
    /*tproducto productos[100]*/


    i = 0;
    opcion = 0;
    done = 0;
    cant_productos_distintos = 0;
    totalUnidades = 0;
    numero_proceso = 0;
    memset(buffer,0x00,sizeof(buffer));
    memset(nombreArchivo,0x00,sizeof(nombreArchivo));


    id_semaforo = creo_semaforo();
    inicia_semaforo(id_semaforo,VERDE);
    crearArchivoSincronismo(FILE_SINCRONISMO,1);

    printf("Proceso ... LISTO PARA EMPEZAR. Enter para empezar...\n");
    getchar();

    while (done == 0)
    {
        espera_semaforo(id_semaforo);
        /*sc*/

        printf("Soy el proceso base\n");

        opcion = menu();
        if (opcion == 1)/*cargar el medicamento*/
        {
            medicamentos[cant_productos_distintos].color_medicamento = cargarInt("Ingrese el color del medicamento (1-Rojo,2-Amarillo,3-Fucsia): ");
            medicamentos[cant_productos_distintos].cantidad_medicamento = cargarInt("Ingrese cantidad de pastillas (5,10 ó 20): ");
            
            while (medicamentos[cant_productos_distintos].cantidad_medicamento != 5 && medicamentos[cant_productos_distintos].cantidad_medicamento != 10 && medicamentos[cant_productos_distintos].cantidad_medicamento != 20)
            {
                printf("Cantidad incorrecta\n");
                medicamentos[cant_productos_distintos].cantidad_medicamento = cargarInt("Ingrese cantidad de pastillas (5,10 ó 20): ");
            }
            
            cargarData("Ingrese descripcion del medicamento: ",medicamentos[cant_productos_distintos].descripcion_medicamento);

            medicamentos[cant_productos_distintos].leido = NO_LEIDO;

            cant_productos_distintos++;
            printf("Medicamento cargado\n");
        /*}else if (opcion == 2)procesar*/
            /*{*/
                if (cant_productos_distintos == 0 )
                {
                    printf("No se cargaron productos. Volviendo a empezar...\n");
                    
                    
                }else
                {
                    for (i = 0; i < cant_productos_distintos; i++)
                    {
                        totalUnidades = totalUnidades + medicamentos[i].cantidad_medicamento;
                    }
                    /*logica para decidier que caja/jugador*/
                    /* determinar la caja segun el total de unidades */
                    if (totalUnidades == 5)
                    {
                        numero_proceso = 1;
                    }
                    else if (totalUnidades == 10)
                    {
                        numero_proceso = 2;
                    }
                    else
                    {
                        numero_proceso = 3;
                    }

                    printf("Total unidades: %d -> Caja: %d\n",
                        totalUnidades, numero_proceso);
                    

                    /* escribir en el archivo de la caja correspondiente */
                    memset(nombreArchivo, 0x00, sizeof(nombreArchivo));
                    sprintf(nombreArchivo, "caja%d.dat", numero_proceso);

                    /*escribo las variables dentro del buffer que va a ir en el file*/
                    /* formato: <leido>|<total_unidades> */
                    memset(buffer, 0x00, sizeof(buffer));
                    sprintf(buffer, "%d|%d\n", NO_LEIDO, totalUnidades);
                        

                    /*escribe en el file que va*/
                    if (abrirArchivo(nombreArchivo, "w") == TRUE)
                    {
                        if (escribirArchivo(buffer) == TRUE)
                        {
                            printf("Datos escritos en %s\n", nombreArchivo);
                        }
                        else
                        {
                            printf("Error al escribir en %s\n", nombreArchivo);
                        }
                        cerrarArchivo();
                    }
                    else
                    {
                        printf("Error al abrir %s\n", nombreArchivo);
                    }
                    cant_productos_distintos = 0;
                    totalUnidades = 0;
                    memset(medicamentos, 0x00, sizeof(medicamentos));

                }    
        } if (opcion == 0)
        {
            done = 1;
        }
    
        levanta_semaforo(id_semaforo);
        usleep(2000*1000);
    }
        

    

    return 0;
}

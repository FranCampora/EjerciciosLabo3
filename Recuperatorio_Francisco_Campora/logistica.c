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
    
    char buffer_escritura[LARGO];
    
    int opcion;
    int numero_proceso;
    

    int cant_bebidas_distintas;
    int done;

    Bebidas bebidas_logistica[100];
    /*tproducto productos[100]*/


    
    opcion = 0;
    numero_proceso = 0;
    
    cant_bebidas_distintas = 0;
    done = 0;
    memset(buffer_escritura,0x00,sizeof(buffer_escritura));
    memset(nombreArchivo,0x00,sizeof(nombreArchivo));
    memset(bebidas_logistica, 0x00, sizeof(bebidas_logistica));

    id_semaforo = creo_semaforo();
    srand(time(NULL));
    inicia_semaforo(id_semaforo,VERDE);
    crearArchivoSincronismo(FILE_SINCRONISMO,1);

    printf("Proceso logistica /LOGIARG/ LISTO PARA EMPEZAR. Enter para empezar...\n");
    getchar();

    /*tal vez tengo que setear variables iniciales*/
    while (done == 0)
    {
        espera_semaforo(id_semaforo);
        /*sc*/

        

        opcion = menu();

        /*1 es para cargar manualmente*/
        if (opcion == 1)
        {
            bebidas_logistica[cant_bebidas_distintas].cantidad_bebidas = cargarInt("Ingrese la cantidad de bebidas (10,20 ó 30): ");
            while (bebidas_logistica[cant_bebidas_distintas].cantidad_bebidas != 10 && bebidas_logistica[cant_bebidas_distintas].cantidad_bebidas != 20 && bebidas_logistica[cant_bebidas_distintas].cantidad_bebidas != 30)
            {
                printf("Cantidad incorrecta\n");
                bebidas_logistica[cant_bebidas_distintas].cantidad_bebidas = cargarInt("Ingrese cantidad de bebidas (10,20 ó 30): ");
            }
            cargarData("Ingrese el nombre de la bebida: ", bebidas_logistica[cant_bebidas_distintas].nombre_bebida);
            cargarData("Ingrese la descripcion de la bebida: ", bebidas_logistica[cant_bebidas_distintas].descripcion_bebida);
            
            bebidas_logistica[cant_bebidas_distintas].alcohol = cargarInt("Ingrese si la bebida tiene alcohol (1-Si, 0-No): ");
            while (bebidas_logistica[cant_bebidas_distintas].alcohol != 1 && bebidas_logistica[cant_bebidas_distintas].alcohol != 0)
            {
                printf("Valor incorrecto, tiene que ser 1(tiene) o 0(no tiene)\n");
                
                bebidas_logistica[cant_bebidas_distintas].alcohol = cargarInt("Ingrese si la bebida tiene alcohol (1-Si, 0-No): ");
            }
            bebidas_logistica[cant_bebidas_distintas].numero_lote_bebidas = cant_bebidas_distintas + 1; /*le asigno un numero de lote segun la cantidad de bebidas distintas que tengo, arranco en 1*/
            bebidas_logistica[cant_bebidas_distintas].leido = NO_LEIDO;
            cant_bebidas_distintas++;
        }
        else if (opcion == 2)/*cargar aleatoriamente la cantidad de bebidas*/
        {
            bebidas_logistica[cant_bebidas_distintas].cantidad_bebidas = devolverNumAleatorio(10,30);
            while (bebidas_logistica[cant_bebidas_distintas].cantidad_bebidas != 10 && bebidas_logistica[cant_bebidas_distintas].cantidad_bebidas != 20 && bebidas_logistica[cant_bebidas_distintas].cantidad_bebidas != 30)
            {
                /*tiene que tirar si o si 10 20 o 30*/
                bebidas_logistica[cant_bebidas_distintas].cantidad_bebidas = devolverNumAleatorio(DESDE,HASTA);
            }
            cargarData("Ingrese el nombre de la bebida: ", bebidas_logistica[cant_bebidas_distintas].nombre_bebida);
            cargarData("Ingrese la descripcion de la bebida: ", bebidas_logistica[cant_bebidas_distintas].descripcion_bebida);
            
            bebidas_logistica[cant_bebidas_distintas].alcohol = cargarInt("Ingrese si la bebida tiene alcohol (1-Si, 0-No): ");
            while (bebidas_logistica[cant_bebidas_distintas].alcohol != 1 && bebidas_logistica[cant_bebidas_distintas].alcohol != 0)
            {
                printf("Valor incorrecto, tiene que ser 1(tiene) o 0(no tiene)\n");
                bebidas_logistica[cant_bebidas_distintas].alcohol = cargarInt("Ingrese si la bebida tiene alcohol (1-Si, 0-No): ");
            }
            bebidas_logistica[cant_bebidas_distintas].numero_lote_bebidas = cant_bebidas_distintas + 1; /*le asigno un numero de lote segun la cantidad de bebidas distintas que tengo, arranco en 1*/
            
            bebidas_logistica[cant_bebidas_distintas].leido = NO_LEIDO;
            cant_bebidas_distintas++;
        }
        else if (opcion == 0)
        {
            done = 1;
            /*tiene que hacer levanta semaforo pero sin escribilo aca sino el que esta debajo*/
        }
        
        /*ya se cargaron todos los datos del struct ahora derivo a los distintos procesos*/
        if (cant_bebidas_distintas == 0)
        {
            printf("No se cargaron bebidas, arrancar de nuevo\n");
                    
        }else if (done == 0)/*si no se eligio salir, entonces tengo que derivar a los procesos*/
        {
            switch (bebidas_logistica[cant_bebidas_distintas - 1].cantidad_bebidas)
            {
            case 10:
                numero_proceso = 1;
                break;
            case 20:
                numero_proceso = 2;
                break;
            case 30:
                numero_proceso = 3;
                break;
            
            default:
                break;
            }
        }
        
        /*creo nombre de archivo*/
        memset(nombreArchivo, 0x00, sizeof(nombreArchivo));
        sprintf(nombreArchivo, "control%d.dat", numero_proceso);

        /*escribo las variables dentro del buffer que va a ir en el file*/
        memset(buffer_escritura, 0x00, sizeof(buffer_escritura));
        sprintf(buffer_escritura, "%d|%d|%d|%d|%s|%s\n", NO_LEIDO, bebidas_logistica[cant_bebidas_distintas - 1].numero_lote_bebidas, 
            bebidas_logistica[cant_bebidas_distintas - 1].cantidad_bebidas,bebidas_logistica[cant_bebidas_distintas-1].alcohol,
            bebidas_logistica[cant_bebidas_distintas - 1].nombre_bebida, bebidas_logistica[cant_bebidas_distintas - 1].descripcion_bebida);

        /*escribe en el file que va*/
    
        if (abrirArchivo(nombreArchivo, "w") == TRUE)
        {
            escribirArchivo(buffer_escritura);
            cerrarArchivo();
        }
        else
        {
            printf("Error al abrir %s\n", nombreArchivo);
        }





        /*se va sc*/
        levanta_semaforo(id_semaforo);
        usleep(2000*1000);
    }
    

    return 0;
}

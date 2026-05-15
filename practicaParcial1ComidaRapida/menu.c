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
    char opcionMenu;
    
    int montoCalculado;
    Producto productoLeido;/*el struct de Producto esta en def.h*/
    Historial historial;/*el struct de Historial esta en def.h*/
    
    historial.contador = 0;
    historial.totalRecaudado = 0;


    /*chekeo de args para el numero del cajero*/
    if(argc != 2)
		{
			printf("Ingrese la letra del menu (A,B,C)\n");
			exit(1);
		}
    
    opcionMenu = argv[1][0];/*agarra solo el primer caracter*/

    if (argv[1][1] != '\0' || (opcionMenu != 'A' && opcionMenu != 'B' && opcionMenu != 'C'))
    {
        printf("Opcion invalida. Para ejecutar el proceso el menu debe ser A, B o C\n");
        exit(1);
    }

    id_semaforo = creo_semaforo();


    printf("Menu %c listo. Enter para comenzar a trabajar.\n", opcionMenu);
    getchar();

    while (1)
    {
        espera_semaforo(id_semaforo);
        montoCalculado = 0;

        if (abrirArchivo(NOMBRE_FILE, "r") == TRUE) {
            if (leerArchivo(buffer) == TRUE) {
                sscanf(buffer, "%s-%d-%d\n", productoLeido.menu, &productoLeido.precio, &productoLeido.postre);
                
            
                if (productoLeido.menu[0] == opcionMenu) {
                    if (opcionMenu == 'A') montoCalculado = PRECIO_MENU_A;
                    else if (opcionMenu == 'B') montoCalculado = PRECIO_MENU_B;
                    else if (opcionMenu == 'C') montoCalculado = PRECIO_MENU_C;
                    
                    if (productoLeido.postre == 1) montoCalculado += 200;

                    if (montoCalculado > 0) {
                        historial.totalRecaudado += montoCalculado;
                        printf("Venta exitosa! Total actual: %d\n", historial.totalRecaudado);
                        
                        
                        cerrarArchivo();
                        abrirArchivo(NOMBRE_FILE, "w"); 
                    }
                }
            } 
            cerrarArchivo();
        }
        levanta_semaforo(id_semaforo);
        
    
        usleep(500 * 1000); 
    
    }   
    /*usleep(2000*1000);*/
    return 0;
}

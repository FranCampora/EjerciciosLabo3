/*cambiar def*/
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
    char buffer[LARGO];
    int numeroUsuario;
    Usuario usuarioLeido;/*el struct de Usuario esta en def.h*/
    int montoInvertido;
    char nombreArchivo[LARGO]; /*BUFFER para nombre de file*/

    /*chekeo de args para el numero del usuario*/
    if(argc != 2)
		{
			printf("Ingrese el numero del usuario (1, 2 o 3)\n");
			exit(1);
		}

    numeroUsuario = atoi(argv[1]);/*lo hago int*/
    
    id_semaforo = creo_semaforo();

    printf("Usuario %d listo. Enter para comenzar a trabajar.\n", numeroUsuario);
    getchar();

    while (1)
    {
        espera_semaforo(id_semaforo);
        /*sc*/
        
            cargarData("Ingrese nombre de usuario: ", usuarioLeido.nombre);
            cargarData("Ingrese apellido de usuario: ", usuarioLeido.apellido);
            

            memset(nombreArchivo,0x00,sizeof(nombreArchivo));
            sprintf(nombreArchivo, "%s-%s.dat", usuarioLeido.nombre, usuarioLeido.apellido );

            if (abrirArchivo(nombreArchivo, "r") == TRUE)
            {
                printf("Ganancia incial para el usuario\n");
                while (leerArchivo(buffer) == TRUE)
                {
                    printf("%s", buffer);
                    memset(buffer,0x00,sizeof(buffer));
                }
                cerrarArchivo();
            }
            
            montoInvertido = cargarInt("Ingrese monto a invertir: ");
                while (montoInvertido < MIN_INVERSION)
                {
                    printf("El monto a invertir debe ser mayor a %d\n", MIN_INVERSION);
                    montoInvertido = cargarInt("Ingrese monto a invertir: ");
                }

            memset(buffer,0x00,sizeof(buffer));
            sprintf(buffer, "%s-%s-%d\n", usuarioLeido.nombre, usuarioLeido.apellido, montoInvertido);

            if (abrirArchivo(NOMBRE_FILE, "a") == TRUE)
            {
                escribirArchivo(buffer);
                cerrarArchivo();
                printf("se cargo bien el user");
            }


        /*se va sc*/
        levanta_semaforo(id_semaforo);
        usleep(1000*1000);
        
    
    }
    return 0;
}

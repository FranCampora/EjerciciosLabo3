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
    int montoFinalCalculado;
    Usuario usuarios[MAX_USUARIOS]; /*array de usuarios para guardar los datos de cada uno*/
    int i;
    int cantidadUsuarios = 0;

    char nombreArchivo[LARGO]; /*BUFFER para nombre de file*/

    id_semaforo = creo_semaforo();
    inicia_semaforo(id_semaforo, VERDE);/*semaforo en 1*/

    printf("Entidad financiera lista. Enter para comenzar a trabajar.\n");
    getchar();
    while (1)
    {
        espera_semaforo(id_semaforo);
        /*sc*/
        montoFinalCalculado = 0;
        cantidadUsuarios = 0;
        memset(usuarios,0x00,sizeof(usuarios));
        

        if (abrirArchivo(NOMBRE_FILE,"r") == TRUE)
        {
            while (leerArchivo(buffer) == TRUE && cantidadUsuarios < MAX_USUARIOS)
            {
                if (sscanf(buffer, "%[^-]-%[^-]-%d\n", usuarios[cantidadUsuarios].nombre, usuarios[cantidadUsuarios].apellido, &usuarios[cantidadUsuarios].montoIncial) == 3)
                {
                    /*lo primero q lee es el nombre, -, lo q lee es el apellido, -, lo q lee es el monto inicial*/
                    printf("Usuario: %s %s, monto inicial: %d\n", usuarios[cantidadUsuarios].nombre, usuarios[cantidadUsuarios].apellido, usuarios[cantidadUsuarios].montoIncial);
                    cantidadUsuarios++;
                }
            }
            cerrarArchivo();
        }

            for ( i = 0; i < cantidadUsuarios; i++)
                {
                montoFinalCalculado = usuarios[i].montoIncial *2;
            
                printf("Monto final calculado para el usuario: %d\n", montoFinalCalculado);
                
                /*usuarios[i].montoFinal = montoFinalCalculado;*/
                /*reescribo en el file asi: nombre-apellido-montoInicial-montoFinal*/
                memset(nombreArchivo,0x00,sizeof(nombreArchivo));
                sprintf(nombreArchivo, "%s-%s.dat", usuarios[i].nombre, usuarios[i].apellido);/*creo el nombre del archivo*/
                
                memset(buffer,0x00,sizeof(buffer));
                sprintf(buffer, "%s-%s-%d-%d\n", usuarios[i].nombre, usuarios[i].apellido, usuarios[i].montoIncial, montoFinalCalculado);/*escribo con la inversion*/

                if (abrirArchivo(nombreArchivo, "a") == TRUE)
                {
                    if (escribirArchivo(buffer) == TRUE)
                    {
                        printf("Archivo de usuario %s %s actualizado con monto final.\n", usuarios[i].nombre, usuarios[i].apellido);
                    }else
                    {
                        printf("Error al escribir el archivo del usuario.\n");
                    }
                    cerrarArchivo();
            
                }

            
                
            }
            if (abrirArchivo(NOMBRE_FILE,"w")==TRUE)
                {
                    cerrarArchivo();/*borro el contenido del file buffer.dat para la proxima iteracion*/
                }

        /*se va sc*/
        levanta_semaforo(id_semaforo);
        usleep(2000*1000);
    }
    
    return 0;
}

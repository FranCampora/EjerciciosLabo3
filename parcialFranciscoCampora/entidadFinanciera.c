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
    
    Usuario usuarios[MAX_USUARIOS]; /*array de usuarios para guardar los datos de cada uno*/
    int i;
    
    int montoInversion;
    char nombreArchivo[LARGO]; /*BUFFER para nombre de file*/

    id_semaforo = creo_semaforo();
    inicia_semaforo(id_semaforo, VERDE);/*semaforo en 1*/

    printf("Entidad financiera lista. Enter para comenzar a trabajar.\n");
    getchar();

    while (1)
    {
        espera_semaforo(id_semaforo);
        /*sc*/
        for (i = 0; i < MAX_USUARIOS; i++)
        {
            sprintf(nombreArchivo,"usuario%d.dat",i+1);/*para pasar por los 3 users*/
            
            if (abrirArchivo(nombreArchivo,"r") == TRUE){
                if (leerArchivo(buffer) == TRUE)
                {
                    sscanf(buffer, "%[^-]-%[^-]-%d\n", usuarios[i].nombre, usuarios[i].apellido, &usuarios[i].montoIncial);/*leo el nombre, apellido y monto inicial del archivo de cada usuario*/
                    
                    montoInversion = usuarios[i].montoIncial *2;
                    
                    usuarios[i].montoFinal = montoInversion;/*calculo el monto final y lo guardo en la struct del user*/
                    
                    printf("Usuario: %s %s, monto inicial: %d, monto final calculado: %d\n", usuarios[i].nombre, usuarios[i].apellido, usuarios[i].montoIncial, usuarios[i].montoFinal);
                    
                    cerrarArchivo();


                    if (abrirArchivo(nombreArchivo,"w")== TRUE)
                    {
                        memset(buffer,0x00,sizeof(buffer));
                        sprintf(buffer, "%s-%s-%d\n", usuarios[i].nombre, usuarios[i].apellido, usuarios[i].montoFinal);/*escribo con la inversion*/

                        escribirArchivo(buffer);
                        cerrarArchivo();
                    }else
                    {
                        cerrarArchivo();
                        printf("Error al abrir el archivo del usuario para escribir.\n");
                    }
                    
                    

                }
                
            }
        }
        /*se va sc*/
        levanta_semaforo(id_semaforo);
        usleep(2000*1000);
    }
    
    return 0;
}

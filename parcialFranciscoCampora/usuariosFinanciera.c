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
    int montoInvertir;
    int montoActual;
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

    cargarData("Ingrese nombre de usuario: ", usuarioLeido.nombre);
    cargarData("Ingrese apellido de usuario: ", usuarioLeido.apellido);

    memset(nombreArchivo,0x00,sizeof(nombreArchivo));
    sprintf(nombreArchivo, "usuario%d.dat", numeroUsuario);


    while (1)
    {
        espera_semaforo(id_semaforo);
    
        /*LEER archivo para actualizar el monto actual, y buscar inversiones previas*/
        montoActual = 0;
        if (abrirArchivo(nombreArchivo,"r") == TRUE)/*tengo que chekear que si tiene ya una inversion cargada el user*/
        {
            if (leerArchivo(buffer) == TRUE)/*si hay ya en el archivo un monto invertido se muestra*/
            {
                sscanf(buffer, "%[^-]-%[^-]-%d\n",usuarioLeido.nombre, usuarioLeido.apellido, &montoActual);/*leo el nombre, apellido y monto del archivo*/
                
                printf("Data: %s %s, monto actual invertido: $%d\n", usuarioLeido.nombre, usuarioLeido.apellido, montoActual); /*monto del archivo que cargó la financiera*/
                
            }
            cerrarArchivo(); 
        }
        
        

        /*en todas las vueltas osea los años tiene que estar la opcion de invertir más*/
        montoInvertir = cargarInt("Ingrese el monto a invertir: "); /*Le pondria "montoInvertir"*/
        
        while (montoInvertir < MIN_INVERSION)
        {
            printf("El monto a invertir debe ser mayor a %d\n", MIN_INVERSION);
            montoInvertir = cargarInt("Ingrese el monto a invertir: ");
        }
        
        usuarioLeido.montoIncial = montoInvertir + montoActual; /*el monto a invertir es el monto que ya tenia invertido + lo que va a invertir ahora*/

        /*le paso la NUEVA data al buffer*/
        memset(buffer,0x00,sizeof(buffer));
        sprintf(buffer,"%s-%s-%d",usuarioLeido.nombre,usuarioLeido.apellido, usuarioLeido.montoIncial);
        
        /*escsribe la struct del user como monto inicial, porque el
            que calcula lo invertido es la entidad*/

        
        if (abrirArchivo(nombreArchivo,"w")==TRUE)
        {
            escribirArchivo(buffer);
            cerrarArchivo();
        }
        

        levanta_semaforo(id_semaforo);
        usleep(1000*1000);
        
    
    }
    return 0;
}

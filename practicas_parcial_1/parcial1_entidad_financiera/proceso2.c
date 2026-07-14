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
    char nombreArchivo[LARGO];
    char buffer_lectura[LARGO];
    char buffer_escritura[LARGO];
    int numero_proceso;
    
    int monto_old;
    int monto_ingresar;
    
    int done;
    int opcion;
    int leido;
    Usuario user;



    memset(buffer_lectura,0x00,sizeof(buffer_lectura));
    memset(buffer_escritura,0x00,sizeof(buffer_escritura));
    memset(nombreArchivo,0x00,sizeof(nombreArchivo));
    numero_proceso = 0;
    monto_ingresar = 0;
    monto_old = 0;
    done = 0;
    opcion = 0;
    leido = 0;
    

    if(argc != 2)
		{
			printf("Ingrese el numero del proceso (1, 2 o 3)\n");
			exit(1);
		}

    numero_proceso = atoi(argv[1]);

    if (numero_proceso < 1 || numero_proceso > MAX_USUARIOS)
    {
        printf("Numero de caja invalido. Debe ser 1, 2 o 3.\n");
        exit(1);
    }

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    id_semaforo = creo_semaforo();

    printf("Proceso %d: listo. Enter para empezar...\n", numero_proceso);
    getchar();

     /* armar el nombre del archivo de esta caja (no cambia nunca) */
    memset(nombreArchivo, 0x00, sizeof(nombreArchivo));
    sprintf(nombreArchivo, "caja%d.dat", numero_proceso);

    cargarData("Ingresar nombre del user: ", user.nombre_usuario);
    cargarData("Ingresar apellido del user: ", user.apellido_usuario);
    

    printf("DEBUG -> %s %s\n",
    user.nombre_usuario,
    user.apellido_usuario);

    while (done == 0)
    {
        espera_semaforo(id_semaforo);
        
        if (abrirArchivo(nombreArchivo,"r") == TRUE)
        {
            memset(buffer_lectura, 0x00, sizeof(buffer_lectura));
            if (leerArchivo(buffer_lectura) == TRUE)
            {
                
                sscanf(buffer_lectura, "%d|%d|%d",&leido, &numero_proceso, &monto_old);
                
            }
            
            cerrarArchivo();
        }
        
        opcion = menu();
        if (opcion == 1)/*cargar monto sumado al viejo*/
        {
            printf("Opcion cargar monto sumado al old\n");
            monto_ingresar = cargarInt("Ingresar monto a invertir: ");
            monto_ingresar = monto_old + monto_ingresar;

            if (abrirArchivo(nombreArchivo, "w") == TRUE)
            {
                /* antes de cada escribirArchivo, hacer el sprintf */
                memset(buffer_escritura, 0x00, sizeof(buffer_escritura));
                sprintf(buffer_escritura, "%d|%d|%d|%s|%s\n",NO_LEIDO, numero_proceso, monto_ingresar,
                        user.nombre_usuario, user.apellido_usuario);
                printf("VOY A ESCRIBIR -> %s %s\n",user.nombre_usuario,user.apellido_usuario);
                if (escribirArchivo(buffer_escritura) == TRUE)
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
        }else if (opcion == 2)/*cargar nuevo monto*/
        {
            printf("Opcion cargar monto nuevo\n");
            monto_ingresar = cargarInt("Ingresar monto a invertir: ");

            if (abrirArchivo(nombreArchivo, "w") == TRUE)
            {
                /* antes de cada escribirArchivo, hacer el sprintf */
                memset(buffer_escritura, 0x00, sizeof(buffer_escritura));
                sprintf(buffer_escritura, "%d|%d|%d|%s|%s\n",NO_LEIDO, numero_proceso, monto_ingresar,
                        user.nombre_usuario, user.apellido_usuario);
                /*printf("VOY A ESCRIBIR -> %s %s\n",user.nombre_usuario,user.apellido_usuario);*/
                if (escribirArchivo(buffer_escritura) == TRUE)
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
        }else if (opcion == 0)/*salir*/
        {
            done = 1;
            printf("Monto final de %d (%s - %s) es:$ %d\n",(numero_proceso+1),user.nombre_usuario,user.apellido_usuario,monto_old);
        }

        levanta_semaforo(id_semaforo);
        usleep(1000*1000);
        
        
        
    }
    
    
    return 0;
}

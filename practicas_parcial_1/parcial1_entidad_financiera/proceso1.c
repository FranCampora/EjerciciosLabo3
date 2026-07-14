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
    char buffer_lectura[LARGO];
    char buffer_escritura[LARGO];
    int i;
    int opcion;
    int numero_proceso;

    int monto_inicial;
    int nuevo_monto;
    int leido;
    
    Usuario user[MAX_USUARIOS];


    /*tproducto productos[100]*/


    i = 0;
    opcion = 0;
    numero_proceso = 0;
    monto_inicial = 0;
    nuevo_monto = 0;
    leido = 0;
    memset(buffer_lectura,0x00,sizeof(buffer_lectura));
    memset(buffer_escritura,0x00,sizeof(buffer_escritura));
    memset(nombreArchivo,0x00,sizeof(nombreArchivo));
    
    
    id_semaforo = creo_semaforo();
    
    inicia_semaforo(id_semaforo,VERDE);
    crearArchivoSincronismo(FILE_SINCRONISMO,1);

    printf("Proceso ... LISTO PARA EMPEZAR. Enter para empezar...\n");
    getchar();

    /*seteta de variables iniciales para cada proceso*/
        for ( i = 0; i < MAX_USUARIOS; i++)
        {
            user[i].numero_usuario = i+1;
            user[i].monto = 0;/*de base*/
            strcpy(user[i].nombre_usuario, "user");
            strcpy(user[i].apellido_usuario, "user");
        }

    while (1)
    {
        espera_semaforo(id_semaforo);
        /*sc*/

        

        for ( i = 0; i < MAX_USUARIOS; i++)
        {
            
            memset(nombreArchivo, 0x00, sizeof(nombreArchivo));
            sprintf(nombreArchivo, "caja%d.dat", i + 1);

            
            
            if (abrirArchivo(nombreArchivo,"r") == TRUE)
            {
                memset(buffer_lectura, 0x00, sizeof(buffer_lectura));
                if (leerArchivo(buffer_lectura) == TRUE)
                {
                    
                    sscanf(buffer_lectura,"%d|%d|%d|%[^|]|%[^\n]",&leido,&numero_proceso,&monto_inicial,user[i].nombre_usuario,user[i].apellido_usuario);
                    
                }
                cerrarArchivo();
            }
            printf("User numero %d\n",numero_proceso);
            user[i].numero_usuario = numero_proceso;
            user[i].monto = monto_inicial;

            /*si lee correctamente el proceso dentro del file, no entra aca*/
            /*sino va a imprimir algo que esta mal*/
            if (numero_proceso < 1 || numero_proceso > MAX_USUARIOS)
            {
                continue;/*vuelve a inciiar el for*/
            }
            /*escribe en el file que va*/
            if (leido == NO_LEIDO)
            {
                nuevo_monto = monto_inicial * 2;
                user[i].monto = nuevo_monto;/*actualizo monto en struct*/

                printf("Usuario(%s - %s) num %d invirtio: $%d | ganancia: $%d\n",user[i].nombre_usuario,user[i].apellido_usuario, numero_proceso, monto_inicial, nuevo_monto);

                memset(buffer_escritura, 0x00, sizeof(buffer_escritura));
                sprintf(buffer_escritura,"%d|%d|%d|%s|%s\n",LEIDO,numero_proceso,nuevo_monto,user[i].nombre_usuario,user[i].apellido_usuario);
                if (abrirArchivo(nombreArchivo, "w") == TRUE)
                {
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
            }else
            {
                printf("Caja %d: sin nuevas inversiones.\n", i + 1);  /* mostrar actividad */
    
            }
            
            
            
        }
        
        /*se va sc*/
        levanta_semaforo(id_semaforo);
        usleep(2000*1000);
    }
    

    return 0;
}

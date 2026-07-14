#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

#include "def.h"
#include "global.h"
#include "semaforo.h"
#include "archivos.h"
#include "funciones.h"
#include "clave.h"
#include "memoria.h"
#include "mensajes.h"
#include "threads.h"

int main(int argc, char *argv[])
{
    int i;
    int id_cola_mensajes;
    char szBuffer[LARGO];

    mensaje msg;
    tauto *datos_thread;
    
    pthread_t idHilo[MAX_AUTOS];
    pthread_attr_t atributos;

    /*THREADS*/
    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/
    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/

    datos_thread = (tauto*) malloc(sizeof(tauto) * MAX_AUTOS);
    if (datos_thread == NULL){
        printf("Error: al crear EL AUTO\n");
        exit(-1);
    }

    /*limpio vars y asigno espacio*/
    memset(&msg,0x00,sizeof(msg));
    memset(szBuffer,0x00,sizeof(szBuffer));
    memset(datos_thread, 0x00, sizeof(tauto) * MAX_AUTOS);/*max_usuarios va a cambiar*/

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    id_cola_mensajes = creo_id_cola_mensajes();

    printf("PROCESO auto LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    /*logica*/
    /*cargar los saldos inciales de los autos*/
    /*lanzo hilos*/

for ( i = 0; i < MAX_AUTOS; i++)
    {
        datos_thread[i].id_cola_msg = id_cola_mensajes;
        cargarData("Ingrese el nombre del PAIS: ", datos_thread[i].nombre_pais);
        cargarData("Ingrese el nombre del piloto: ",datos_thread[i].nombre_piloto);

        datos_thread[i].numero_auto = i + 1;
        printf("El numero del auto es: %d\n",datos_thread[i].numero_auto);

        datos_thread[i].escuderia = cargarInt("Ingrese el numero de la escuderia (FERRARI 1, MCLAREN 2 , OTRO 3)");
        datos_thread[i].posicion = 0;
        datos_thread[i].puntos = cargarInt("Cuantos puntos tiene en el torneo: ");
    }

    printf("--------------------------\n");
    printf("LANZO LOS HILOS\n");

    for ( i = 0; i < MAX_AUTOS; i++)
    {
        if (pthread_create(&idHilo[i],&atributos,&funcionThread,&datos_thread[i])!= 0)
        {
            printf("No puedo crear el hilo\n");
            exit(1);
        }
        printf("Hilo auto %d creado\n",i+1);
    }
    
    printf("--------------------------\n");
    printf("ESPERO A LOS HILOS\n");

    for ( i = 0; i < MAX_AUTOS; i++)
    {
        pthread_join(idHilo[i],NULL);
    }
    printf("Hilos finalizados\n");
    /*for si tengo que mostrar un dato de todos los hilos usando datos_thread[i]
    for ( i = 0; i < MAX_USUARIOS; i++){
        printf("El usuario %d: %s\n",datos_thread[i].numero,datos_thread[i].nombre_entidad_financiera);
    }
    */

    /*shmdt((char *)memoria);*/
    free(datos_thread);
    /*free(idHilo);*/
    return 0;
}

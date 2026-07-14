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
    char nom_equipo[MAX_EQUIPO][LARGO];
    int id_memoria;
    int golesUmbral;

    
    tequipo *datos_thread;
    tequipo *memoria = NULL;
    
    

    pthread_t idHilo[MAX_EQUIPO];
    pthread_attr_t atributos;/*struct para los atributos del hilo*/

    

    /*THREADS*/
    pthread_mutex_init (&mutex, NULL);/*inicilaizo SEMAFORO MUTEX*/
    pthread_attr_init (&atributos);/*INICIALIZO ATRIBUTOS DEL HILO*/
    pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_JOINABLE);/*uso el JOIN para q se trabe en el hilo hasta q no haya un exit*/


    
    memset(szBuffer,0x00,sizeof(szBuffer));
    memset(nom_equipo,  0x00, sizeof(nom_equipo));
    
    golesUmbral = 0;
    

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);
    id_cola_mensajes = creo_id_cola_mensajes();
    memoria = (tequipo*)creo_memoria(sizeof(tequipo)*MAX_EQUIPO, &id_memoria);
    datos_thread = (tequipo*) malloc(sizeof(tequipo) * MAX_EQUIPO);

    if (datos_thread == NULL){
        printf("Error: al crear jugador\n");
        exit(-1);
    }
    memset(datos_thread, 0x00, sizeof(tequipo) * MAX_EQUIPO);
    
    printf("PROCESO jugador LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    /*cargo nombre de equipos*/
    for(i = 0; i < MAX_EQUIPO; i++)
    {
        cargarData("Ingrese el nombre del equipo:", nom_equipo[i]);

        printf("El nombre del equipo %d es %s\n", i + 1,nom_equipo[i]);
    }

    /*INGRESO LOS GOLES DE UMBRAL*/
    golesUmbral = cargarInt("Ingrese la cantidad de goles de umbral:");
    while (golesUmbral <= 0)
    {
        printf("Los goles de umbral deben ser positivos\n");
        golesUmbral = cargarInt("Ingrese la cantidad de goles de umbral:");
    }
    
    /*compartir por mem compartida los goles de umbral a cancha.c*/
    memoria[0].goles_umbral = golesUmbral;
    printf("Equipo: umbral %d escrito en memoria compartida.\n", golesUmbral);
    
    
    
    
    /*lanzo hilos*/
    for ( i = 0; i < MAX_EQUIPO; i++)
    {
        datos_thread[i].numero_equipo = i +1;
        datos_thread[i].id_cola_msg = id_cola_mensajes;
        strcpy(datos_thread[i].nombre_equipo, nom_equipo[i]);
        datos_thread[i].num_pantalla = 0;
        datos_thread[i].num_random = 0;
        datos_thread[i].goles_umbral    = golesUmbral;
        datos_thread[i].goles_acertados = 0;
        if (pthread_create(&idHilo[i], &atributos,&funcionThread, &datos_thread[i])!= 0)
        {
            perror("No puedo crear el hilo\n");
            exit(-1);
        }
        printf("HILO equipo %d CREADO\n",i);
    
        
    }
    
    printf("------------------------------\n");

    printf("EQUIPO: ESPERO A LOS HILOS\n");
    printf("------------------------------\n");
    
    for ( i = 0; i < MAX_EQUIPO; i++)
    {
        pthread_join(idHilo[i],NULL);
    }
    
    printf("Muestro resultados finales:\n");
    for ( i = 0; i < MAX_EQUIPO; i++){
        printf("El equipo %d(%s): METIO %d goles, umbral %d\n",datos_thread[i].numero_equipo,datos_thread[i].nombre_equipo,datos_thread[i].goles_acertados,datos_thread[i].goles_umbral);
    }
    free(datos_thread);
    
    return 0;
}

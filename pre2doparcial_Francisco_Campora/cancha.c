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
    int num_equipo;
    int id_cola_mensajes;
    int id_memoria;
    int partido_terminado;
    int numero_random_generado;
    int numero_ingresado;
    int goles_equipo[MAX_EQUIPO+1];
    char szBuffer[LARGO];
    
    tequipo *memoria = NULL;
    mensaje msg;

    memset(goles_equipo,0x00,sizeof(goles_equipo));
    memset(szBuffer,0x00,sizeof(szBuffer));
    memset(&msg,0x00,sizeof(msg));

    id_cola_mensajes = creo_id_cola_mensajes();
    memoria = (tequipo*)creo_memoria(sizeof(tequipo)*MAX_EQUIPO, &id_memoria);
    
    partido_terminado = FALSE;

    srand(time(NULL));

    crearArchivoSincronismo(FILE_SINCRONISMO,1);
    borrar_mensajes(id_cola_mensajes);

    printf("PROCESO cancha LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    for ( i = 0; i < MAX_EQUIPO; i++)
    {
        /*envio evento inicio a los hilos*/
        
        enviar_mensaje(id_cola_mensajes, MSG_EQUIPO + (i + 1), MSG_CANCHA, EVT_INICIO, "inicio");
    }
    printf("Cancha: inicio enviado a los %d equipos. Partido comenzando.\n", MAX_EQUIPO);

    while (partido_terminado == FALSE)
    {
        recibir_mensaje(id_cola_mensajes,MSG_CANCHA,&msg);/*recibe el tiro y calcula*/
        
        
        
        memset(szBuffer, 0x00, sizeof(szBuffer));
        sscanf(msg.char_mensaje,"%d|%d",&num_equipo,&numero_ingresado);
        
        numero_random_generado = devolverNumAleatorio(DESDE,HASTA);
        printf("El numero random generado es %d\n",numero_random_generado);


        printf("Cancha: equipo %d ingreso %d | random = %d\n",num_equipo, numero_ingresado, numero_random_generado);

        memset(szBuffer, 0x00, sizeof(szBuffer));
        sprintf(szBuffer, "%d", num_equipo);

        /*comparo los numero para saber si es gol*/
        if (numero_random_generado == 3)
        {
            if (numero_ingresado == 3)
            {
                printf("El equipo %d metio un GOL de juagada\n",num_equipo);
                
                goles_equipo[num_equipo]++;
                
            
            }else if(numero_ingresado == 4)
            {
                printf("El equipo %d metio un GOL\n",num_equipo);
                
                goles_equipo[num_equipo]++;
            }else if (numero_ingresado == 5)
            {
                printf("El equipo %d hizo un tiro libre\n",num_equipo);
                goles_equipo[num_equipo]++;
            }else
            {
                printf("eL Equipo %d, le pego afuera\n",num_equipo);
            }
        }else/*osea q random es 4 o 5*/
        {
            if (numero_ingresado == 3)
            {
                printf("El equipo %d LE PEGO AL PALO\n",num_equipo);
                /* envio mensaje de gol al equipo */
                
            
            }else if (numero_ingresado == 4)
            {
                printf("El equipo %d LE PEGO AFUERA\n",num_equipo);
                /* envio mensaje de gol al equipo */
                
            
            }else if (numero_ingresado == 5)
            {
                printf("El equipo %d HIZO UN LATERAL\n",num_equipo);
                
            
            }else
            {
                printf("eL Equipo %d, le pego afuera\n",num_equipo);
                
            }
            
        }
        if (goles_equipo[num_equipo] >= memoria[0].goles_umbral)/*comparo el gol del umbra con los goles acertados*/
        {
            printf("HUBO GANADOR\n");
            printf("Cancha: equipo %d GANO con %d goles (umbral: %d)\n",num_equipo,goles_equipo[num_equipo],memoria[0].goles_umbral);
            
            for ( i = 0; i < MAX_EQUIPO; i++)
            {
                /*avisarle al thread para q corte*/
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d|%d", i+1,goles_equipo[i+1]);
                enviar_mensaje(id_cola_mensajes,MSG_EQUIPO + (i + 1),MSG_CANCHA,EVT_FIN,szBuffer);
                partido_terminado = TRUE;
            }
        }else
        {
            /* el equipo sigue pateando: le aviso para que haga otro tiro */
            memset(szBuffer, 0x00, sizeof(szBuffer));
            sprintf(szBuffer, "%d|%d", num_equipo, goles_equipo[num_equipo]);
            enviar_mensaje(id_cola_mensajes,MSG_EQUIPO + num_equipo,MSG_CANCHA,EVT_SEGUIR_PATEANDO,szBuffer);
        }
        
    }
    printf("Cancha: esperando que el equipo perdedor procese el fin...\n");
    sleep(3);
    
    borrar_cola_de_mensajes(id_cola_mensajes);

    return 0;
}

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

int main(int argc, char  *argv[])
{
    int i;
    int id_cola_mensajes;
    int id_memoria;
    char szBuffer[LARGO];
    
    char nombre_financiera[LARGO];
    int terminado;

    int num_usuario;
    int saldo_usuario;
    int dias_usuario;
    int monto;
    int total_fci;
    int ganancia;
    
    tusuario *memoria;
    mensaje msg;

    /*inicializo variables y limpio*/
    memoria = NULL;
    memset(&msg,     0x00, sizeof(msg));
    memset(szBuffer, 0x00, sizeof(szBuffer));
    memset(nombre_financiera, 0x00, sizeof(nombre_financiera));
    terminado = FALSE;
    monto = 0;
    total_fci = 0;


    id_cola_mensajes = creo_id_cola_mensajes();
    srand(time(NULL));
    
    memoria = (tusuario*)creo_memoria(sizeof(tusuario)*MAX_USUARIOS, &id_memoria);
    memset(memoria, 0x00, sizeof(tusuario) * MAX_USUARIOS);
    
    crearArchivoSincronismo(FILE_SINCRONISMO,1);
    borrar_mensajes(id_cola_mensajes);

    
    printf("PROCESO cancha LISTA PARA EMPEZAR. Enter para empezar...\n");
    getchar();

    cargarData("Ingresar nombre de entidad financiera: ", nombre_financiera);
    printf("Entidad financiera: %s\n", nombre_financiera);

    /*copio en memoria compartida el nombre de la entidad financiera y el horario laboral*/
    strcpy(memoria[0].nombre_entidad_financiera, nombre_financiera);
    memoria[0].hora_minima_laboral = HORA_INICIAL_LABORAL;
    memoria[0].hora_maxima_laboral = HORA_MAXIMA_LABORAL;


    for ( i = 0; i < MAX_USUARIOS; i++)
    {
        /*envio evento inicio a los hilos usuarios*/
        
        enviar_mensaje(id_cola_mensajes, MSG_USUARIO + (i + 1), MSG_ENTIDAD_FINANCIERA, EVT_INICIO, "inicio");
    }
    printf("Entidad: inicio enviado a los %d usuarios\n", MAX_USUARIOS);
    

    while (terminado == FALSE)
    {
        /*loop de logica*/
        recibir_mensaje(id_cola_mensajes,MSG_ENTIDAD_FINANCIERA,&msg);


        switch (msg.int_evento)
        {
            case EVT_CONSULTA:
                memset(szBuffer,0x00,sizeof(szBuffer));
                sscanf(msg.char_mensaje,"%d|%d|%d",&num_usuario,&saldo_usuario,&dias_usuario); 
                printf("Entidad financiera: recibí CONSULTA del usuario %d, saldo: %d, dias invertido: %d\n", num_usuario, saldo_usuario, dias_usuario);
                /*logica para responder la consulta*/
                ganancia = saldo_usuario * 23 * dias_usuario / (100 * 372);
                printf("Entidad: ganancia calculada para usuario %d: %d\n", num_usuario, ganancia);

                /* calcular y mostrar total FCI */
                total_fci = 0;
                for (i = 0; i < MAX_USUARIOS; i++)
                {
                    total_fci += memoria[i].saldo_usuario;
                }
                printf("Entidad: total FCI: %d\n", total_fci);
                
                memoria[num_usuario - 1].saldo_usuario = ganancia;
                memset(szBuffer,0x00,sizeof(szBuffer));
                sprintf(szBuffer, "%d|%d|%d", num_usuario, saldo_usuario, ganancia);
                enviar_mensaje(id_cola_mensajes, MSG_USUARIO + num_usuario, MSG_ENTIDAD_FINANCIERA, EVT_RTA_CONSULTA, szBuffer);
                break;
            case EVT_RETIRO:
                memset(szBuffer,0x00,sizeof(szBuffer));
                sscanf(msg.char_mensaje,"%d|%d|%d|%d",&num_usuario,&saldo_usuario,&dias_usuario,&monto); 
                printf("Entidad financiera: recibí RETIRO del usuario %d, monto a retirar: %d, saldo: %d, dias invertido: %d\n", num_usuario, monto, saldo_usuario, dias_usuario);
                /*logica para responder el retiro*/


                if (monto > saldo_usuario)
                {
                    /*no tiene plata para retirar*/
                    memset(szBuffer,0x00,sizeof(szBuffer));
                    sprintf(szBuffer, "%d|%s", num_usuario, "No tiene saldo suficiente para retirar");
                    enviar_mensaje(id_cola_mensajes, MSG_USUARIO + num_usuario, MSG_ENTIDAD_FINANCIERA, EVT_RTA_RETIRO_NOK, szBuffer);
                }
                else
                {
                    /*puede retirar*/
                    saldo_usuario = saldo_usuario - monto;
                    memset(szBuffer,0x00,sizeof(szBuffer));
                    sprintf(szBuffer, "%d|%d", num_usuario, saldo_usuario);
                    enviar_mensaje(id_cola_mensajes, MSG_USUARIO + num_usuario, MSG_ENTIDAD_FINANCIERA, EVT_RTA_RETIRO_OK, szBuffer);
                }
                break;
            case EVT_INGRESO:
                memset(szBuffer,0x00,sizeof(szBuffer));
                sscanf(msg.char_mensaje,"%d|%d|%d|%d",&num_usuario,&saldo_usuario,&dias_usuario,&monto); 
                printf("Entidad financiera: recibí INGRESO del usuario %d, monto a ingresar: %d, saldo: %d, dias invertido: %d\n", num_usuario, monto, saldo_usuario, dias_usuario);
                /*logica para responder el ingreso*/
                saldo_usuario = saldo_usuario + monto;
                memset(szBuffer,0x00,sizeof(szBuffer));
                sprintf(szBuffer, "%d|%d", num_usuario, saldo_usuario);
                enviar_mensaje(id_cola_mensajes, MSG_USUARIO + num_usuario, MSG_ENTIDAD_FINANCIERA, EVT_RTA_INGRESO_OK, szBuffer);
                break;
            default:
                break;
        }
    }
    printf("MUESTRO DATOS FINALES\n");
    usleep(5000*1000);

    shmdt((char *)memoria);
    shmctl(id_memoria, IPC_RMID, (struct shmid_ds *)NULL);

    borrar_cola_de_mensajes(id_cola_mensajes);
        
    return 0;
}

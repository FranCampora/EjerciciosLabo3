#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <errno.h>

#include "def.h"
#include "global.h"
#include "semaforo.h"
#include "archivos.h"
#include "funciones.h"
#include "clave.h"
#include "memoria.h"
#include "mensajes.h"

int main(int argc, char  *argv[])
{
    int id_cola_mensajes;
    int id_semaforo;
    int opcion;
    int monto;
    int salir;
    int codRta;
    int saldoRta;
    
    char szBuffer[LARGO];
    char szError[LARGO];

    int codCliente;

    mensaje msg;
    
    salir = FALSE;

    memset(szBuffer,0x00,sizeof(szBuffer));
    memset(&msg,0x00,sizeof(msg));
    memset(szError,0x00,sizeof(szError));

    esperarArchivoSincronismo(FILE_SINCRONISMO,1);

    id_semaforo = creo_semaforo();
    id_cola_mensajes = creo_id_cola_mensajes();

    printf("PROCESO CAJERO LISTA PARA EMPEZAR. Enter para empezar...");
    getchar();

    printf("MENU DEL CLINTE");
    codCliente= cargarInt("Ingrese codigo del clinte: \n");
    printf("Esta trabajando el cliente con el codigo %d",codCliente);

    while (salir == FALSE)
    {
        espera_semaforo(id_semaforo);
        printf("==============================\n");
        printf("     CAJERO AUTOMATICO        \n");
        printf("1. Consulta de saldo          \n");
        printf("2. Deposito                   \n");
        printf("3. Extraccion                 \n");
        printf("4. Salir                      \n");
        printf("==============================\n");

        opcion = cargarInt("Ingrese una opcion: ");
        

        /*CASOS DE EVENTOS*/
        switch (opcion)
        {
        case 1:
            
            memset(szBuffer,0x00,sizeof(szBuffer));
            sprintf(szBuffer,"%d",codCliente);

            
            enviar_mensaje(id_cola_mensajes, PROCESO_1,PROCESO_2,EVT_CONSULTA_SALDO,szBuffer);
            
            levanta_semaforo(id_semaforo);
            usleep(1000*1000);
            
            printf("CAJERO: ENVIO *CONSULTA DE SALDO* AL CAJERO. Espera respuesta\n");
            


            recibir_mensaje(id_cola_mensajes,PROCESO_2,&msg);
            if (msg.int_evento == EVT_RTA_SALDO)/*del msg de la respuesta del BACNO*//*con que compara este if?*/
            {
                sscanf(msg.char_mensaje,"%d|%d",&codRta,&saldoRta); /*el mensaje q envio el BANCO, extraigo el code y lo pongo en la var "codRta", y lo mismo para el saldo
                con saldoRta*/
                printf("CAJERO: saldo del cliente %d es: $%d\n", codRta, saldoRta);
            }else if (msg.int_evento == EVT_RTA_SALDO_NOK)
            {
                sscanf(msg.char_mensaje,"%d|%s",&codRta,szError);
                printf("ERROR del cliente %d: %s ",codRta,szError);
            }
            
            break;
        case 2:
            monto = cargarInt("De cuanto queres que sea el deposito?\n");

            memset(szBuffer,0x00,sizeof(szBuffer));
            sprintf(szBuffer,"%d|%d",codCliente,monto);

            
            enviar_mensaje(id_cola_mensajes, PROCESO_1,PROCESO_2,EVT_DEPOSITO,szBuffer);
            levanta_semaforo(id_semaforo);
            usleep(1000*1000);
            
            printf("CAJERO: ENVIO *DEPOSITO* desde L CAJERO. Espera respuesta\n");
            

            recibir_mensaje(id_cola_mensajes,PROCESO_2,&msg);


            sscanf(msg.char_mensaje,"%d|%d",&codRta,&saldoRta);
            if (msg.int_evento == EVT_RTA_DEPOSITO_OK )
            {
                printf("CAJERO: Recibio respuesta, DEPOSITO EXITOS0 del cajero\n");
                printf("El cliente %d , depositó CORRECTAMENTE Y tiene un SALDO DE:%d",codRta,saldoRta);
            }else if (msg.int_evento == EVT_RTA_DEPOSITO_NOK)
            {
                sscanf(msg.char_mensaje,"%d|%s",&codRta,szError);
                printf("ERROR del cliente %d: %s ",codRta,szError);
            }
            
            
            
            break;
        case 3:
            monto = cargarInt("De cuanto queres que sea la extraccion?\n");
        
            memset(szBuffer,0x00,sizeof(szBuffer));
            sprintf(szBuffer,"%d|%d",codCliente,monto);

            
            enviar_mensaje(id_cola_mensajes, PROCESO_1,PROCESO_2,EVT_EXTRACCION,szBuffer);
            levanta_semaforo(id_semaforo);
            usleep(1000*1000);
            printf("CAJERO: ENVIO *EXTRACCION* AL CAJERO. Espera respuesta\n");
            
            recibir_mensaje(id_cola_mensajes,PROCESO_2,&msg);
            if (msg.int_evento == EVT_RTA_EXTRACCION_OK)
            {
                sscanf(msg.char_mensaje,"%d|%d",&codRta,&saldoRta);
                printf("CAJERO: Recibio RESPUESTA EXTRACCION EXITOSA del cajero\n");
                printf("La extraccion del cliente %d fue de de:%d",codRta,saldoRta);
            }
            else if (msg.int_evento == EVT_RTA_EXTRACCION_NOK)
            {
                sscanf(msg.char_mensaje,"%d|%s",&codRta,szError);
                printf("ERROR del cliente %d: %s ",codRta,szError);
            }
            
            break;
        case 4:
            
            enviar_mensaje(id_cola_mensajes, PROCESO_1,PROCESO_2,EVT_FIN,"FIN");
            levanta_semaforo(id_semaforo);
            usleep(1000*1000);
            
            printf("CAJERO: ENVIO *FIN* AL CAJERO. Espera respuesta\n");
            
        
            
            recibir_mensaje(id_cola_mensajes,PROCESO_2,&msg);
            printf("CAJERO: Recibio RESPUESTA de FIN del cajero. ADIOS CLIENTE %d\n",codCliente);
            printf("El programa finalizo");
            salir = TRUE;
            
            break;
        



        default:
            printf("Opcion invalida");
            levanta_semaforo(id_semaforo);
            break;
        }
    }
    
    return 0;
}

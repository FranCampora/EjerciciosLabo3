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
    int codCliente;
    int monto;
    int i;
    char szBuffer[LARGO];


    mensaje msg;
    cliente clienteUsando[MAX_USERS];
    cliente *auxCliente;
    
    

    memset(&msg,0x00,sizeof(msg));
    memset(clienteUsando,0x00,sizeof(clienteUsando));
    memset(szBuffer,0x00,sizeof(szBuffer));
    /*memset(auxCliente,0x00,sizeof(auxCliente));*/
    auxCliente = NULL;
    i = 0;
    monto = 0;
    codCliente = 0;

    while (i < MAX_USERS)
    {
        clienteUsando[i].codigo_cliente = i +1;
        clienteUsando[i].saldo_final = 10000;
        i++;
    }
    
    id_semaforo = creo_semaforo();
    inicia_semaforo(id_semaforo,VERDE);
    id_cola_mensajes = creo_id_cola_mensajes();

    crearArchivoSincronismo(FILE_SINCRONISMO,1);
    borrar_mensajes(id_cola_mensajes);

    

    printf("Proceso BANCO LISTO. Enter para empezar ...");
    getchar();

    printf("----Menu del banco----");
    while (msg.int_evento != EVT_FIN)
    {
        /*espera_semaforo();*/

        recibir_mensaje(id_cola_mensajes,PROCESO_1,&msg);
        printf("----------------\n");
        printf("BANCO (1): RECIBI UN MENSAJE DEL CAJERO, evento NUMERO:%d",msg.int_evento);
        printf("BANCO(1): Data recibida: %s\n",msg.char_mensaje);

        switch (msg.int_evento)
        {
            case EVT_CONSULTA_SALDO:
                /*sscanf(msg.char_mensaje, "%d".&codCliente);1 sola s en scanf, toma teclado, la 2da agarra de un char que ya tengo*/
                auxCliente = buscar_cliente(msg.char_mensaje,clienteUsando,MAX_USERS);/*me devuelve un cliente del array con el code q busco*/
                if (auxCliente != NULL)
                {
                    printf("Saldo: %d\n", auxCliente->saldo_final);
                    /*auxCliente->saldo_final = auxCliente->saldo_final + monto; */
                    memset(szBuffer,0x00,sizeof(szBuffer));
                    sprintf(szBuffer,"%d|%d",auxCliente->codigo_cliente,auxCliente->saldo_final);
                    
                    espera_semaforo(id_semaforo);
                    enviar_mensaje(id_cola_mensajes,PROCESO_2,PROCESO_1,EVT_RTA_SALDO,szBuffer);
                    
                    levanta_semaforo(id_semaforo);
                    usleep(1000*1000);
                }else
                {
                    printf("Cliente no encontrado");
                    memset(szBuffer,0x00,sizeof(szBuffer));
                    sprintf(szBuffer,"%d|cliente no encontrado",codCliente);
                    
                    espera_semaforo(id_semaforo);
                    enviar_mensaje(id_cola_mensajes,PROCESO_2,PROCESO_1,EVT_RTA_SALDO_NOK,szBuffer);
                    
                    levanta_semaforo(id_semaforo);
                    usleep(1000*1000);
                }
                break;
            

                /*
                levanta_semaforo(id_semaforo);
                usleep(1000*1000);
                break;*/
            case EVT_DEPOSITO:
                sscanf(msg.char_mensaje, "%d|%d", &codCliente, &monto);/*sepro el code y el monto del mensaje*/

                /* convierto codCliente a string para pasarselo a buscar_cliente */
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d", codCliente);
                auxCliente = buscar_cliente(szBuffer,clienteUsando,MAX_USERS);/*me devuelve un cliente del array con el code q busco*/
                
                if (auxCliente != NULL)
                {
                    printf("BANCO: cliente con el codigo %d, tiene un saldo INICIAL %d y quiere hacer un deposito de: %d",auxCliente->codigo_cliente,auxCliente->saldo_final,monto);
                    auxCliente->saldo_final = auxCliente->saldo_final + monto;
                    printf("BANCO: Nuevo saldo %d",auxCliente->saldo_final);

                    memset(szBuffer, 0x00, sizeof(szBuffer));
                    sprintf(szBuffer,"%d|%d",auxCliente->codigo_cliente,auxCliente->saldo_final);
                    
                    espera_semaforo(id_semaforo);
                    enviar_mensaje(id_cola_mensajes,PROCESO_2,PROCESO_1,EVT_RTA_DEPOSITO_OK,szBuffer);
                    

                
                }else
                {
                    printf("Cliente no encontrado para deposito");
                    memset(szBuffer,0x00,sizeof(szBuffer));
                    sprintf(szBuffer,"%d|cliente no encontrado",codCliente);
                    espera_semaforo(id_semaforo);
                    enviar_mensaje(id_cola_mensajes,PROCESO_2,PROCESO_1,EVT_RTA_DEPOSITO_NOK,szBuffer);
                
                }
                
                
                

                
                levanta_semaforo(id_semaforo);
                usleep(1000*1000);
                break;
            case EVT_EXTRACCION:
                sscanf(msg.char_mensaje, "%d|%d", &codCliente, &monto);/*sepro el code y el monto del mensaje*/

                /* convierto codCliente a string para pasarselo a buscar_cliente */
                memset(szBuffer, 0x00, sizeof(szBuffer));
                sprintf(szBuffer, "%d", codCliente);
                auxCliente = buscar_cliente(szBuffer,clienteUsando,MAX_USERS);/*me devuelve un cliente del array con el code q busco*/
                
                if (auxCliente != NULL)
                {
                    if (auxCliente->saldo_final >= monto)
                    {
                        printf("BANCO: cliente %d, tiene un saldo INICIAL %d y quiere hacer una EXTRACCUION de: %d",auxCliente->codigo_cliente,auxCliente->saldo_final,monto);
                    
                        auxCliente->saldo_final = auxCliente->saldo_final-monto;
                        printf("BANCO: queda un saldo final de %d",auxCliente->saldo_final);

                        memset(szBuffer,0x00,sizeof(szBuffer));
                        sprintf(szBuffer,"%d|%d",auxCliente->codigo_cliente,auxCliente->saldo_final);
                        espera_semaforo(id_semaforo);
                        enviar_mensaje(id_cola_mensajes,PROCESO_2,PROCESO_1,EVT_RTA_EXTRACCION_OK,szBuffer);
                
                    }else
                    {
                        printf("El cliente no tiene el saldo disponible para la extraccion, saldo del cliente %d",auxCliente->saldo_final);
                        memset(szBuffer,0x00,sizeof(szBuffer));
                        sprintf(szBuffer,"%d|saldo insuficiente",auxCliente->codigo_cliente);
                        
                        espera_semaforo(id_semaforo);
                        enviar_mensaje(id_cola_mensajes,PROCESO_2,PROCESO_1,EVT_RTA_EXTRACCION_NOK,szBuffer);
                
                    }
                    
                    
                    }else{
                        printf("Cliente no encontrado");
                        memset(szBuffer,0x00,sizeof(szBuffer));
                        sprintf(szBuffer,"%d|cliente no encontrado",codCliente);
                        espera_semaforo(id_semaforo);
                        enviar_mensaje(id_cola_mensajes,PROCESO_2,PROCESO_1,EVT_RTA_EXTRACCION_NOK,"NO SE ENCONTRO CLIENTE");        
                    }
                
                levanta_semaforo(id_semaforo);
                usleep(1000*1000);
                
                break;
            case EVT_FIN:
                printf("BANCO: Recibi EVENTO FIN\n");
                
                espera_semaforo(id_semaforo);
                
                enviar_mensaje(id_cola_mensajes,PROCESO_2,PROCESO_1,EVT_FIN,"BANCO CERRADO");
                
                levanta_semaforo(id_semaforo);
                usleep(1000*1000);
                break;
            default:
                break;
        }
    }

    borrar_cola_de_mensajes(id_cola_mensajes);
    return 0;
}

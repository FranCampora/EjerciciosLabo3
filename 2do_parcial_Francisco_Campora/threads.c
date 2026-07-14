#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "global.h"
#include "clave.h"
#include "mensajes.h"
#include "funciones.h"
#include "def.h"

void *funcionThread(void *parametro){

    
    int done;
    int id_cola_mensajes;
    char szBuffer[LARGO];
    int numero_usuario;
    int saldo_usuario;
    char nombre_usuario[LARGO];
    int dias_invertido;/*el mes es de 31 dias*/
    char nombre_entidad_financiera[LARGO];
    int hora_minima_laboral;
    int hora_maxima_laboral;
    int ganancia;
    int monto;
    
    int numero_aleatorio;
    int opcion_menu;
    


    mensaje msg; 
    tusuario *datos_thread;

    datos_thread = (tusuario*) parametro;

    /*srand(time(NULL));*/

    id_cola_mensajes = datos_thread->id_cola_msg;
    numero_usuario = datos_thread->numero_usuario; 
    saldo_usuario = datos_thread->saldo_usuario;
    dias_invertido = datos_thread->dias_invertido;
    hora_minima_laboral = datos_thread->hora_minima_laboral;
    hora_maxima_laboral = datos_thread->hora_maxima_laboral;
    ganancia = datos_thread->ganancia;
    strcpy(nombre_usuario, datos_thread->nombre_usuario);
    strcpy(nombre_entidad_financiera, datos_thread->nombre_entidad_financiera);

    
    done = 0;
    numero_aleatorio = 0;
    monto = 0;
    

    recibir_mensaje(id_cola_mensajes,MSG_USUARIO+numero_usuario,&msg);
    pthread_mutex_lock(&mutex);
    if (msg.int_evento == EVT_INICIO)
    {
        printf("Hilo usuario numero%d: recibio INICIO.\n", numero_usuario);
    }
    pthread_mutex_unlock(&mutex);
    usleep(1000*1000);

    while (done == 0) 
    {
        pthread_mutex_lock(&mutex);
        dias_invertido++;
        /*recibir_mensaje(id_cola_mensajes, MSG_USUARIO + numero_usuario, &msg);
        */
        
        numero_aleatorio = devolverNumAleatorio(HORA_INICIAL_RELOJ, HORA_MAXIMA_RELOJ);/*ENTRE 1 Y 24*/
        /*valido que el numero sea en el horario laboral osea entre 10 y 16*/
        pthread_mutex_unlock(&mutex);

        usleep(1000*1000);
        if (numero_aleatorio >= 10 && numero_aleatorio <= 16)
        {
            
            printf("Usuario %d, esta dentro del horario laboral, numero: %d horas\n", numero_usuario, numero_aleatorio);
            /*continua logica*/
            printf("Seleccione una opcion del menu:\n");
            
            opcion_menu = menu();
            switch (opcion_menu)
            {
            case 1:
                pthread_mutex_lock(&mutex);
                memset(szBuffer,0x00,sizeof(szBuffer));
                sprintf(szBuffer,"%d|%d|%d",numero_usuario,saldo_usuario,dias_invertido);
                enviar_mensaje(id_cola_mensajes, MSG_ENTIDAD_FINANCIERA, MSG_USUARIO + numero_usuario, EVT_CONSULTA, szBuffer);
                printf("Usuario %d, envio CONSULTA a la entidad financiera\n", numero_usuario);

                pthread_mutex_unlock(&mutex);
                usleep(1000*1000);
                recibir_mensaje(id_cola_mensajes, MSG_USUARIO + numero_usuario, &msg);
                pthread_mutex_lock(&mutex);
                if (msg.int_evento == EVT_RTA_CONSULTA)
                {
                    sscanf(msg.char_mensaje,"%d|%d|%d",&numero_usuario,&saldo_usuario,&ganancia);

                    printf("Usuario %d, recibio respuesta de CONSULTA. Saldo: %d, en dias %d\n", numero_usuario, saldo_usuario, dias_invertido);
                    
                }
                else
                {
                    printf("Usuario %d, recibio respuesta de CONSULTA desconocida\n", numero_usuario);
                }
                pthread_mutex_unlock(&mutex);
                usleep(1000*1000);
                break;
            case 2:/*ingreso dinero*/
                
                monto = cargarInt("Cuanto dinero desea ingresar?\n");
                pthread_mutex_lock(&mutex);
                memset(szBuffer,0x00,sizeof(szBuffer));
                sprintf(szBuffer,"%d|%d|%d|%d",numero_usuario,saldo_usuario,dias_invertido,monto);
                enviar_mensaje(id_cola_mensajes, MSG_ENTIDAD_FINANCIERA, MSG_USUARIO + numero_usuario, EVT_INGRESO, szBuffer);
                printf("Usuario %d, envio INGRESO a la entidad financiera\n", numero_usuario);
                
                pthread_mutex_unlock(&mutex);
                usleep(1000*1000);
                recibir_mensaje(id_cola_mensajes, MSG_USUARIO + numero_usuario, &msg);
                pthread_mutex_lock(&mutex);
                if (msg.int_evento == EVT_RTA_INGRESO_OK)
                {
                    sscanf(msg.char_mensaje,"%d|%d",&numero_usuario,&saldo_usuario);

                    printf("Usuario %d, recibio respuesta de INGRESO OK. Saldo: %d\n", numero_usuario, saldo_usuario);
                    
                }
                else if (msg.int_evento == EVT_RTA_INGRESO_NOK)
                {
                    sscanf(msg.char_mensaje,"%d|%s",&numero_usuario,szBuffer);

                    printf("Usuario %d, recibio respuesta de INGRESO NOK. Error: %s\n", numero_usuario, szBuffer);
                    
                }
                else
                {
                    printf("Usuario %d, recibio respuesta de INGRESO desconocida\n", numero_usuario);
                }

                pthread_mutex_unlock(&mutex);
                usleep(1000*1000);
                break;
            case 3:/*retiro dinero*/
                
                monto = cargarInt("Cuanto dinero desea retirar?\n");
                pthread_mutex_lock(&mutex);
                memset(szBuffer,0x00,sizeof(szBuffer));
                sprintf(szBuffer,"%d|%d|%d|%d",numero_usuario,saldo_usuario,dias_invertido,monto);
                enviar_mensaje(id_cola_mensajes, MSG_ENTIDAD_FINANCIERA, MSG_USUARIO + numero_usuario, EVT_RETIRO, szBuffer);
                printf("Usuario %d, envio RETIRO a la entidad financiera\n", numero_usuario);

                pthread_mutex_unlock(&mutex);
                usleep(1000*1000);
                recibir_mensaje(id_cola_mensajes, MSG_USUARIO + numero_usuario, &msg);
                pthread_mutex_lock(&mutex);
                if (msg.int_evento == EVT_RTA_RETIRO_OK)
                {
                    sscanf(msg.char_mensaje,"%d|%d",&numero_usuario,&saldo_usuario);

                    printf("Usuario %d, recibio respuesta de RETIRO OK. Saldo: %d\n", numero_usuario, saldo_usuario);
                    
                }
                else if (msg.int_evento == EVT_RTA_RETIRO_NOK)
                {
                    sscanf(msg.char_mensaje,"%d|%s",&numero_usuario,szBuffer);

                    printf("Usuario %d, recibio respuesta de RETIRO NOK. Error: %s\n", numero_usuario, szBuffer);
                    
                }
                else
                {
                    printf("Usuario %d, recibio respuesta de RETIRO desconocida\n", numero_usuario);
                }

                pthread_mutex_unlock(&mutex);
                usleep(1000*1000);
                break;
            case 4:/*consultar ganancia*/
                pthread_mutex_lock(&mutex);
                memset(szBuffer,0x00,sizeof(szBuffer));
                sprintf(szBuffer,"%d|%d|%d|%d",numero_usuario,saldo_usuario,dias_invertido,monto);
                enviar_mensaje(id_cola_mensajes, MSG_ENTIDAD_FINANCIERA, MSG_USUARIO + numero_usuario, EVT_CONSULTA_GANANCIA, szBuffer);
                printf("Usuario %d, envio CONSULTA a la entidad financiera\n", numero_usuario);

                pthread_mutex_unlock(&mutex);
                usleep(1000*1000);
                recibir_mensaje(id_cola_mensajes, MSG_USUARIO + numero_usuario, &msg);
                pthread_mutex_lock(&mutex);


                pthread_mutex_unlock(&mutex);
                usleep(1000*1000);
                break;
            case 0:/*salgo*/
                
                pthread_mutex_lock(&mutex);
                done = 1; 
                printf("Usuario %d, selecciono salir\n", numero_usuario);
                pthread_mutex_unlock(&mutex);
                break;
            
            default:
                printf("Usuario %d, selecciono una opcion de menu invalida\n", numero_usuario);
                pthread_mutex_unlock(&mutex);
                usleep(1000*1000);
                break;
            }
        }else
        {
            pthread_mutex_lock(&mutex);
            printf("Usuario %d, esta fuera del horario laboral, numero: %d horas\n", numero_usuario, numero_aleatorio);
            pthread_mutex_unlock(&mutex);    
            usleep(1000*1000);
            /*recibir_mensaje(id_cola_mensajes, MSG_USUARIO + numero_usuario, &msg);
        */
        }
        /*pthread_mutex_unlock(&mutex);*/
        usleep(1000*1000);


    
    }
    
    
    
    pthread_exit(NULL);
    
}

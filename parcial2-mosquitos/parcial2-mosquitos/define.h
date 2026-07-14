#ifndef _DEFINE_H
#define _DEFINE_H

#define TRUE 1
#define FALSE 0

#define COMANDO "/bin/ls" /*Comando conocido para ftok()*/
#define ENTERO 11 /*Entero para ftok()*/

#define ROJO 0 /*Semaforo envia el proc a la cola*/
#define VERDE 1 /*Semaforo permite el ingreso*/

/*Para char*/
#define LARGO 400
#define CORTO 100
#define LINEA 255+1

#define MS 1000 /*Multiplicar usleep() por la cantidad de ms requerida*/



/* ----------------------------------------------------- */

#define CANTIDAD_HILOS 2 /*Cantidad de hilos a lanzar*/
#define META 95 /*Condicion de fin*/

#define MOSQUITO_DESDE 3
#define MOSQUITO_HASTA 6

#define HUMO_DESDE 1
#define HUMO_HASTA 4

#define GANADOR 1
#define PERDEDOR 2

/*Eventos*/
#define EVT_INICIO 1
#define EVT_FIN 2
#define EVT_AVANZAR 3
#define EVT_SEGUIR 4

#define MSG_PISTA 1 /*Identificador de la cola de mensajes*/
#define MSG_HILO 2 /*Identificador de la cola de mensajes de los hilos (MSG_HILO + nro_hilo)*/


#endif



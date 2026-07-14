#ifndef _DEFINE_H
#define _DEFINE_H

#define TRUE 1
#define FALSE 0

#define COMANDO "/bin/ls" /*Comando conocido para ftok()*/
#define ENTERO 11 /*Entero para ftok()*/

#define ROJO 0 /*Semaforo envia el proc a la cola*/
#define VERDE 1 /*Semaforo permite el ingreso*/

/*Para char*/
#define LARGO 150
#define CORTO 50
#define LINEA 255+1

#define MS 1000 /*Multiplicar usleep() por la cantidad de ms requerida*/



/* ----------------------------------------------------- */
#define CANTIDAD 10
#define SHM_CANTIDAD 10 /*Para memoria compartida*/

/*Utilizados para numero aleatorio*/
#define DESDE 0
#define HASTA 5

#define META 100

#define INT_MAXIMO 100 /*Utilizado en funciones.c para validar el ingreso*/

#define INICIALIZADOR "init"
#define ARCHIVO "usuarios.dat" /*Nombre del archivo de almacenamiento*/

#define I_MAIN2 1 /*instancias del main2*/

#define JUGADORES 6 /*Cantidad de jugadores*/
#define TAMBOR 6 /*Capacidad del tambor*/

#define EVT_INICIO 1
#define EVT_DISPARO 2
#define EVT_SALVADO 3
#define EVT_MUERTO 4
#define EVT_FIN 5


#define MSG_REVOLVER 1 /*Identificador de la cola de mensajes del revolver*/
#define MSG_OTRO 3
#define MSG_JUGADOR 10 /*Identificador de la cola de mensajes de los jugadores (MSG_JUGADOR + nro_jugador)*/




#endif



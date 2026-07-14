#ifndef _DEF_H
#define _DEF_H

#define TRUE 1
#define FALSE 0

#define DESDE 1
#define HASTA 99

#define LOTE 10
#define PATH_CLAVE "/bin/ls"
#define ENTERO_CLAVE 33
#define LARGO 255
#define VERDE 1
#define CANTIDAD_PARTIDA 5
#define INTERVALO_PRODUCTO 50
#define INTERVALO_PARTIDA 200
#define NOMBRE_FILE "buffer.dat"

#define FILE_SINCRONISMO "sincro.dat"
#define CLAVE_BASE 1
#define PROCESO_1 1
#define PROCESO_2 2
#define MAX_USERS 100


/*eventos para cola de menajes*/
#define EVT_NINGUNO 0
#define EVT_CONSULTA_SALDO 1
#define EVT_DEPOSITO 2 
#define EVT_EXTRACCION 3
#define EVT_RTA_SALDO 4
#define EVT_RTA_SALDO_NOK 5
#define EVT_RTA_DEPOSITO_OK 6
#define EVT_RTA_DEPOSITO_NOK 7
#define EVT_RTA_EXTRACCION_OK 8
#define EVT_RTA_EXTRACCION_NOK 9
#define EVT_FIN 100

#define CANTIDAD_THREADS 5

typedef struct{
    
    char nombreJugador[LARGO];
    int numeroPensado;
    int estadoAcierto;
}juego;


/*typedef struct tipo_dato dato;*/

#endif

#ifndef _DEF_H
#define _DEF_H

#define TRUE 1
#define FALSE 0

#define DESDE 1
#define HASTA 99


#define PATH_CLAVE "/bin/ls"
#define ENTERO_CLAVE 33
#define LARGO 255
#define VERDE 1
#define ROJO 0
#define NOMBRE_FILE "buffer.dat"

#define FILE_SINCRONISMO "sincro.dat"
#define CLAVE_BASE 1
#define PROCESO_BOLERO 1
#define PROCESO_JUGADOR 2
#define MAX_USERS 100

#define CANTIDAD_THREADS 5
#define SALDO_INICIAL 100000
/*eventos para cola de menajes*/
typedef enum
{
    MSG_NADIE = 0,
    MSG_BANCO = 1,
    MSG_CAJERO = 2
} Destinos;

/*eventos para cola de menajes*/


typedef enum
{
    EVT_NINGUNO = 0,
    EVT_CONSULTA_SALDO = 1,
    EVT_DEPOSITO = 2,
    EVT_EXTRACCION = 3,
    EVT_RTA_SALDO = 4,
    EVT_RTA_SALDO_NOK = 5,
    EVT_RTA_DEPOSITO_OK = 6,
    EVT_RTA_DEPOSITO_NOK = 7,
    EVT_RTA_EXTRACCION_OK = 8,
    EVT_RTA_EXTRACCION_NOK = 9,
    EVT_FIN = 100
} Eventos;


typedef struct  tipo_jugador tjugador;

struct tipo_jugador
{
    int numeroJugador;
    int id_colamsg;
    int monto;
};



#endif

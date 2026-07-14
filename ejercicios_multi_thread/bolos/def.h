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
#define MAX_USERS 2

#define CANTIDAD_THREADS 5


/*eventos para cola de menajes*/


typedef enum
{
    EVT_NINGUNO = 0,
    EVT_INICIO = 1,
    EVT_TIRO = 2,
    EVT_RESULTADO = 3,
    EVT_RTA_INICIO = 4,
    EVT_RTA_INICIO_NOK = 5,
    EVT_RTA_TIRO_OK = 6,
    EVT_RTA_TIRO_NOK = 7,
    EVT_RTA_RESULTADO_OK = 8,
    EVT_RTA_RESULTADO_NOK = 9,
    EVT_FIN = 100
} Eventos;

typedef struct  tipo_jugador tjugador;

struct tipo_jugador
{
    int numeroJugador;
    int cantidadPinosEnPie[2];/*posicion 0 para jugador 1, posicion 1 para jugador 2*/
    int cantidadTiros;
    int id_colamsg;
}t;



#endif

#ifndef _DEF_H
#define _DEF_H

#define TRUE 1
#define FALSE 0

#define DESDE 2
#define HASTA 5


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
#define MAX_USERS 5

#define CANTIDAD_THREADS 5


/*eventos para cola de menajes*/
typedef enum
{
    MSG_NADIE,
    MSG_ARQUERO,
    MSG_PATEADOR
} Destinos;

#define EVT_NINGUNO 0
#define EVT_TIRO 1
#define EVT_GOL 2
#define EVT_PALO 3
#define EVT_TRAVESANO 4
#define EVT_FUERA 5
#define EVT_FIN 100


typedef struct  tipo_jugador tjugador;

struct tipo_jugador
{
    int numeroJugador;
    int id_colamsg;
    int tipoTiro;

};



#endif

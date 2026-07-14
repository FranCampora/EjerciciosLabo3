#ifndef _DEF_H
#define _DEF_H

#define TRUE 1
#define FALSE 0

#define DESDE 0
#define HASTA 3


#define PATH_CLAVE "/bin/ls"
#define ENTERO_CLAVE 33
#define LARGO 255
#define VERDE 1
#define ROJO 0
#define NOMBRE_FILE "buffer.dat"

#define FILE_SINCRONISMO "sincro.dat"
#define CLAVE_BASE 1

#define MONTO_INICIAL_MIN 500
#define MONTO_INICIAL_MAX 800

#define MSG_NADIE = 0,
#define MSG_PISTA = 1,
#define MSG_JUGADOR1 = 2

/*eventos para cola de menajes*/

#define EVT_NINGUNO = 0,
#define EVT_CORRO = 1,
#define EVT_RTA_TABLERO = 2,
#define EVT_INICIO = 200,
#define EVT_FIN = 100


typedef struct  tipo_jugador tjugador;

struct tipo_jugador
{
    int numeroJugador;
    char nombre_jug[LARGO];
    int id_colamsg;
    int monto;
    int posicion;
    int turnoActivo;

};



#endif

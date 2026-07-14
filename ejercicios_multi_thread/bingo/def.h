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





#define MSG_NADIE 0 
#define MSG_BINGO 1
#define MSG_JUGADOR 2 

/*eventos para cola de menajes*/


typedef enum
{
    EVT_NINGUNO = 0,
    EVT_BOLILLA = 1,
    EVT_CARTON_LLENO = 2,
    EVT_FIN = 100,
    EVT_INICIO = 200,
    EVT_INICIO_ACK = 300
} Eventos;



typedef struct  tipo_jugador tjugador;

struct tipo_jugador
{
    int numeroJugador;
    int id_colamsg;
    int carton[5];
    int cantJugadores;
    int aciertos;
};

typedef struct tipo_bolillero tbolillero;
struct tipo_bolillero
{
    int cantJugadores;
    int id_colamsg;
    
};



#endif

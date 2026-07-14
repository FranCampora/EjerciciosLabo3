#ifndef _DEF_H
#define _DEF_H

#define TRUE 1
#define FALSE 0

#define DESDE_MOSQUITO 3
#define HASTA_MOSQUITO 6

#define DESDE_HUMO 1
#define HASTA_HUMO 4


#define PATH_CLAVE "/bin/ls"
#define ENTERO_CLAVE 33
#define LARGO 255
#define VERDE 1
#define ROJO 0
#define NOMBRE_FILE "buffer.dat"

#define FILE_SINCRONISMO "sincro.dat"
#define CLAVE_BASE 1



#define MAX_MOSQUITOS 2

#define MSG_NADIE 0
#define MSG_TABLERO 1
#define MSG_MOSQUITO 2

/*eventos para cola de menajes*/

#define EVT_NINGUNO 0
#define EVT_MOV_MOSQUITO 1
#define EVT_INICIO 200
#define EVT_MOSQUITO_MUERTO 3
#define EVT_TIRO_LIBRE 4
#define EVT_PALO 5
#define EVT_FUERA 6
#define EVT_LATERAL 7
#define EVT_INICIO_ACK 250
#define EVT_FIN 100


typedef struct  tipo_equipo tmosquito;

struct tipo_equipo
{
    int id_cola_msg;
    int num_mosquito;
    int distancia_espiral;
    int posicion_humo;
    int posicion_mosquito;
    int mov_mosquito;
    int mov_humo;
    int salud_mosquito;/*0 esta muerto y 1 esta vivo*/


};



#endif

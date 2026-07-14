#ifndef _DEF_H
#define _DEF_H

#define TRUE 1
#define FALSE 0

#define PATH_CLAVE "/bin/ls"
#define ENTERO_CLAVE 33
#define LARGO 255
#define VERDE 1
#define ROJO 0
#define NOMBRE_FILE "buffer.dat"

#define FILE_SINCRONISMO "sincro.dat"
#define CLAVE_BASE 1

#define DISTANCIA_FIN 130

/*defines de velocidadades*/
#define AVANCE_PEZ_DESDE 1
#define AVANCE_PEZ_HASTA 6
#define AVANCE_TIBURON_DESDE 1
#define AVANCE_TIBURON_HASTA 8
#define AVANCE_PULPO_DESDE 1
#define AVANCE_PULPO_HASTA 3



#define PULPO_PODER_DESDE 1
#define PULPO_PODER_HASTA 3
#define NUM_PULPO_PODER 1
#define CERCANIA_PULPO 10

#define TIBURON_PODER_DESDE 1
#define TIBURON_PODER_HASTA 3
#define NUM_TIBURON_PODER 1
#define CERCANIA_TIBURON 7

#define USO_PODER 1
#define NO_USO_PODER 0

#define ESTADO_CORRIENDO 1
#define ESTADO_MUERTO 2
#define ESTADO_META 3


#define MAX_MARITIMOS 6
#define MSG_NADIE 0
#define MSG_PISTA 1
#define MSG_MARITIMO 10

#define PEZ 1
#define PULPO 2
#define TIBURON 3

/*eventos para cola de menajes*/

#define EVT_NINGUNO 0
#define EVT_INICIO 1
#define EVT_AVANZA 2
#define EVT_MUERE 3
#define EVT_LLEGA_META 4
#define EVT_TURNO 5
#define EVT_FIN 100
#define EVT_ACK 6
#define EVT_ATAQUE_ENVIAR 7
#define EVT_ATAQUE_RECIBIR 8




typedef struct  tipo_maritimo tmaritimo;

struct tipo_maritimo
{
    int id_cola_msg;
    char nombre_maritimo[LARGO];
    int tipo_maritimo;/*1 pez, 2 tiburon, 3 pulpo*/
    int posicion;
    int estado;/*corriendo(1), muerto(2), meta(3)*/
    int numero_maritimo;
    int uso_poder;
    int cant_vuelta;
    int distancia_meta;/*medido en km*/
};



#endif

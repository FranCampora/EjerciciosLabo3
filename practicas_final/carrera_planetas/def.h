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

#define DISTANCIA_FIN 800

/*defines de velocidadades*/
#define AVANCE_TIERRA_DESDE 20
#define AVANCE_TIERRA_HASTA 50
#define AVANCE_DINO_DESDE 30
#define AVANCE_DINO_HASTA 60
#define AVANCE_OVNI_DESDE 40
#define AVANCE_OVNI_HASTA 90


#define CHEKEO_PROPU 500
#define PROPU_DESDE 1
#define PROPU_HASTA 4

#define DINO_PODER_DESDE 1
#define DINO_PODER_HASTA 10
#define NUM_DINO_PODER 5

#define USO_PODER 1
#define NO_USO_PODER 0

#define ESTADO_CORRIENDO 1
#define ESTADO_MUERTO 2
#define ESTADO_META 3


#define MAX_PLANETAS 4
#define MSG_NADIE 0
#define MSG_PISTA 1
#define MSG_PLANETA 10

#define TIERRA 1
#define DINO 2
#define OVNI 3

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




typedef struct  tipo_planeta tplaneta;

struct tipo_planeta
{
    int id_cola_msg;
    char nombre_planeta[LARGO];
    int tipo_planeta;/*1 tierra, 2 dino, 3 ovni*/
    int posicion;
    int estado;/*corriendo(1), muerto(2), meta(3)*/
    int numero_planeta;
    int uso_poder;/*tierra 1 sola vez posr 500km, dinos 1 x vuelta, ovni no tiene. SI EL VALOR ES 1 LO USO Y SI ES 0 NO */
    int cant_vuelta;
    int distancia_meta;/*medido en km*/
};



#endif

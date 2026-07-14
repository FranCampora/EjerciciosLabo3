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
#define AVANCE_THOMAS_DESDE 2
#define AVANCE_THOMAS_HASTA 8
#define AVANCE_MUCHACHO_DESDE 1
#define AVANCE_MUCHACHO_HASTA 5
#define AVANCE_CRIATURA_DESDE 2
#define AVANCE_CRIATURA_HASTA 6

#define METRAJE_ENTRADA_DESDE 120
#define METRAJE_ENTRADA_HASTA 220

#define METRAJE_INICIAL_CRIATURA 20

#define GOLPE_CRIATURA_DESDE 1
#define GOLPE_CRIATURA_HASTA 6
#define GOLPE_MUCHACHO_DESDE 1
#define GOLPE_MUCHACHO_HASTA 6
#define GOLPE_MORTAL 6


#define CHEKEO_METRAJE1 67
#define SALTO_CRIATURA_DESDE 1
#define SALTO_CRIATURA_HASTA 5
#define SALTO_MUCHACHO_DESDE 1
#define SALTO_MUCHACHO_HASTA 4
#define SALTO_THOMAS_DESDE 1
#define SALTO_THOMAS_HASTA 9
#define SALTO_MUERTE 1


#define MAX_CORREDORES 6
#define MSG_NADIE 0
#define MSG_PISTA 1
#define MSG_MUCHACHO1 2
#define MSG_MUCHACHO2 3
#define MSG_THOMAS 4
#define MSG_CRIATURA1 5
#define MSG_CRIATURA2 6
#define MSG_CRIATURA3 7

/*eventos para cola de menajes*/

#define EVT_NINGUNO 0
#define EVT_INICIO 1
#define EVT_TURNO 2
#define EVT_AVANZA 3
#define EVT_MUERE 4
#define EVT_LLEGA_META 5
#define EVT_GOLPE 6
#define EVT_DEFENSA 7
#define EVT_SALTA 8
#define EVT_MATA_RIVAL 9
#define EVT_FIN 100

/* tipos de corredor */
#define TIPO_MUCHACHO 1
#define TIPO_THOMAS 2
#define TIPO_CRIATURA 3

/* activacion de las criaturas */
#define ACTIVADA 1
#define NO_ACTIVADA 0

#define ESTADO_CORRIENDO 0
#define ESTADO_META 1
#define ESTADO_MUERTO 2

#define NOMBRE_THOMAS "Thomas"

typedef struct  tipo_corredor tcorredor;

struct tipo_corredor
{
    int id_cola_msg;
    char nombre_corredor[LARGO];/*puede ser de muchacho ó criatura*/
    int metraje_entrada;/*entrada 1 corredor 1 y asi*/
    int tipo_corredor;/*1 muchacho, 2 thomas, 3 criatura*/
    int posicion;
    int estado;/*corriendo(1), muerto(2), meta(3)*/
    int msg_destino;/*direccion de mensaje propia del hilo, ej MSG_THOMAS, MSG_CRIATURA1*/
    int msg_rival;/*direccion de mensaje del par de este corredor en el mismo camino*/
    
};



#endif

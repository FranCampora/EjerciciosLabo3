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

/*parametros del ejercicio*/
#define VIDA_LUCHADOR 140

#define EMPERADOR_DECISION_DESDE 1
#define EMPERADOR_DECISION_HASTA 5
#define EMPERADOR_DECISION_VIVO_1 1
#define EMPERADOR_DECISION_VIVO_2 2

#define ATAQUE_VALOR_DESDE 1
#define ATAQUE_VALOR_HASTA 140

/*----*/
#define ESTADO_VIVO 1
#define ESTADO_MUERTO 2 
#define ESTADO_META 3 /*llego al obj y gano*/


#define MAX_LUCHADORES 8 /*3 GLADIADORES Y 5 IMPERIO*/
#define MSG_NADIE 0
#define MSG_COLISEO 1
#define MSG_LUCHADOR 10

#define IMPERIO 1
#define GLADIADOR 2


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




typedef struct tipo_luchador tluchador;

struct tipo_luchador
{
    int id_cola_msg;
    char nombre_luchador[LARGO];
    int tipo_luchador;/* TIPO_IMPERIO, TIPO_GLADIADOR, TIPO_C */
    int estado;/*ESTADO_MUERTO, ESTADO_VIVO */
    int numero_luchador;/*numero de luchador, para identificarlo en la memoria compartida*/
    int vida;/*vida del luchador*/
    int arma;/*arma del luchador, 1 espada, 2 lanza 3 otro*/
    
};



#endif

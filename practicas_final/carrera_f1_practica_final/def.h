#ifndef _DEF_H
#define _DEF_H

#define TRUE 1
#define FALSE 0

#define DESDE 3
#define HASTA 5


#define PATH_CLAVE "/bin/ls"
#define ENTERO_CLAVE 33
#define LARGO 255
#define VERDE 1
#define ROJO 0
#define NOMBRE_FILE "buffer.dat"

#define FILE_SINCRONISMO "sincro.dat"
#define CLAVE_BASE 1

#define DISTANCIA_FIN 295

/*defines de velocidadades*/
#define AVANCE_NORMAL_DESDE 5
#define AVANCE_NORMAL_HASTA 10
#define AVANCE_RAPIDO_DESDE 6
#define AVANCE_RAPIDO_HASTA 12

#define BOXES_DESDE 1
#define BOXES_HASTA 3
#define BOXES_VA_BOXES 1

#define CHEKEO_1 50
#define CHEKEO_2 100
#define CHEKEO_3 150
#define CHEKEO_4 200
#define CHEKEO_5 250

#define FERRARI 1
#define MCLAREN 2
#define ESCUDERIA_OTRA 3

#define ESTADO_CORRIENDO 0
#define ESTADO_FUERA 1
#define ESTADO_META 2


#define MAX_AUTOS 3
#define MSG_NADIE 0
#define MSG_PISTA 1
#define MSG_AUTO 10

/*eventos para cola de menajes*/

#define EVT_NINGUNO 0
#define EVT_INICIO 1
#define EVT_AVANZA 2
#define EVT_BOXES 3
#define EVT_SALE_PISTA 4
#define EVT_LLEGA_META 5
#define EVT_TURNO 6
#define EVT_FIN 100
#define EVT_ACK 7



typedef struct  tipo_auto tauto;

struct tipo_auto
{
    int id_cola_msg;
    char nombre_pais[LARGO];
    char nombre_piloto[LARGO];
    int numero_auto;
    int escuderia;
    int posicion;
    int puntos;
    int estado;
    int vueltas_perdidas;

};



#endif

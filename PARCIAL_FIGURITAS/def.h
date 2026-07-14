#ifndef _DEF_H
#define _DEF_H

#define TRUE 1
#define FALSE 0

#define DESDE_JUGADORES 2
#define HASTA_JUGADORES 4

#define DESDE_VUELTA 1
#define HASTA_VUELTA 5


#define PATH_CLAVE "/bin/ls"
#define ENTERO_CLAVE 33
#define LARGO 255
#define VERDE 1
#define ROJO 0
#define NOMBRE_FILE "buffer.dat"

#define FILE_SINCRONISMO "sincro.dat"
#define CLAVE_BASE 1

#define MONTO_INICIAL 1600
#define COSTO_PAQUETE 50

#define CANTIDAD_PAQUETES 50
#define CANTIDAD_FIGURITAS 70

#define FIGURITAS_NORMAL_DESDE 10
#define FIGURITAS_NORMAL_HASTA 60

#define FIGURITAS_ESPECIAL_DESDE 2
#define FIGURITAS_ESPECIAL_HASTA 30

#define FIGURITAS_SOBRE 5

#define OPCION_NORMAL 1
#define OPCION_ESPECIAL 2


#define MSG_NADIE 0
#define MSG_KIOSCKO 1
#define MSG_JUGADOR 2

/*eventos para cola de menajes*/

#define EVT_NINGUNO 0
#define EVT_COMPRO 1
#define EVT_FIN 2
#define EVT_SEGUIR_COMPRANDO 3
#define EVT_INICIO 100
#define EVT_INICIO_ACK 250



typedef struct  tipo_jugador tjugador;

struct tipo_jugador
{
    int id_cola_msg;
    int numero_jugador;
    int saldo_jugador;
    /*int num_vuelta_jugador;del 1 al 5, 1 compra 1 paquete, 2 compra 2 paquetes, 3,4 o 5 no compra nada y pasa turno*/
    int figuritas_jugador[70];/*la posicion es el num de la figurita, si es 0 no la tiene, 1 tiene 1 y asi*/
    int cantJugadores;
    
};



#endif

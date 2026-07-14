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

#define MONTO_INICIAL_MIN 500
#define MONTO_INICIAL_MAX 800

#define MAX_EQUIPO 2
#define MSG_NADIE 0
#define MSG_CANCHA 1
#define MSG_EQUIPO 2

/*eventos para cola de menajes*/

#define EVT_NINGUNO 0
#define EVT_TIRO 1
#define EVT_RTA_TIRO 2
#define EVT_INICIO 200
#define EVT_GOL 3
#define EVT_TIRO_LIBRE 4
#define EVT_PALO 5
#define EVT_FUERA 6
#define EVT_LATERAL 7
#define EVT_INICIO_ACK 250
#define EVT_FIN 100
#define EVT_SEGUIR_PATEANDO 400


typedef struct  tipo_equipo tequipo;

struct tipo_equipo
{
    char nombre_equipo[LARGO];
    int numero_equipo;
    int num_pantalla;
    int num_random;
    int goles_umbral;
    int goles_acertados;
    int id_cola_msg;



};



#endif

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
#define NO_LEIDO 0
#define LEIDO 1
#define NOMBRE_FILE "buffer.dat"

#define FILE_SINCRONISMO "sincro.dat"
#define CLAVE_BASE 1

#define HORA_INICIAL_RELOJ 1 
#define HORA_MAXIMA_RELOJ 24

#define HORA_INICIAL_LABORAL 10 
#define HORA_MAXIMA_LABORAL 16

#define MAX_CAJAS 4
#define MSG_NADIE 0
#define MSG_ENTIDAD_FINANCIERA 1
#define MSG_USUARIO 2

/*eventos para cola de menajes*/

#define EVT_NINGUNO 0
#define EVT_CONSULTA 1
#define EVT_SUELDO 9
#define EVT_GANANCIA 10
#define EVT_RTA_CONSULTA 2
#define EVT_INICIO 200
#define EVT_RETIRO 3
#define EVT_RTA_RETIRO_OK 4
#define EVT_RTA_RETIRO_NOK 40
#define EVT_INGRESO 5
#define EVT_RTA_INGRESO_OK 6
#define EVT_RTA_INGRESO_NOK 60
#define EVT_CONSULTA_GANANCIA 7
#define EVT_RTA_CONSULTA_GANANCIA 8
#define EVT_DIA 300

#define EVT_INICIO_ACK 250
#define EVT_FIN 100
#define EVT_SEGUIR_INVIRTIENDO 400


/*typedef struct  tipo_usuario tusuario;
typedef struct  tipo_producto tproducto;*/

/*
struct tipo_usuario
{
    int numero_caja;
    int cant_minima;
    int cant_maxima;
};*/

typedef struct tproducto
{
    int precio_producto;
    char descripcion_producto[LARGO];
    int unidades_producto;
    int leido;
}tproducto;



#endif

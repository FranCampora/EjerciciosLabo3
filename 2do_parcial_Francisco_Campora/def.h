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

#define HORA_INICIAL_RELOJ 1 
#define HORA_MAXIMA_RELOJ 24

#define HORA_INICIAL_LABORAL 10 
#define HORA_MAXIMA_LABORAL 16

#define MAX_USUARIOS 3
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


typedef struct  tipo_usuario tusuario;

struct tipo_usuario
{
    int id_cola_msg;
    int numero_usuario;
    int saldo_usuario;
    char nombre_usuario[LARGO];
    int dias_invertido;/*el mes es de 31 dias*/
    char nombre_entidad_financiera[LARGO];
    int hora_minima_laboral;
    int hora_maxima_laboral;
    int ganancia;


};



#endif

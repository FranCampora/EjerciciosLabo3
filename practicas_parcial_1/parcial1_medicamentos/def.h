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

#define LEIDO 1
#define NO_LEIDO 0
#define NOMBRE_FILE "buffer.dat"

#define FILE_SINCRONISMO "sincro.dat"
#define CLAVE_BASE 1

#define HORA_INICIAL_RELOJ 1 
#define HORA_MAXIMA_RELOJ 24

#define HORA_INICIAL_LABORAL 10 
#define HORA_MAXIMA_LABORAL 16

#define MAX_PROCESOS 3
#define OPCION_FIN_MENU 0
#define SENIAL_FIN 99


typedef struct tmedicamento
{
    int color_medicamento;
    char descripcion_medicamento[LARGO];
    int cantidad_medicamento;
    int leido;
}tmedicamento;


#endif

#ifndef _DEF_H
#define _DEF_H

#define TRUE 1
#define FALSE 0

#define DESDE 1
#define HASTA 99

#define LOTE 10
#define PATH_CLAVE "/bin/ls"
#define ENTERO_CLAVE 33
#define LARGO 255
#define VERDE 1
#define CANTIDAD_PARTIDA 5
#define INTERVALO_PRODUCTO 50
#define INTERVALO_PARTIDA 200
#define NOMBRE_FILE "buffer.dat"
#define FILE_SINCRONISMO "sincro.dat"

#define MAX_USUARIOS 3

#define MIN_INVERSION 10000


typedef struct{
    
    char nombreJugador[LARGO];
    int numeroPensado;
    int estadoAcierto;
}juego;


/*typedef struct tipo_dato dato;*/

#endif

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

#define LIMITE_LIBERACION 10
#define MIN_TIEMPO_PEAJE 100
#define MAX_TIEMPO_PEAJE 5000
#define MIN_TIEMPO_AUTOS 400
#define MAX_TIEMPO_AUTOS 6000


typedef struct{
    int cantidadAutos;
    int numeroVia;
}via;

/*typedef struct tipo_dato dato;*/

#endif

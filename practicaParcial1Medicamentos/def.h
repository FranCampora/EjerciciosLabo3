#ifndef _DEF_H
#define _DEF_H

#define TRUE 1
#define FALSE 0
#define DESDE 0
#define HASTA 50
#define LOTE 10
#define PATH_CLAVE "/bin/ls"
#define ENTERO_CLAVE 33
#define LARGO 255
#define VERDE 1
#define CANTIDAD_PARTIDA 5
#define INTERVALO_PRODUCTO 50
#define INTERVALO_PARTIDA 200
#define NOMBRE_FILE "buffer.dat"

#define MIN_CAJA1 1
#define MAX_CAJA1 5
#define MIN_CAJA2 6
#define MAX_CAJA2 10
#define MIN_CAJA3 11


typedef struct{
    
    int cantidad;
    char descripcion[LARGO];
    int precio;
}Producto;

#endif

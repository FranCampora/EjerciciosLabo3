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

#define UMBRAL_PRECIO 20000


typedef struct{
    
    int cantidad;
    char descripcion[LARGO];
    int precio;
}Producto;

#endif

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
#define CORTO 10
#define VERDE 1
#define CANTIDAD_PARTIDA 5
#define INTERVALO_PRODUCTO 50
#define INTERVALO_PARTIDA 200
#define NOMBRE_FILE "buffer.dat"

#define PRECIO_MENU_A 1000
#define PRECIO_MENU_B 2000
#define PRECIO_MENU_C 3000


typedef struct{
    char menu[CORTO];/* A, B o C */
    int precio;
    int postre;/* 1 si es postre, 0 si no */
}Producto;

typedef struct
{
    Producto lista[LARGO];
    int contador;
    int totalRecaudado;
}Historial;


#endif

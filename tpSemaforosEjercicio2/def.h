#ifndef _DEF_H
#define _DEF_H

#define TRUE 1
#define FALSE 0
#define DESDE 100
#define HASTA 500
#define LOTE 10
#define PATH_CLAVE "/bin/ls"
#define ENTERO_CLAVE 33
#define LARGO 255
#define VERDE 1
#define CANTIDAD_PARTIDA 5
#define INTERVALO_PRODUCTO 50
#define INTERVALO_PARTIDA 200

#define DEPOSITO_MINIMO 10
#define DEPOSITO_MAXIMO 20
#define IMPORTE_MINIMO 100
#define IMPORTE_MAXIMO 500
#define CANTIDAD_DE_CAJEROS 3

typedef struct{ 
    int cantEfectivo;
    int cantCheque;
    int totalEfectivo;
    int totalCheque;
} Totales;


#endif

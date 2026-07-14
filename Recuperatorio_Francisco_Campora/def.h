#ifndef _DEF_H
#define _DEF_H

#define TRUE 1
#define FALSE 0

#define DESDE 10
#define HASTA 30


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

#define MAX_CONTROLES 3





typedef struct 
{
    
    int numero_lote_bebidas;/*es un id de las cargas de las bebidas*/
    int cantidad_bebidas;
    int alcohol;/*1 es que tiene alcohol, 0 no tiene*/
    char nombre_bebida[LARGO];
    char descripcion_bebida[LARGO];
    int leido; /*1 es que ya se leyo el lote, 0 no se leyo el lote*/


}Bebidas;



#endif

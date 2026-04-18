#ifndef _DEFINE_H
#define _DEFINE_H

#define TRUE 1
#define FALSE 0

#define ARCHIVO "usuarios.dat" /*Nombre del archivo de almacenamiento*/
/*#define PRECIOS "preciosDolar.dat" */
#define CAJEROS 2 /*instancias del main2*/
#define USUARIOS_MAX 4 /*Cantidad de usuarios del banco*/
#define SALDO_INICIAL 500000 /*Saldo inicial de los usuarios*/

#define COMANDO "/bin/ls" /*Comando conocido para ftok()*/
#define ENTERO 12 /*Entero para ftok()*/

#define ROJO 0 /*Semaforo envia el proc a la cola*/
#define VERDE 1 /*Semaforo permite el ingreso*/

/*Para char*/
#define LARGO 150
#define CORTO 50
#define LINEA 255+1

#define MS 1000 /*Multiplicar usleep() por la cantidad de ms requerida*/

#define MEP 1150 /*Valor del dolar MEP*/
#define OFICIAL 1100 /*Valor del dolar Oficial*/
#define BLUE 1250 /*Valor del dolar Blue*/

typedef struct{
	int nro; /*Numero del usuario*/
	int saldo; /*Saldo del usuario, manejamos todos int*/
}Usuario;


#endif



#ifndef _FUNCIONES_H
#define _FUNCIONES_H

int devolverNumAleatorio(int desde, int hasta);
void mostrarVec(int numeros[], int dim);

/*Ingresos*/
int ingresarUsuario();
int menu();
int ingresarTipoDolar();
int ingresarMontoDolar();
int ingresarMontoPesos();

int calcularPrecioCompra(int tipoDolar, int cantidad);



#endif



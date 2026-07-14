#ifndef _FUNCIONES_H
#define _FUNCIONES_H

int devolverNumAleatorio(int desde, int hasta);
int devolverAleatorioSinRepetir(int desde, int hasta, int *vector, int* size);
int* generarVecRandom(int desde, int hasta, int cantidad);
int funcionPrimos(int n);
void mostrarVec(int numeros[], int dim);
char** separar(char *mensaje, char *caracter);

int todos_terminaron(int *array, int dim);

/*Ingresos*/
int ingresarCliente();
int ingresarMonto();
int menu();


#endif



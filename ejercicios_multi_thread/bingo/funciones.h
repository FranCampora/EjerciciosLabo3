#ifndef _FUNCIONES_H
#define _FUNCIONES_H

	
	int cargarInt(char *mensaje);
	int cargarPrecio(char *mensaje);
	void cargarData(char *mensaje, char *data);
	void actualizarArchivo(char *nombreArchivo, int buffer);
	void crearArchivoSincronismo(char *nombreArchivo, int valorInicial);
	void esperarArchivoSincronismo(char *nombreArchivo, int valorEsperado);

	int devolverNumAleatorio(int desde, int hasta);
	int devolverAleatorioSinRepetir(int desde, int hasta, int *vector, int* size);
	int funcionPrimos(int n);
	int menu();
	void mostrarVec(int numeros[], int dim);
#endif

#ifndef _FUNCIONES_H
#define _FUNCIONES_H

	
	int cargarInt(char *mensaje);
	int cargarPrecio(char *mensaje);
	void cargarData(char *mensaje, char *data);
	void actualizarArchivo(char *nombreArchivo, int buffer);
	void crearArchivoSincronismo(char *nombreArchivo, int valorInicial);
	void esperarArchivoSincronismo(char *nombreArchivo, int valorEsperado);
	void funcionTestParametroFuncion(int valor1, int valor2, int (*funcionSecundaria)(int,int));
	void funcionTest();
	void funcionTestParametrosInt(int valor1, int valor2);
	int sumar(int valor1, int valor2);
	int restar(int valor1, int valor2);

#endif

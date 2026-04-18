#include "stdio.h"
#include "stdlib.h"
#include "funciones.h"
#include "def.h"
#include "global.h"
#include "time.h"
#include "archivos.h"

int main(int argc, char *argv[]){
	int idNumeroAl=0;

	srand(time(NULL));

	idNumeroAl=idNumeroAleatorio(DESDE,HASTA);

	/*GENERO*/
	printf("\n Numero: %d\n",idNumeroAl);
	/*escriboo*/
	if(abrirArchivo("resultadoNumAl.txt","w")){
		char texto[50];
		sprintf(texto,"\nNumero escrito: %d\n", idNumAl);
		escribirArchivo(texto);
		cerrarArchivo();
	}
	/*leeo
	if(abrirArchivo("resultado.txt","r")){
		printf("\nLo que leo del file es:\n");
		leerArchivo();
		cerrarArchivo();
	}*/
	return 0;
}

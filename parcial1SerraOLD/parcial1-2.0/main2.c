#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

#include "define.h"
#include "semaforo.h"
#include "archivos.h"
#include "funciones.h"


int main(int argC, char *argv[])
{
	int i, nroCajero, id_semaforo, nroUsuario, opcion, tipoDolar;
	int montoDolar, montoDeposito, precioCompra, nuevoSaldo;
	Usuario usuarios[USUARIOS_MAX];
	char buffer[LINEA];

/*Verifico que se haya ingresado el numero de cajero*/
	if(argC != 2) {
		printf("\nIngresa como parametro el numero del cajero \n%s {numero}\n\n", argv[0]);
			exit(1);
	}

	nroCajero = atoi(argv[1]); /*Convierto el numero de cajero a int*/
	
/*Verifico que el numero de cajerjo sea correcto*/
	if(nroCajero < 1 || nroCajero > CAJEROS){
		printf("\nNumero de cajero incorrecto\nDebe ser 1 o 2\n\n");
		exit(1);
	}

	if(existe(ARCHIVO) == FALSE){
		printf("\nCajero inhabilitado -- Inicie el proceso banco\n");
		exit(1);
	}

	printf("\n======= Cajero numero %d =========\n",nroCajero);
	

	id_semaforo = crearSemaforo(); 
/*Devuelve el id del semaforo inicializado por el banco.*/
	
	while(1){
		nroUsuario = ingresarUsuario();
		if(nroUsuario == 0)
			exit(0);

		opcion = menu();

		esperarSemaforo(id_semaforo);
/*Cargo el array de struct Usuario con los datos del archivo*/
		if(abrirArchivo(ARCHIVO,"r"))
		{
			for(i=0; i < USUARIOS_MAX; i++)
			{
				leerLinea(buffer);
				sscanf(buffer,"%d-%d", &usuarios[i].nro, &usuarios[i].saldo);
			}
			cerrarArchivo();
		}

		if(opcion == 1) /*Opcion 1 comprar dolares*/
		{
			tipoDolar = ingresarTipoDolar();
			montoDolar = ingresarMontoDolar();

			precioCompra = calcularPrecioCompra(tipoDolar, montoDolar);
			nuevoSaldo = usuarios[nroUsuario-1].saldo - precioCompra;

		}else if(opcion == 2){ /*Opcion 2 ingresar dinero*/
			montoDeposito = ingresarMontoPesos();

			nuevoSaldo = usuarios[nroUsuario-1].saldo + montoDeposito;

			printf("\nEl usuario %d ha ingresado $%d\n",nroUsuario,montoDeposito);
		}else{ /*Opcion 3 finalizar...*/
			printf("\nPrograma finalizado...\n");
			exit(0);
		}

		if(nuevoSaldo > 0)
		{
			usuarios[nroUsuario-1].saldo = nuevoSaldo;

			if(abrirArchivo(ARCHIVO, "w"))
			{
				for(i = 0; i < USUARIOS_MAX; i++)
				{
					sprintf(buffer, "%d-%d\n",usuarios[i].nro, usuarios[i].saldo);
					escribirArchivo(buffer);
				}
				cerrarArchivo();
				printf("Operacion realizada con exito. Nuevo saldo: $%d\n", usuarios[nroUsuario-1].saldo);
			}
		}else
			printf("ERROR: Saldo insuficiente\n");

		levantarSemaforo(id_semaforo);
		usleep(200 * MS);
	}
	return 0;
}



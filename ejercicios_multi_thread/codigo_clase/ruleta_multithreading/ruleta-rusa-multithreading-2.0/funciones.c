#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "funciones.h"
#include "define.h"
#include "global.h"
#include "archivos.h"


int devolverNumAleatorio(int desde, int hasta)
{
	int num = 0;
	num = rand()%(hasta - desde + 1)+ desde;
	/*srand(time(NULL)); una sola vez antes de llamar a la funcion*/
	return num;
}

int devolverAleatorioSinRepetir(int desde, int hasta, int *vector, int* size){

	int found = 0;
	int numero = 0;
	int i = 0;
	
	do{
		found = 0;		
		numero = devolverNumAleatorio(desde,hasta);
		for(i = 0; i < *size; i++){
			
			if(vector[i] == numero){
				found = 1;
			}
		}
	} while(found);

	vector[*size]= numero;
	(*size)++;
	return numero;
}

int funcionPrimos(int n){

	int c=0,c2=0,res=0,nc=0, total=0;
	for(c=1;c<=n;c++)
	{
		for(c2=1;c2<=c;c2++)
		{
			res=c%c2;
			if(res==0)
				nc=nc+1;
		}
		if(nc==2)
			total+=c;
		nc=0;
	}
	return total;	
}



void mostrarVec(int numeros[], int dim)
{
	int i;
	for(i=0;i<dim;i++)
	{
		printf("[%d] ", numeros[i]);
	}
}



int ingresarCliente(){
	int num;
	
	printf("\nIngrese el numero de cliente (0 para finalizar): ");
	scanf("%d",&num);

/*Si es 0 termina*/
/*	if(num == 0){
		printf("Programa finalizado\n");
		exit(0);
	}
*/
/*Validación*/
	while((num < 0) || (num > 110))
	{
		printf("\nNumero incorrecto\n");
		printf("Ingrese el numero de cliente (0 para finalizar): ");
		scanf("%d",&num);
	}
	return num;
}

int ingresarMonto(){
	int num;
	
	printf("\nIngrese el monto: $");
	scanf("%d",&num);

/*Validación*/
	while((num < 1))
	{
		printf("\nNumero incorrecto\n");
		printf("Ingrese el monto: $");
		scanf("%d",&num);
	}
	return num;
}



int menu()
{
	int opcion;

	printf("\n1. Consultar saldo\n2. Deposito\n3. Extracción\n0. Salir\nOpcion: ");
	scanf("%d",&opcion);

	while((opcion < 0) || (opcion > 3))
	{
		printf("\nOpcion incorrecta\n");
		printf("1.Consultar saldo\n2. Deposito\n3. Extracción\n0. Salir\nOpcion: ");
		scanf("%d",&opcion);
	}
	return opcion;
}




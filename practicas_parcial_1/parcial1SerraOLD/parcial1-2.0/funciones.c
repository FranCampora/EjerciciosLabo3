#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "funciones.h"
#include "define.h"
#include "archivos.h"


int devolverNumAleatorio(int desde, int hasta)
{
	int num = 0;
	num = rand()%(hasta - desde + 1)+ desde;
	/*srand(time(NULL)); una sola vez antes de llamar a la funcion*/
	return num;
}

void mostrarVec(int numeros[], int dim)
{
	int i;
	for(i=0;i<dim;i++)
	{
		printf("%d, ", numeros[i]);
	}
}


int ingresarUsuario(){ /*Devuelve un numero de usuario valido*/
	int num;
	
	printf("\nIngrese numero de usuario (0 para finalizar): ");
	scanf("%d",&num);

/*Validación*/
	while((num < 0) || (num > USUARIOS_MAX))
	{
		printf("\nNumero de usuario incorrecto\n");
		printf("Ingrese numero de usuario (0 para finalizar): ");
		scanf("%d",&num);
	}
	return num;
}

int menu() /*Devuelve una opcion valida*/
{
	int opcion;

	printf("\n1.Comprar dolares\n2. Ingresar dinero\n3. Finalizar\nOpcion: ");
	scanf("%d",&opcion);

	while((opcion < 1) || (opcion > 3))
	{
		printf("\nOpcion incorrecta\n");
		printf("1.Comprar dolares\n2. Ingresar dinero\n3. Finalizar\nOpcion: ");
		scanf("%d",&opcion);
	}
	return opcion;
}

/*Devuelve un tipoDolar valido (1.MEP 2.Oficial 3.Blue)*/
int ingresarTipoDolar() 
{
	int num;
	
	printf("\nIngrese el tipo de dolar\n1. MEP\n2. Oficial\n3. Blue\n0. FINALIZAR\nOpcion: ");
	scanf("%d",&num);

/*Si es 0 termina*/
	if(num == 0){
		printf("Programa finalizado\n");
		exit(0);
	}
/*Validación*/
	while((num < 1) || (num > 3))
	{
		printf("\nTipo de dolar incorrecto\n");
		printf("Ingrese el tipo de dolar\n1. MEP\n2. Oficial\n3. Blue\n0. FINALIZAR\nOpcion: ");
		scanf("%d",&num);
	}
	return num;
}

int ingresarMontoDolar()
{
	int num;
	
	printf("\nIngrese la cantidad de dolares a comprar: U$D");
	scanf("%d",&num);

/*Validación*/
	while((num < 1) || (num > 10000))
	{
		printf("\nNo puede comprar esa cantidad\n");

		printf("Ingrese la cantidad de dolares a comprar: U$D");
		scanf("%d",&num);
	}
	return num;
}


int ingresarMontoPesos()
{
	int num;
	
	printf("\nIngrese el monto a depositar: $");
	scanf("%d",&num);

/*Validación*/
	while((num < 1) || (num > 1000000))
	{
		printf("\nNo puede ingresar mas de $1.000.000 por cajero\n");

		printf("Ingrese el monto a depositar: $");
		scanf("%d",&num);
	}
	return num;
}

int calcularPrecioCompra(int tipoDolar, int cantidad)
{
	int pesos = 0;

	if(tipoDolar == 1){
		pesos = cantidad * MEP;
	}else if(tipoDolar == 2){
		pesos = cantidad * OFICIAL;
	}else if(tipoDolar == 3){
		pesos = cantidad * BLUE;
	}

	return pesos;
}


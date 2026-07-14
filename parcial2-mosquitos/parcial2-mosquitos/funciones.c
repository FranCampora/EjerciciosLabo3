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

int* generarVecRandom(int desde, int hasta, int cantidad)
{
	/* El 'desde' no es incluido en la seleccion del numero aleatorio. Ejemplo: para obtener el valor '0' debo arrancar desde el valor '-1' */ 

	int n,num,i,j,hastaRand,temp;
	int* vector = (int*) malloc(cantidad*sizeof(int));
	
	desde--;
	if((hasta-desde) < cantidad)
	{		
		printf("deben haber mas numeros que cantidades, %d es menor que %d\n",(hasta-desde),cantidad);
		free(vector);
		exit (0);
	}
    

	hastaRand = (hasta-desde)/cantidad ;

	temp = desde;
	
	for (n=0 ; n < cantidad ; n++)
	{	
		num = (rand()%(hastaRand)) + 1;
		vector[n] = temp + num;
		temp = temp + hastaRand;
	}

	/* printf("Reordeno---------\n") */	
	for (i=cantidad-1 ; i > 0 ; i--)
	{
		num = rand()%(i);
		j = vector[i];
		vector[i] = vector[num];
		vector[num] = j;
	}
	return vector;

	/*En el main ejecutar
	
	int* vector;
	vector = generarRandom(desde,hasta,cantidad);
	for (n=0; n<cantidad ; n++)
	{	
		printf("%d \n", vector[n]);	
	}
	*/
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


char** separar(char *mensaje, char *caracter){
	int count = 0; 
	char** array;	

	/*creo temp para poder primero contar la cantidad de iteraciones*/
	char *temp = malloc(strlen(mensaje));
	char *token;
	token = NULL;
	sprintf(temp,"%s\n",mensaje);
	token = strtok(temp, caracter);

	/*Cuento las iteraciones (asi se el tamaño del array que necesito)*/
	while (token != NULL)
	{
		token = strtok(NULL,caracter);
		count++;
	}

	/* una vez que tengo el tamaño del array creo el array con un maloc */
	array=(char**)malloc(sizeof(char*)*count);

	/*empiezo a iterar por el texto */
	count = 0;
	token = strtok(mensaje, caracter);
	while (token != NULL)
	{
	    /* le asigno su tamaño */
		array[count]= (char*)malloc(sizeof(char)*strlen(token));

	    /*le paso al vector el dato */
		sprintf(array[count],"%s\n", token);

		/*printf("pasando a %d: %s\n",count,array[count]);*/
		token = strtok(NULL,caracter);

		count++;
	}
	return array; /*Devuelve un array con los strings separados por el caracter*/

	/* EJEMPLO de ejecucion en el main
	char **array;
	char test[100];

	sprintf(test,"Esto|es|test");

	array= separar(test,"|");
	Devuelve el array ["Esto", "es", "test"]

	for(i=0 ; i<CANTIDAD ; i++)
	{
		printf("dato %d: %s\n",i,array[i]);
	}

	------------------------ ----------------
	No es tan útil usar el array para cola de mensajes porque hay q limpiarlo en cada vuelta.
	Mejor usar strtok() directamente
	EJEMPLO

	char *nombre_insumo, *valor;

	recibir_mensaje(id_colamsg, dest, &msg);
	(El mensaje viene como "hojas|10")

	nombre_insumo = strtok(msg.char_mensaje, "|");
	valor = strtok(NULL, "|");
	(strtok modifica directamente el msg-char_mensaje por eso no hay que asignarles memoria ni limpiarlos) 

	cant_insumo = atoi(valor);

	Ahora ya puedo "nombre_insumo" y "cant_insumo" para ese momento
	*/
}


int todos_terminaron(int *array, int dim)
{
	int i;
	for(i=0; i<dim; i++)
	{
		if(array[i] == 0)
			return FALSE;
	}
	return TRUE;
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




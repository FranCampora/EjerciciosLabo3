#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/ipc.h"
#include "sys/sem.h"
#include "time.h"
#include "unistd.h"
#include "sys/shm.h"

#include "funciones.h"
#include "archivos.h"
#include "def.h"
#include "memoria.h"
#include "semaforo.h"
#include "clave.h"
#include "global.h"
#include "archivos.h"


int cargarInt(char *mensaje){
	int num = 0;
	printf("%s", mensaje);
	scanf("%d", &num);
	return num;
}

int cargarPrecio(char *mensaje){
	int num = 0;
	printf("%s", mensaje);
	scanf("%d", &num);
	return num;
}


void cargarData(char *mensaje, char *data){
	printf("%s",mensaje);
	scanf(" %[^\n]", data);
}


void actualizarArchivo(char *nombreArchivo, int buffer){
	char szBuffer[LARGO];
	if (abrirArchivo(nombreArchivo,"w") == TRUE)
	{
		memset(szBuffer,0x00,sizeof(szBuffer));
		sprintf(szBuffer, "%d\n", buffer);
		escribirArchivo(szBuffer);
		cerrarArchivo();
	}
}

void crearArchivoSincronismo(char *nombreArchivo, int valorInicial){
	char szBuffer[LARGO];
	if (abrirArchivo(nombreArchivo,"w") == TRUE)
	{
		memset(szBuffer,0x00,sizeof(szBuffer));
		sprintf(szBuffer, "%d\n", valorInicial);
		escribirArchivo(szBuffer);
		cerrarArchivo();
		printf("Archivo de sincronismo llamado %s creado con valor: %d\n", nombreArchivo, valorInicial);
		
	}
}

void esperarArchivoSincronismo(char *nombreArchivo, int valorEsperado){
	char szBuffer[LARGO];
	int valorLeido;
	int continuar;
	
	continuar = FALSE;
	valorLeido = -1; /*valor por defecto, no es 0 ni 1 para evitar confusiones*/

	printf("SINCRONISMO DE PROCESO VIA FILE");
	while (continuar == FALSE)
	{
		if (abrirArchivo(nombreArchivo,"r") == FALSE)
		{
			printf("Error SINCRONISMO. ARCHIVO %s NO ENCONTRADO. El proceso inicializador NO COMENZO. Esperando...\n", nombreArchivo);
			sleep(1); /* Espera 1 segundo antes de volver a intentar */
		}else
		{
			if (leerArchivo(szBuffer)==FALSE)
			{
				printf("Error SINCRONISMO. Archivo %s detectado pero VACIO O ILEGIBLE. Esperando...\n", nombreArchivo);
				cerrarArchivo();
			}else
			{
				sscanf(szBuffer, "%d\n", &valorLeido);
				cerrarArchivo();

				if (valorLeido != valorEsperado)
				{
					printf("ERROR SINCRONISMO. Valor del archivo de sincronismo %s es: %d. Se esperaba: %d. Esperando...\n", nombreArchivo, valorLeido, valorEsperado);
				}
				else
				{
					printf("EXITO SINCRONISMO. Valor del archivo de sincronismo %s es: %d. Continuando...\n", nombreArchivo, valorLeido);
					continuar = TRUE;
				
				}
			}
		}
		if (continuar == FALSE)
		{
			sleep(1); /* Espera 1 segundo antes de volver a intentar */
		}
		printf("--------------------------------------------------\n");
		printf("SINCRONISMO FINALIZADO. Valor del archivo de sincronismo %s es: %d. Continuando...\n", nombreArchivo, valorLeido);
	}
}

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

int menu()
{
	int opcion;

	printf("\n1. Consultar saldo\n2. Ingreso dinero\n3. Retiro dinero\n 4. Consultar ganancia\n0. Salir\nOpcion: ");
	scanf("%d",&opcion);

	while((opcion < 0) || (opcion > 4))
	{
		printf("\nOpcion incorrecta\n");
		printf("1.Consultar saldo\n2. Ingreso dinero\n3. Retiro dinero\n 4. Consultar ganancia\n0. Salir\nOpcion: ");
		scanf("%d",&opcion);
	}
	return opcion;
}
/*
cliente* buscar_cliente(char *szCodigo, cliente *clientes, int cantClientes)
{
    recibe el codigo del cliente que tiene que buscar, el array donde tiene que buscar, el maximo osea la cantidad del array asi tiene corte
    int codBuscado;
    int i;

    codBuscado = atoi(szCodigo); convierte el char_mensaje a int 
    i = 0;

    while (i < cantClientes && clientes[i].codigo_cliente != codBuscado)
    {
        i++;
    }

    if (i < cantClientes)
    {
        return &clientes[i]; encontrado: devuelve puntero al elemento del array 
    }else
    {
        return NULL; no encontrado 
    }
}*/

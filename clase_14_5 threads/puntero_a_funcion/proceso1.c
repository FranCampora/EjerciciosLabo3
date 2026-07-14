#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <pthread.h>


#include "def.h"
#include "global.h"
#include "semaforo.h"
#include "archivos.h"
#include "funciones.h"
#include "clave.h"
#include "memoria.h"
#include "mensajes.h"
#include "threads.h"

int main(int argc, char  *argv[])
{
    int num1, num2;
    num1 = cargarInt("Ingrese el primer numero: ");
    num2 = cargarInt("Ingrese el segundo numero: ");

    /*
    void (*punteroFuncion)() = &funcionTest;/*declaro una funcion llamada punteroFuncion que es un puntero a la funcion llamada funcionTest. Estoy pasandole
    la direccion de memoria de funcionTest*/
    /*

    punteroFuncion();/*llamo a la funcionTest a traves del puntero a funcion*/
    /*
    void (*punteroFuncionParametros)(int, int) = &funcionTestParametrosInt;/*declaro una funcion llamada punteroFuncionParametros que es un puntero a la funcion llamada funcionTestParametros. Estoy pasandole
    la direccion de memoria de funcionTestParametros*/
    
    /*
    punteroFuncionParametros(10, 20);/*llamo a la funcionTestParametros a traves del puntero a funcion, pasandole los parametros 10 y 20*/
    
    funcionTestParametroFuncion(num1, num2, &sumar);/*llamo a la funcionTestParametroFuncion pasandole los parametros 10, 20 y la direccion de memoria de la funcion sumar. 
    La funcionTestParametroFuncion va a ejecutar la funcion sumar con los parametros 10 y 20 y va a imprimir el resultado*/
    /*1er : num1*/
    /*2do : num2*/
    /*3er : direccion de memoria de la funcion sumar*/
    

    return 0;
}

Francisco Campora Recuperatorio 1er parcial
11/06/2026

Los procesos son logistica.c y controlBebidas.c 
El que se va a ejecutar 3 veces es el controlBebida.c pasandole como parametetro el numero del control del 1 al 3

Se puede ingresar la cantidad de bebidas manualmente o que sea aleatorio, pero siempre las opciones van a ser 10 20 o 30

Logica de ejecuccion de los ejecutables de los procesos.
1. Ejecutar el ./logistica y los ./controlBebidas + el numero del proceso
2. cargar los datos deseados para las bebidas y se va ver automaticamente en la terminal del proceso correspondiente

Las terminales que no reciben una cantidad no van a mostrar nada porque van a quedar en leido, por las primeras lineas del while

Pregunta: Para que se utilizan los semaforos:
Los semaforos son un herramienta que utilizamos para poder generar concurrencia entre los procesos, 
permiten sincronizarlos para que no se superpogan cuando un proceso está ejecutando su seccion critica. 
Por ejemplo, si tengo dos procesos que intentan escribir el mismo archivo a la vez, podría obtenerse un resultado no esperado, 
por eso es necesario sincronizarlos para que solo un proceso escriba en el archivo por vez.
Se utiliza un usleep luego de levantar el semaforo para que el proceso libere correctamente los recursos 
y le de tiempo a otros procesos a solicitar el semáforo.


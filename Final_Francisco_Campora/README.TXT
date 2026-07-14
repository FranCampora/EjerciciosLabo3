FINAL FRANCISCO CAMPORA 13/7/2026

Los procesos principales son coliseo.c y luchador.c

Los char de los mensajes de cola de mensajes lo extraigo con sscanf.

Primero ejecutar el file coliseo.c y luego luchador.c

Considero 
A tanto los imperio como gladiadores como luchadores y comparten la misma struct

Las armas solo pueden ser de 3 tipo, espada, lanza y otro

Los ataques pasan por el coliseo

El emperador no es un luchador mas, sino que es el "dueño" del coliseo por eso su decision esta en coliseo.c

Respuesta a la pregutna teorica:
Un puntero a función es una variable que su contenido apunta a la dirección de memoria de una función. Nos sirve para llamar a una función desde su dirección de memoria. Para invocar la función usamos los () sobre la variable que contiene la dirección de memoria de la función. Para utilizar esa variable usamos el nombre de la variable declarada pasando los parámetros necesarios de la función.



Un callback es una forma de utilizar un puntero a funcion, donde en una funcion uno de sus parametros es otra funcion.

La función principal va a tener sus parámetros + la dirección de memoria de la función secundaria. Sirve para que un mismo programa ejecute distintas acciones, donde las diferencias son mínimas y la mayor parte del código se reúsa, entonces podemos eliminar redundancia y aportar abstracción al tener la función principal mas genérica.



La diferencia entre un puntero de un callback es que el puntero a función es una manera de declarar los datos de una variable y el callback es una técnica donde utilizamos un puntero a funcion.




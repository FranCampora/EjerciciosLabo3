CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean pista maritimo

pista: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c pista.c
	$(CC) -o pista global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c pista.c $(FLAGS) $(HEADERS)

maritimo: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c maritimo.c
	$(CC) -o maritimo global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c maritimo.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o pista maritimo *.dat sincro.dat resultado.txt

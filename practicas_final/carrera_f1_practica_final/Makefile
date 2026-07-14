CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean pista autos

pista: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c pista.c
	$(CC) -o pista global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c pista.c $(FLAGS) $(HEADERS)

autos: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c autos.c
	$(CC) -o autos global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c autos.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o pista autos *.dat sincro.dat

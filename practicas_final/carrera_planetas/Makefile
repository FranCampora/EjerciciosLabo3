CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean pista planeta

pista: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c pista.c
	$(CC) -o pista global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c pista.c $(FLAGS) $(HEADERS)

planeta: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c planeta.c
	$(CC) -o planeta global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c planeta.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o pista planeta *.dat sincro.dat *.txt

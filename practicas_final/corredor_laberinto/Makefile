CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean pista muchacho

pista: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c pista.c
	$(CC) -o pista global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c pista.c $(FLAGS) $(HEADERS)

muchacho: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c muchacho.c
	$(CC) -o muchacho global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c muchacho.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o pista muchacho *.dat sincro.dat resultados.txt

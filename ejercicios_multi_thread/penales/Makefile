CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean arquero pateadores

arquero: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c arquero.c
	$(CC) -o arquero global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c arquero.c $(FLAGS) $(HEADERS)

pateadores: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c pateadores.c
	$(CC) -o pateadores global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c pateadores.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o arquero pateadores *.dat sincro.dat

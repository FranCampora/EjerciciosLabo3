CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean tablero mosquito

tablero: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c tablero.c
	$(CC) -o tablero global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c tablero.c $(FLAGS) $(HEADERS)

mosquito: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c mosquito.c
	$(CC) -o mosquito global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c mosquito.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o tablero mosquito *.dat sincro.dat

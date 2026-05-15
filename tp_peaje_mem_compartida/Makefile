CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean autos peaje

autos: global.c clave.c semaforo.c funciones.c archivos.c memoria.c autos.c
	$(CC) -o autos global.c clave.c semaforo.c funciones.c archivos.c memoria.c autos.c $(FLAGS) $(HEADERS)

peaje: global.c clave.c semaforo.c funciones.c archivos.c memoria.c peaje.c
	$(CC) -o peaje global.c clave.c semaforo.c funciones.c archivos.c memoria.c peaje.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *.o autos peaje *.dat sincro.dat

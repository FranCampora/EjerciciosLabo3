CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean colchonera vendedor

colchonera: global.c clave.c semaforo.c funciones.c archivos.c memoria.c colchonera.c
	$(CC) -o colchonera global.c clave.c semaforo.c funciones.c archivos.c memoria.c colchonera.c $(FLAGS) $(HEADERS)

vendedor: global.c clave.c semaforo.c funciones.c archivos.c memoria.c vendedor.c
	$(CC) -o vendedor global.c clave.c semaforo.c funciones.c archivos.c memoria.c vendedor.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *.o colchonera vendedor *.dat sincro.dat

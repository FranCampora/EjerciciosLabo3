CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean logistica controlBebidas

logistica: global.c clave.c semaforo.c funciones.c archivos.c logistica.c
	$(CC) -o logistica global.c clave.c semaforo.c funciones.c archivos.c logistica.c $(FLAGS) $(HEADERS)

controlBebidas: global.c clave.c semaforo.c funciones.c archivos.c controlBebidas.c
	$(CC) -o controlBebidas global.c clave.c semaforo.c funciones.c archivos.c controlBebidas.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o logistica controlBebidas *.dat sincro.dat

CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean caja superDerivador

caja: global.c clave.c semaforo.c funciones.c archivos.c caja.c
	$(CC) -o caja global.c clave.c semaforo.c funciones.c archivos.c caja.c $(FLAGS) $(HEADERS)

superDerivador: global.c clave.c semaforo.c funciones.c archivos.c superDerivador.c
	$(CC) -o superDerivador global.c clave.c semaforo.c funciones.c archivos.c superDerivador.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o caja superDerivador buffer.dat

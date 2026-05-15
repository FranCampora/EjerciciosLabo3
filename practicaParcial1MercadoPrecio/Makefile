CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean caja derivador

caja: global.c clave.c semaforo.c funciones.c archivos.c cajaMercadoPrecio.c
	$(CC) -o caja global.c clave.c semaforo.c funciones.c archivos.c cajaMercadoPrecio.c $(FLAGS) $(HEADERS)

derivador: global.c clave.c semaforo.c funciones.c archivos.c derivadorMercadoPrecio.c
	$(CC) -o derivador global.c clave.c semaforo.c funciones.c archivos.c derivadorMercadoPrecio.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o caja derivador buffer.dat

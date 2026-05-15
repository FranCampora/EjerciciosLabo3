CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean piensoNumero adivinoNumero

piensoNumero: global.c clave.c semaforo.c funciones.c archivos.c memoria.c piensoNumero.c
	$(CC) -o piensoNumero global.c clave.c semaforo.c funciones.c archivos.c memoria.c piensoNumero.c $(FLAGS) $(HEADERS)

adivinoNumero: global.c clave.c semaforo.c funciones.c archivos.c memoria.c adivinoNumero.c
	$(CC) -o adivinoNumero global.c clave.c semaforo.c funciones.c archivos.c memoria.c adivinoNumero.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o piensoNumero adivinoNumero *.dat sincro.dat

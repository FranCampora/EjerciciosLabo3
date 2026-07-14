CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean adivinoNumero piensoNumero

adivinoNumero: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c adivinoNumero.c
	$(CC) -o adivinoNumero global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c adivinoNumero.c $(FLAGS) $(HEADERS)

piensoNumero: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c piensoNumero.c
	$(CC) -o piensoNumero global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c piensoNumero.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o adivinoNumero piensoNumero *.dat sincro.dat

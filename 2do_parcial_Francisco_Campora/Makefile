CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean entidadFinanciera usuarios

entidadFinanciera: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c entidadFinanciera.c
	$(CC) -o entidadFinanciera global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c entidadFinanciera.c $(FLAGS) $(HEADERS)

usuarios: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c usuarios.c
	$(CC) -o usuarios global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c usuarios.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o entidadFinanciera usuarios *.dat sincro.dat

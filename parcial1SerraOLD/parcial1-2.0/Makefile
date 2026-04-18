CC=gcc-4.1

CFLAGS= -Wall -O3 -std=gnu89 -pedantic-errors
HEADERS= -I./
CFILES= archivos.c clave.c semaforo.c funciones.c

all: clean main1 main2

main1: main1.c $(CFILES)
	$(CC) -o banco main1.c $(CFILES) $(CFLAGS) $(HEADERS)

main2: main2.c $(CFILES)
	$(CC) -o cajero main2.c $(CFILES) $(CFLAGS) $(HEADERS)

clean: 
	rm -rf *.o *.dat banco cajero



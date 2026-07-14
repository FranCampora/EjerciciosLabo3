CC=gcc-4.1

CFLAGS= -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS= -I./
CFILES= funciones.c global.c archivos.c clave.c semaforo.c memoria.c colamsg.c thread.c

NAME1= mosquitos
NAME2= pista

all: clean main1 main2

main1: $(NAME1).c $(CFILES)
	$(CC) -o $(NAME1) $(NAME1).c $(CFILES) $(CFLAGS) $(HEADERS)

main2: $(NAME2).c $(CFILES)
	$(CC) -o $(NAME2) $(NAME2).c $(CFILES) $(CFLAGS) $(HEADERS)

clean: 
	rm -rf *.o *.dat $(NAME1) $(NAME2)



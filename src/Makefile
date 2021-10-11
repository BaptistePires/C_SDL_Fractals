CC  = gcc
OBJS = src/main.c
CFLAGS= -w -O3 
LIBS= -lSDL2 -lpthread
OBJ_NAME = Fractals

Fractals: main.o fractals.o guiMenu.o utils.o
	$(CC) $(CFLAGS) -o $(OBJ_NAME) main.o fractals.o guiMenu.o utils.o $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

fractals.o: fractals.h fractals.c
	$(CC) $(CFLAGS) -c fractals.c

guiMenu.o: guiMenu.h guiMenu.c
	$(CC) $(CFLAGS) -c guiMenu.c

utils.o: utils.h utils.c
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm *.o $(OBJ_NAME)

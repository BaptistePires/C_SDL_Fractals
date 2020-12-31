CC  = gcc
OBJS = src/main.c

DEPS = src/*.h
COMPILER_FLAGS = -w -lpthread -O3 

LINKER_FLAGS = -lSDL2

OBJ_NAME = Fractals

%.o: src/%.c %(DEPS):
	$(CC) -c -o $@ $< 

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
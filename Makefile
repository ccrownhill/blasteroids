CC = gcc
CLFAGS = -Wall
LIBS = -lm -lpthread -lallegro -lallegro_primitives -lallegro_font
SRC = blasteroids.c asteroid.c blast.c spaceship.c
OBJ = ${SRC:.c=.o}

all: blasteroids

blasteroids.o: blasteroids.c blasteroids.h spaceship.h asteroid.h
spaceship.o: spaceship.c blasteroids.h spaceship.h asteroid.h
asteroid.o: asteroid.c blasteroids.h asteroid.h
%.o: %.c
	${CC} -c ${CFLAGS} $<

blasteroids: ${OBJ} spaceship.h
	${CC} -o $@ ${OBJ} ${LIBS}

clean:
	rm -f blasteroids ${OBJ}

.PHONY: all clean

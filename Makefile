CC = gcc
CLFAGS = -Wall
INCS = -I.
LIBS = -lm -lallegro -lallegro_primitives -lallegro_font
SRC = blasteroids.c asteroid.c blast.c spaceship.c
OBJ = ${SRC:.c=.o}

all: blasteroids

%.o: %.c
	${CC} -c ${INCS} ${CFLAGS} $<

blasteroids: ${OBJ}
	${CC} -o $@ ${OBJ} ${LIBS}

clean:
	rm -f blasteroids ${OBJ}

.PHONY: all clean

CC=cc

SRC=examples/main.c

OBJ=$(SRC:.c=.o)

TARGET=program

CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99

INC=-I.

LIBS=-L.

.PHONY: test

${TARGET}: ${OBJ}
	${CC} ${SRC} -o ${TARGET} ${CFLAGS} ${INC} ${LIBS}

%.o: %.c
	${CC} -c ${CFLAGS} $< -o $@ ${INC}

test: ${TARGET}
	@./${TARGET}

clean:
	rm -vf ${TARGET} ${OBJ}

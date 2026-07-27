CC=cc

MAIN_SRC=examples/main.c
MINIMAL_SRC=examples/minimal.c

MAIN_OBJ=$(MAIN_SRC:.c=.o)
MINIMAL_OBJ=$(MINIMAL_SRC:.c=.o)

MAIN_TARGET=main-test
MINIMAL_TARGET=minimal-test

CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99

INC=-I.

LIBS=-L. -lX11 -lXrandr

.PHONY: clean test test-main test-minimal

all: ${MAIN_TARGET} ${MINIMAL_TARGET}

${MAIN_TARGET}: ${MAIN_OBJ}
	${CC} ${MAIN_OBJ} -o ${MAIN_TARGET} ${CFLAGS} ${INC} ${LIBS}

${MINIMAL_TARGET}: ${MINIMAL_OBJ}
	${CC} ${MINIMAL_OBJ} -o ${MINIMAL_TARGET} ${CFLAGS} ${INC} ${LIBS}

%.o: %.c
	${CC} -c ${CFLAGS} $< -o $@ ${INC}

test-main: ${MAIN_TARGET}
	./${MAIN_TARGET}

test-minimal: ${MINIMAL_TARGET}
	./${MINIMAL_TARGET}

test: test-main test-minimal

clean:
	rm -vf ${MAIN_TARGET} ${MINIMAL_TARGET} ${MAIN_OBJ} ${MINIMAL_OBJ}

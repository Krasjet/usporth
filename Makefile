include config.mk

CC = cc
CFLAGS = -std=c99 -Os -Wall -Wextra -pedantic -Wno-unused-parameter -I.
LDLIBS = -lm

BIN = \
	usporth_jack

OBJ = \
	usporth.o \
	util.o \
	ugens/basic.o \
	ugens/sine.o
OBJ_JACK = ${OBJ} main_jack.o

all: ${BIN}

CFLAGS.usporth_jack = ${JACK_CFLAGS}
LDLIBS.usporth_jack = ${JACK_LDLIBS}
usporth_jack: ${OBJ_JACK}
	${CC} ${CFLAGS} ${CFLAGS.$@} -o $@ ${OBJ_JACK} ${LDLIBS} ${LDLIBS.$@}

usporth.o: util.h ugens.h usporth.h
util.o: util.h

CFLAGS.main_jack.o = ${JACK_CFLAGS}
main_jack.o: usporth.h util.h

.c.o:
	${CC} ${CFLAGS} ${CFLAGS.$@} -c -o $@ $<

clean:
	rm -f ${BIN} *.o ugens/*.o *.exe

.PHONY: clean

include config.mk

CC = cc
CFLAGS = -std=c99 -Os -Wall -Wextra -pedantic -Wno-unused-parameter -I.
LDLIBS = -lm

BINS = \
	usporth_text \
	${JACK_BINS}

OBJ = \
	usporth.o \
	util.o \
	ugens/basic.o \
	ugens/sine.o \
	ugens/in.o \
	ugens/scale.o \
	ugens/biscale.o \
	ugens/metro.o
OBJ_JACK = ${OBJ} main_jack.o
OBJ_TEXT = ${OBJ} main_text.o

all: ${BINS}

CFLAGS.usporth_jack = ${JACK_CFLAGS}
LDLIBS.usporth_jack = ${JACK_LDLIBS}
usporth_jack: ${OBJ_JACK}
	${CC} ${CFLAGS} ${CFLAGS.$@} -o $@ ${OBJ_JACK} ${LDLIBS} ${LDLIBS.$@}

usporth_text: ${OBJ_TEXT}
	${CC} ${CFLAGS} ${CFLAGS.$@} -o $@ ${OBJ_TEXT} ${LDLIBS} ${LDLIBS.$@}

usporth.o: util.h ugens.h usporth.h
util.o: util.h
ugens/basic.o: usporth.h
ugens/sine.o: usporth.h
ugens/in.o: usporth.h
ugens/scale.o: usporth.h
ugens/biscale.o: usporth.h
ugens/metro.o: usporth.h

CFLAGS.main_jack.o = ${JACK_CFLAGS}
main_jack.o: usporth.h util.h
main_text.o: usporth.h util.h

ugens.h: ugens.lua
	-./ugens.lua > ugens.h

.c.o:
	${CC} ${CFLAGS} ${CFLAGS.$@} -c -o $@ $<

clean:
	rm -f ${BINS} *.o ugens/*.o *.exe

.PHONY: clean

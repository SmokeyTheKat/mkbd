PREFIX=~/.local

CC=g++
CFLAGS=-Wno-sign-compare -Wno-narrowing -g -I./include/ -D__LINUX_ALSA__ -lasound -lpthread -lSDL2 -lSDL2_ttf
TARGET=./mkbd
SRCDIR=src

CHDRS=$(shell find ./include/ -name '*.hpp' -type f)
CSRCS=$(shell find ./src/ -name '*.cpp' -or -name '*.c' -type f)
AOBJS=$(CSRCS:.cpp=.o)
OBJS=$(AOBJS:.c=.o)

all: ${TARGET}

install: all
	cp ${TARGET} ${PREFIX}/bin/

${TARGET}: ${OBJS}
	$(CC) -o ${TARGET} ${OBJS} ${CFLAGS}

%.o: %.cpp
	$(CC) -c ${CFLAGS} -o $@ $<
%.o: %.c
	$(CC) -c ${CFLAGS} -o $@ $<

clean:
	find ./ -type f -name *.o -delete
	rm -rf ${TARGET}

tc: all
	$(TARGET)

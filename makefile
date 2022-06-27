PREFIX=~/.local

RESOURCE_DIR=$(realpath ./resources)

CC=g++
CFLAGS=-std=c++17 -Wno-narrowing -g -I./include/ -D__LINUX_ALSA__ -DRESOURCE_DIR=\"${RESOURCE_DIR}\" -lasound -lpthread -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_gfx -lGL

TARGET=./mkbd

CSRCS=$(shell find ./src/ -name '*.cpp' -or -name '*.c' -type f)
AOBJS=$(CSRCS:.cpp=.o)
OBJS=$(AOBJS:.c=.o)

all: ${TARGET}

install: ${TARGET}
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

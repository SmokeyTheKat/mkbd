PREFIX=~/.local

CC=g++
#CFLAGS=-Wall -Werror -Wno-sign-compare -I./include/ -I/home/william/Programs/VSSynth/include/ -D__LINUX_ALSA__ `pkg-config gtkmm-3.0 --cflags --libs` -lasound -lpthread -llodepng -g -lSDL2
CFLAGS=-Wno-sign-compare -I./include/ -I/home/william/Programs/VSSynth/include/ -D__LINUX_ALSA__ `pkg-config gtkmm-3.0 --cflags --libs` -lasound -lpthread -llodepng -g -lSDL2
TARGET=./mkbd
BUILDDIR=build
SRCDIR=src

CSRCS=$(shell find ./src/ -name '*.cpp' -printf "%f\n")
OBJS=$(addprefix ${BUILDDIR}/, $(CSRCS:.cpp=.o))

all: ${TARGET}

install: all
	cp ${TARGET} ${PREFIX}/bin/

${TARGET}: ${BUILDDIR} ${OBJS}
	$(CC) -o ${TARGET} ${OBJS} ${CFLAGS}

${BUILDDIR}:
	mkdir -p ${BUILDDIR}

${BUILDDIR}/%.o: ${SRCDIR}/%.cpp
	$(CC) -c ${CFLAGS} -o $@ $<


clean:
	rm -rf ${BUILDDIR}
	rm -rf ${TARGET}

tc: all
	$(TARGET)

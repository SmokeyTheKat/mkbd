PREFIX=~/.local

RESOURCE_DIR=$(realpath ./resources)

CFLAGS=--std=c++17 -std=c++17 -Wno-narrowing -g -I./include/ -DRESOURCE_DIR=\"${RESOURCE_DIR}\"

linux: CC=g++
linux: PKG_CONFIG=pkg-config
linux: OS_CFLAGS=-D__LINUX_ALSA__ -lpthread -lasound -lpthread -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_gfx -lGL -Llib/linux -Wl,-rpath=lib/linux -ldrawtext -lfreetype

windows: CC=x86_64-w64-mingw32.static-gcc
windows: PKG_CONFIG=x86_64-w64-mingw32-pkg-config
windows: OS_CFLAGS=-I/usr/x86_64-w64-mingw32/include -L/usr/x86_64-w64-mingw32/lib -D__WINDOWS_MM__ -l:libstdc++.a -lstdc++fs -lpthread -l:libSDL2.a -l:libSDL2_ttf.a -l:libSDL2_image.a -lopengl32 -l:libfreetype.a -lwinmm -lmingw32 -lSDL2main -lSDL2 -mwindows -Dmain=SDL_main -lwinspool -lcomctl32 -lcomdlg32 -limm32 -lshell32 -Wl,-luuid -ldwmapi -lsetupapi -lcfgmgr32 -lpangocairo-1.0 -lpangoft2-1.0 -lpangowin32-1.0 -lpango-1.0 -lcairo-gobject -lgdk_pixbuf-2.0 -lgio-2.0 -ldnsapi -liphlpapi -lgmodule-2.0 -latk-1.0 -lepoxy -lharfbuzz -lcairo -lgobject-2.0 -lfontconfig -lfreetype -lm -lusp10 -lmsimg32 -lgdi32 -lpixman-1 -lexpat -lbz2 -ltiff -lpng16 -lwebp -llzma -ljpeg -lz -lffi -lws2_32 -lole32 -lwinmm -lshlwapi -lpcre -lintl -liconv -L/usr/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -mwindows -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -lfreetype -lharfbuzz -lfreetype -lharfbuzz -lgraphite2 -lusp10 -lrpcrt4 -lbrotlidec-static -lbrotlienc-static -lbrotlicommon-static


CC=g++

TARGET=./mkbd

CSRCS=$(shell find ./src/ -name '*.cpp' -or -name '*.c' -type f)
AOBJS=$(CSRCS:.cpp=.o)
OBJS=$(AOBJS:.c=.o)

all: linux

linux: ${TARGET}

windows: ${TARGET}

#all: linux ${TARGET}

install: ${TARGET}
	cp ${TARGET} ${PREFIX}/bin/

${TARGET}: ${OBJS}
	$(CC) -o ${TARGET} ${OBJS} ${CFLAGS} ${OS_CFLAGS}

%.o: %.cpp
	$(CC) -c ${CFLAGS} ${OS_CFLAGS} -o $@ $<
%.o: %.c
	$(CC) -c ${CFLAGS} ${OS_CFLAGS} -o $@ $<

clean:
	find ./ -type f -name *.o -delete
	rm -rf ${TARGET}

tc: all
	$(TARGET)

# Makefile for the mv driver (based on that of hello).

COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	game_manager.c main.c vbe.c video_gr.c sprites.c devices.c fighter.c timer.c speaker.c mouse.c keyboard.c rtc.c ser.c utils.c ms.s

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-lvt -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

BINDIR?= /home/bin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
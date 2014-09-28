#
# Adaptado de:
# Internetworking with TCP/IP, Volume III example code Makefile
#
#	David L Stevens, Internetworking Research Group at Purdue
#	Tue Sep 17 19:40:42 EST 1991
#

INCLUDE =

CLNTS = TCPdaytime UDPdaytime TCPecho UDPecho
SERVS = daytimed echod-select

DEFS =
CFLAGS = -W -pedantic -ansi -g ${DEFS} ${INCLUDE}

HDR =
CSRC =  TCPdaytime.c UDPdaytime.c TCPecho.c UDPecho.c
CXSRC = socket_utils.c error.c

SSRC =   daytimed.c echod-select.c

CXOBJ = socket_utils.o error.o
SXOBJ = socket_utils.o error.o

PROGS = ${CLNTS} ${SERVS}  

all: ${PROGS}

${CLNTS}: ${CXOBJ}
	${CC} -o $@ ${CFLAGS} $@.o ${CXOBJ}

${SERVS}: ${SXOBJ}
	${CC} -o $@ ${CFLAGS} $@.o ${SXOBJ}

clients: ${CLNTS}
servers: ${SERVS}

clean: 
	rm -f Makefile.bak a.out core ${PROGS} *.o *~

${HDR} ${CSRC} ${CXSRC} ${SSRC} ${SXSRC}:
	co $@

TCPdaytime: TCPdaytime.o
UDPdaytime: UDPdaytime.o
daytimed: daytimed.o
UDPecho: UDPecho.o
TCPecho: TCPecho.o
echod-select: echod-select.o



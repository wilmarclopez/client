#
# File          : Makefile
# Description   : Build file for CSE473 project 2


# Environment Setup
LIBDIRS=-L. 
INCLUDES=-I.
CC=gcc 
CFLAGS=-c $(INCLUDES) -g -Wall
LINK=gcc -g
LDFLAGS=$(LIBDIRS)
AR=ar rc
RANLIB=ranlib

# Suffix rules
.c.o :
	${CC} ${CFLAGS} $< -o $@

#
# Setup builds

PT-TARGETS=cse473-p2 \
	cse473-pt-server 
CSE473LIB=cse473
CSE473LIBOBJS= cse473-proto.o
UTILLIB=cse473-util
UTILOBJS=	cse473-util.o \
		cse473-network.o \

# pthreads
LIBS=-l$(CSE473LIB) -l$(UTILLIB) -lpthread

#
# Project Protections

p2-pthread : $(PT-TARGETS)

cse473-p2 : cse473-p2.o cse473-client.o lib$(CSE473LIB).a lib$(UTILLIB).a
	$(LINK) $(LDFLAGS) cse473-p2.o cse473-client.o $(LIBS) -o $@

cse473-pt-server : cse473-p2.o lib$(CSE473LIB).a lib$(UTILLIB).a
	${CC} ${CFLAGS} cse473-p2.c -DTHREAD_SERVER -o cse473-p2-server.o
	${CC} ${CFLAGS} cse473-server.c -DTHREAD_SERVER -o cse473-server.o
	${CC} ${CFLAGS} cse473-pthread.c -o cse473-pthread.o
	$(LINK) $(LDFLAGS) cse473-p2-server.o cse473-server.o cse473-pthread.o $(LIBS) -o $@

lib$(CSE473LIB).a : $(CSE473LIBOBJS)
	$(AR) $@ $(CSE473LIBOBJS)
	$(RANLIB) $@

lib$(UTILLIB).a : $(UTILOBJS)
	$(AR) $@ $(UTILOBJS)
	$(RANLIB) $@

clean:
	rm -f *.o *~ $(TARGETS) $(LIBOBJS) lib$(CSE473LIB).a lib$(UTILLIB).a

BASENAME=p2-threads
tar: 
	tar cvfz $(BASENAME).tgz -C ..\
	    $(BASENAME)/Makefile \
	    $(BASENAME)/cse473-p2.c \
	    $(BASENAME)/cse473-network.c \
	    $(BASENAME)/cse473-util.c \
	    $(BASENAME)/cse473-proto.c \
	    $(BASENAME)/cse473-network.h \
	    $(BASENAME)/cse473-proto.h \
	    $(BASENAME)/cse473-util.h \
	    $(BASENAME)/cse473-client.c \
	    $(BASENAME)/cse473-server.h \
	    $(BASENAME)/cse473-server.c \
	    $(BASENAME)/cse473-pthread.h \
	    $(BASENAME)/cse473-pthread.c 


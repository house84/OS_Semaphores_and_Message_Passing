CC = gcc
CFLAGS = -g
LDLIBS = -lm -lpthread -lrt
LDFLAGS =
LIBPATH = -L . -l 

TARGETLIB = libmonitor.a
LIBOBJ = monitorfunc.o 
LIBC = monitorfunc.c

TARGETLIB2 = semHandling.a
LIBOBJ2 = semHandling.o
LIBC2 = semHandling.c

HEADERS = monitor.h shared.h

TARGET1 = monitor
OBJ1 = monitor.o

TARGET2 = consumer
OBJ2 = consumer.o

TARGET3 = producer
OBJ3 = producer.o

.SUFFIXES: .c .o

ALL: $(TARGETLIB) $(TARGETLIB2) $(TARGET1) $(TARGET2) $(TARGET3)

$(LIBOBJ): $(LIBC)
					$(CC) -c $(LIBC)

$(LIBOBJ2): $(LIBC2)
					$(CC) -c $(LIBC2)

$(TARGET1): $(OBJ1)
					$(CC) $(CFLAGS) -o $(TARGET1) $(OBJ1) $(TARGETLIB) $(LDLIBS)

$(TARGET2): $(OBJ2)
					$(CC) $(CFLAGS) -o $(TARGET2) $(OBJ2) $(TARGETLIB2) $(LDLIBS)

$(TARGET3): $(OBJ3)
					$(CC) $(CFLAGS) -o $(TARGET3) $(OBJ3) $(TARGETLIB2) $(LDLIBS)

$(TARGETLIB): $(LIBOBJ) $(HEADERS)
					ar rs $@ $^ 

$(TARGETLIB2): $(LIBOBJ2) 
					ar rs $@ $^
				
clean: 
				rm -f $(TARGET1) $(TARGET2) $(TARGET3) $(TARGETLIB) $(TARGETLIB2) *.o 

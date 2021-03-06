CC = gcc
CFLAGS = -g
LDFLAGS =
LIBPATH = -L . -l 

TARGETLIB = libmonitor.a
LIBOBJ = monitorfunc.o 
LIBC = monitorfunc.c

HEADERS = monitor.h shared.h

TARGET1 = monitor
OBJ1 = monitor.o

TARGET2 = consumer
OBJ2 = consumer.o

TARGET3 = producer
OBJ3 = producer.o

.SUFFIXES: .c .o

ALL: $(TARGETLIB) $(TARGET1) $(TARGET2) $(TARGET3)

$(LIBOBJ): $(LIBC)
					$(CC) -c $(LIBC)

$(TARGET1): $(OBJ1)
					$(CC) $(CFLAGS) -o $(TARGET1) $(OBJ1) $(TARGETLIB)

$(TARGET2): $(OBJ2)
					$(CC) $(CFLAGS) -o $(TARGET2) $(OBJ2)

$(TARGET3): $(OBJ3)
					$(CC) $(CFLAGS) -o $(TARGET3) $(OBJ3)

$(TARGETLIB): $(LIBOBJ) $(HEADERS)
					ar rs $@ $^ 
				
clean: 
				rm -f $(TARGET1) $(TARGET2) $(TARGET3) $(TARGETLIB) *.o 


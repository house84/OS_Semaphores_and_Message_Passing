CC = gcc
CFLAGS = -g
LDLIBS =

TARGET1 = monitor
OBJ1 = monitor.o

TARGET2 = consumer
OBJ2 = consumer.o

TARGET3 = producer
OBJ3 = producer.o

.SUFFIXES: .c .o

ALL: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(OBJ1)
					$(CC) $(CFLAGS) -o $(TARGET1) $(OBJ1) $(LDLIBS)

$(TARGET2): $(OBJ2)
					$(CC) $(CFLAGS) -o $(TARGET2) $(OBJ2)

$(TARGET3): $(OBJ3)
					$(CC) $(CFLAGS) -o $(TARGET3) $(OBJ3)

clean: 
				rm -f $(TARGET1) $(TARGET2) $(TARGET3) *.o 


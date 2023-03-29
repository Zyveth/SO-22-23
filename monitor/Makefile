CC = gcc
CFLAGS = -g -Wall -O2

SRC = src/
OBJ = obj/

default: monitor

monitor: monitor.o handler.o
	$(CC) $(CFLAGS) -o monitor $(OBJ)monitor.o $(OBJ)handler.o

monitor.o:
	$(CC) $(CFLAGS) -c $(SRC)monitor.c -o $(OBJ)monitor.o

handler.o:
	$(CC) $(CFLAGS) -c $(SRC)handler.c -o $(OBJ)handler.o 

clean: 
	$(RM) tracer $(OBJ)*.o *~
CC = gcc
CFLAGS = -g -Wall -O2

SRC = src/
OBJ = obj/

default: tracer

tracer: tracer.o system.o util.o status.o readln.o
	$(CC) $(CFLAGS) -o tracer $(OBJ)tracer.o $(OBJ)system.o $(OBJ)status.o $(OBJ)readln.o $(OBJ)util.o

tracer.o:
	$(CC) $(CFLAGS) -c $(SRC)tracer.c -o $(OBJ)tracer.o

system.o:
	$(CC) $(CFLAGS) -c $(SRC)system.c -o $(OBJ)system.o 

status.o:
	$(CC) $(CFLAGS) -c $(SRC)status.c -o $(OBJ)status.o

readln.o:
	$(CC) $(CFLAGS) -c $(SRC)readln.c -o $(OBJ)readln.o

util.o:
	$(CC) $(CFLAGS) -c $(SRC)util.c -o $(OBJ)util.o 

clean: 
	$(RM) tracer $(OBJ)*.o *~
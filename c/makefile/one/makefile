CC=gcc
CFLAGS=-I.
HEADER = main.h
OBJ = main.o factorial.o 

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm -rf *o main

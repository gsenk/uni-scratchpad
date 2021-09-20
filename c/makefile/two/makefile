CC=gcc
CFLAGS=-I.
HEADER = hellomake.h
OBJ = hellomake.o hellofunc.o 

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm -rf *o hellomake

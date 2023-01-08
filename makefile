# Leon Augusto Okida Gonçalves - GRR20190365
CFLAGS = -Wall -g
objects = tests.o util.o

all: escalona

util.o: util.c util.h
	gcc -c util.c $(CFLAGS)

tests.o: tests.c tests.h util.h operation.h
	gcc -c tests.c $(CFLAGS)

escalona: escalona.c $(objects) operation.h
	gcc escalona.c -o escalona $(objects) $(CFLAGS)

purge: clean
	-rm -f escalona
clean:
	-rm -f $(objects)
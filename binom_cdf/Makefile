CC=gcc
CFLAGS=-Wall -Wpedantic

all: binom_gmp binom_gsl

binom_gmp: binom_gmp.c
	$(CC) $(CFLAGS) -o binom_gmp binom_gmp.c -lgmp

binom_gsl: binom_gsl.c
	$(CC) $(CFLAGS) -o binom_gsl binom_gsl.c -lgsl -lgslcblas

clean:
	rm binom_gmp binom_gsl

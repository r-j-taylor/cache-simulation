#   				\
	Ryan Taylor		\
	11-08-2019


CC=gcc
CFLAGS=-g -Wall
FILES=Simulation.c
DERIV=${FILES:.c=.o}
DEPEND=$(DERIV)

all: Simulation

Simulation: $(DEPEND)
	$(CC) -o Simulation $(CFLAGS) $(DERIV)

clean:
	rm -f $(DERIV) Simulation core*
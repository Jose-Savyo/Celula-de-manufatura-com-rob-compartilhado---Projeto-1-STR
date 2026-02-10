CC = gcc
CFLAGS = -Wall -pthread

all: celula_manufatura

celula_manufatura: celula_manufatura.c
	$(CC) $(CFLAGS) -o celula_manufatura celula_manufatura.c

clean:
	rm -f celula_manufatura
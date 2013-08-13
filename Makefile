CFLAGS=-Wall -g -std=c99 -o
CC=gcc
FILE=ex17
DEPENDENCIES=main.c Database/functions.c
DBFILE=dbfile

$(FILE): $(DEPENDENCIES)
	$(CC) $(CFLAGS) $(FILE) $(DEPENDENCIES)

test:
	./ex17 $(DBFILE) c 100 100
	./ex17 $(DBFILE) s 1 zed zed@zedshaw.com
	./ex17 $(DBFILE) s 2 frank frank@zedshaw.com
	./ex17 $(DBFILE) s 3 joe joe@zedshaw.com

clean:
	rm -rf $(FILE)
	rm -rf $(DBFILE)

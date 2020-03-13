all:server client
.PHONY : all

CC:=gcc
CFLAGS:=-g -Wall -lpthread
LAB:=-lmysqlclient

client: client.o cliaddr.o sock.o list.o file.o
	$(CC) $^ $(CFLAGS) -o $@
server: list.o epoll.o sock.o server.o mysql.o file.o deamon.o
	$(CC) $^ $(CFLAGS) $(LAB) -o $@
.PHONY:clean
clean:
	rm *.o server client


all: client server

hw.h: hw.x
	rpcgen hw.x
	mv hw_clnt.c out/hw_clnt.c
	mv hw_svc.c out/hw_svc.c
	mv hw.h out/hw.h
	
hw_svc.o hw_clnt.o: hw.h
	gcc -c out/hw_svc.c -o out/hw_svc.o
	gcc -c out/hw_clnt.c -o out/hw_clnt.o

hw_client.o: hw.h
	gcc -c src/hw_client.c -o out/hw_client.o

hw_server.o: hw.h
	gcc -c src/hw_server.c -o out/hw_server.o

utils.o:
	gcc -c src/utils/utils.c -o out/utils.o

client: hw_client.o hw_clnt.o utils.o
	gcc -o out/client out/hw_client.o out/hw_clnt.o out/utils.o -lnsl -lpthread

server: hw_server.o hw_svc.o utils.o
	gcc -o out/server out/hw_server.o out/hw_svc.o -lnsl

.PHONY: clean

clean:
	-rm out/*

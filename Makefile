CC = gcc
CFLAGS = -Wall

all:
	$(CC) $(CFLAGS) tftp_client.c tftp.c -o tftp_client
	$(CC) $(CFLAGS) tftp_server.c tftp.c -o tftp_server

clean:
	rm -f tftp_client tftp_server

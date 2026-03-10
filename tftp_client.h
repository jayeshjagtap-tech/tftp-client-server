#ifndef TFTP_CLIENT_H
#define TFTP_CLIENT_H

#include <arpa/inet.h>     // For sockaddr_in structure

/* Client structure to store socket and server info */
typedef struct {
    int sockfd;                         // UDP socket file descriptor
    struct sockaddr_in server_addr;     // Server IP address and port
    socklen_t server_len;               // Length of server address
} tftp_client_t;

/* Establish connection with TFTP server */
void connect_to_server(tftp_client_t *client, char *ip, int port);

/* Upload file to server (WRQ) */
void put_file(tftp_client_t *client, char *filename);

/* Download file from server (RRQ) */
void get_file(tftp_client_t *client, char *filename);

/* Close socket and terminate client */
void disconnect(tftp_client_t *client);

/* Parse and execute user commands */
void process_command(tftp_client_t *client, char *command);

/* Create and send RRQ or WRQ packet */
void send_request(int sockfd,
                  struct sockaddr_in server_addr,
                  char *filename,
                  int opcode);

/* Send request and handle response */
void receive_request(int sockfd,
                     struct sockaddr_in server_addr,
                     char *filename,
                     int opcode);

#endif

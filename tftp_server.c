#include <stdio.h>          // Standard I/O functions
#include <string.h>         // String handling
#include <unistd.h>         // close()
#include <arpa/inet.h>      // Socket related functions
#include <sys/time.h>       // struct timeval for timeout
#include "tftp.h"           // Common TFTP definitions

/* Handle incoming RRQ or WRQ request */
void handle_client(int sockfd,
                   struct sockaddr_in client_addr,
                   socklen_t client_len,
                   tftp_packet *packet);

int main() {
    int sockfd;                             // Server socket
    struct sockaddr_in server_addr;         // Server address structure
    struct sockaddr_in client_addr;         // Client address structure
    socklen_t client_len = sizeof(client_addr);
    tftp_packet packet;                    // TFTP packet buffer

    /* Create UDP socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Configure socket receive timeout */
    struct timeval tv = { TIMEOUT_SEC, 0 };
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,
               &tv, sizeof(tv));

    /* Initialize server address */
    server_addr.sin_family = AF_INET;       // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept from any interface
    server_addr.sin_port = htons(PORT);     // Bind to TFTP port

    /* Bind socket to server address */
    bind(sockfd, (struct sockaddr *)&server_addr,
         sizeof(server_addr));

    printf("TFTP Server listening on port %d\n", PORT);

    /* Server runs continuously */
    while (1) {
        /* Receive RRQ or WRQ from client */
        if (recvfrom(sockfd, &packet, BUFFER_SIZE, 0,
                     (struct sockaddr *)&client_addr,
                     &client_len) < 0)
            continue;                       // Ignore timeout/errors

        /* Process client request */
        handle_client(sockfd, client_addr, client_len, &packet);
    }

    close(sockfd);                          // Close socket
    return 0;
}

/* Check opcode and call appropriate function */
void handle_client(int sockfd,
                   struct sockaddr_in client_addr,
                   socklen_t client_len,
                   tftp_packet *packet) {

    int opcode = ntohs(packet->opcode);     // Convert opcode to host order

    if (opcode == RRQ) {
        /* Client wants to read file */
        send_file(sockfd,
                  client_addr,
                  client_len,
                  packet->body.request.filename);
    }
    else if (opcode == WRQ) {
        /* Client wants to write file */
        receive_file(sockfd,
                     client_addr,
                     client_len,
                     packet->body.request.filename);
    }
}

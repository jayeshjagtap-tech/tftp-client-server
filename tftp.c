#include <stdio.h>      // File operations
#include <string.h>     // Memory functions
#include <unistd.h>     // close()
#include "tftp.h"       // TFTP packet definitions

/* Send file data to client (RRQ handling) */
void send_file(int sockfd,
               struct sockaddr_in client_addr,
               socklen_t client_len,
               char *filename) {

    FILE *fp = fopen(filename, "rb");           // Open file in binary mode
    if (!fp) return;                            // Exit if file not found

    tftp_packet packet;                         // Packet buffer
    int block = 1;                              // TFTP block number
    int bytes;                                  // Bytes read from file

    /* Read file and send in 512-byte chunks */
    while ((bytes = fread(packet.body.data_packet.data,
                           1, 512, fp)) > 0) {

        packet.opcode = htons(DATA);            // Set DATA opcode
        packet.body.data_packet.block_number =
            htons(block);                       // Set block number

        /* Send DATA packet to client */
        sendto(sockfd, &packet, bytes + 4, 0,
               (struct sockaddr *)&client_addr,
               client_len);

        /* Wait for ACK from client */
        recvfrom(sockfd, &packet, BUFFER_SIZE, 0,
                 (struct sockaddr *)&client_addr,
                 &client_len);

        block++;                                // Move to next block
    }
    fclose(fp);                                 // Close file
}

/* Receive file data from client (WRQ handling) */
void receive_file(int sockfd,
                  struct sockaddr_in client_addr,
                  socklen_t client_len,
                  char *filename) {

    FILE *fp = fopen(filename, "wb");           // Create file to write
    tftp_packet packet;                         // Packet buffer
    int block = 1;                              // Expected block number

    while (1) {
        /* Receive DATA packet from client */
        int n = recvfrom(sockfd, &packet,
                         BUFFER_SIZE, 0,
                         (struct sockaddr *)&client_addr,
                         &client_len);

        /* Write received data to file */
        fwrite(packet.body.data_packet.data,
               1, n - 4, fp);                   // Exclude header

        packet.opcode = htons(ACK);             // Prepare ACK
        packet.body.ack_packet.block_number =
            htons(block);                       // ACK block number

        /* Send ACK back to client */
        sendto(sockfd, &packet, 4, 0,
               (struct sockaddr *)&client_addr,
               client_len);

        if (n < BUFFER_SIZE) break;              // Last DATA packet
        block++;                                // Expect next block
    }
    fclose(fp);                                  // Close file
}

/* Common header for both TFTP client and server */

#ifndef TFTP_H
#define TFTP_H

#include <stdint.h>         // For fixed-size integer types
#include <arpa/inet.h>      // For sockaddr_in structure

/* Default TFTP server port */
#define PORT 6969

/* Maximum TFTP packet size:
   2 bytes opcode + 2 bytes block + 512 bytes data */
#define BUFFER_SIZE 516

/* Socket timeout in seconds */
#define TIMEOUT_SEC 5

/* TFTP operation codes as per RFC 1350 */
typedef enum {
    RRQ = 1,    // Read Request (download file)
    WRQ = 2,    // Write Request (upload file)
    DATA = 3,   // Data packet
    ACK = 4,    // Acknowledgment packet
    ERROR = 5   // Error packet
} tftp_opcode;

/* TFTP packet structure */
typedef struct {
    uint16_t opcode;        // Identifies packet type

    union {
        /* Used for RRQ and WRQ */
        struct {
            char filename[256];   // Name of file to transfer
            char mode[8];         // Transfer mode (octet)
        } request;

        /* Used for DATA packets */
        struct {
            uint16_t block_number; // Data block number
            char data[512];        // File data
        } data_packet;

        /* Used for ACK packets */
        struct {
            uint16_t block_number; // Block number being acknowledged
        } ack_packet;

        /* Used for ERROR packets */
        struct {
            uint16_t error_code;   // Error type
            char error_msg[512];   // Error description
        } error_packet;

    } body;
} tftp_packet;

/* Common file transfer functions */
void send_file(int sockfd,
               struct sockaddr_in client_addr,
               socklen_t client_len,
               char *filename);

void receive_file(int sockfd,
                  struct sockaddr_in client_addr,
                  socklen_t client_len,
                  char *filename);

#endif

# TFTP Client–Server Implementation using UDP (C)

This project implements a basic version of the Trivial File Transfer Protocol (TFTP) using UDP socket programming in C.
The goal of the project is to understand how lightweight file transfer protocols work and how reliability can be handled at the application level even when using an unreliable protocol like UDP.

The implementation follows the basic concepts defined in RFC 1350.

---

## Objective

* Implement a simple TFTP Client–Server system
* Understand UDP based communication
* Perform file upload and download
* Implement reliable transfer using ACK packets
* Gain hands-on experience with socket programming in C

---

## Applications

* Network booting (PXE boot)
* Firmware transfer in embedded systems
* Router and switch configuration transfer
* Lightweight file transfer in local networks
* Device initialization in network environments

---

## Resources Used

### Software

* Programming Language: C
* Operating System: Linux
* IDE: VS Code
* Compiler: GCC

### Networking Concepts

* UDP Socket Programming
* Client–Server Architecture
* TFTP Protocol (RFC 1350)

### Important APIs Used

* `socket()` – Creates a communication endpoint
* `sendto()` – Sends UDP packets to the server/client
* `recvfrom()` – Receives UDP packets from the network
* `setsockopt()` – Sets socket options such as timeout

---

## Project Structure

```
tftp-client-server/
│
├── tftp_client.c
├── tftp_client.h
├── tftp_server.c
├── tftp.c
├── tftp.h
├── Makefile
└── README.md
```

---

## Implementation

### Client Side

The client provides a command-line interface where the user can issue commands to interact with the server.

Supported commands:

```
connect <ip>
get <filename>
put <filename>
bye
```

Client responsibilities:

* Create a UDP socket
* Send RRQ (Read Request) or WRQ (Write Request)
* Receive data packets from the server
* Send ACK packets to confirm receipt

---

### Server Side

The server continuously listens for client requests on a UDP port.

Server responsibilities:

* Listen for incoming requests
* Identify request type using opcode
* Handle:

  * RRQ → send file to client
  * WRQ → receive file from client
* Transfer file in 512 byte blocks
* Wait for ACK after each block

---

### File Transfer Logic

* Data is divided into 512-byte blocks
* Each block contains a block number
* Receiver sends ACK for each received block
* Transfer finishes when last block < 512 bytes

This mechanism ensures reliability even though UDP does not guarantee delivery.

---

## How to Compile

Use the provided Makefile.

```
make
```

This generates the executables:

```
tftp_client
tftp_server
```

---

## How to Run

### Start the Server

```
./tftp_server
```

Output:

```
TFTP Server listening on port 6969
```

### Run the Client

Open another terminal and run:

```
./tftp_client
```

Example session:

```
tftp> connect 127.0.0.1
tftp> get test.txt
tftp> put demo.txt
tftp> bye
```

---

## Learnings

Through this project I gained practical experience with:

* UDP socket programming
* Client–server communication
* File transfer protocols
* Packet based communication
* Implementing reliability over UDP

It also improved debugging skills while handling network communication and packet flow.

---

## Challenges Faced

* Handling socket timeouts
* Managing block numbers correctly
* Ensuring proper synchronization between client and server
* Debugging packet transmission issues
* Structuring packets according to TFTP format

---

## Future Scope

* Implement full RFC 1350 error handling
* Add retransmission mechanism for lost packets
* Support multiple clients simultaneously
* Add support for netascii transfer mode
* Improve logging and progress display
* Add security mechanisms for authentication

---

## Conclusion

This project demonstrates the working of the Trivial File Transfer Protocol using UDP sockets.
It highlights how simple protocols can implement reliable file transfer using mechanisms like block numbering and acknowledgments.

The implementation provides a clear understanding of network programming, protocol design, and client–server architecture, which are essential concepts in computer networking.

---

## Author

Jayesh Jagtap

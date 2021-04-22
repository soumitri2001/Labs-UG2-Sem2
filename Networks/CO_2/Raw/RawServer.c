/****************************************************************************************
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: COMPUTER NETWORKS LABORATORY (UG-2, SEM-2)
 * ASSIGNMENT: CO-2 Assignment 3 - RAW SOCKETS - SERVER
 ****************************************************************************************/

/****************************************************************************************
 * COMPILATION: gcc RawServer.c -o RawServer
 * EXECUTION: sudo ./RawServer
 ***************************************************************************************/

/* header files */
#include <arpa/inet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_CLIENTS 5
#define MAX_LEN 1024

const char localhost[10] = "127.0.0.1";
const int PORT = 9090;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    // variable to store the file descriptor returned by socket()
    int server_sockfd;

    // create an end point for communication
    if ((server_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0)
    {
        error("socket() returned error code! ");
    }

    printf("Socket created\n");

    // port to which we will bind the socket
    unsigned short port = PORT;

    // create a sockaddr_in for server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;         // AF_INET implies IPv4
    server_addr.sin_port = htons(port);       // converting host BO to network BO
    server_addr.sin_addr.s_addr = INADDR_ANY; // listen on all interfaces
    bzero(&server_addr.sin_zero, 8);          // set padding to zeroes

    // bind the socket to the port and ip
    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        close(server_sockfd);
        error("bind() returned error code! ");
    }

    printf("Bind successful\n");

    // sockaddr_in struct to receive clients IP and port
    struct sockaddr_in client_addr;
    unsigned int addrlen = sizeof(client_addr);

    // buffer to send and receieve data
    char buffer[MAX_LEN + 1];

    // recive message from client
    ssize_t data_len = recvfrom(server_sockfd, buffer, MAX_LEN, 0,
                                (struct sockaddr *)&client_addr, &addrlen);

    // pointers to headers of received packet
    struct iphdr *ipheader = (struct iphdr *)buffer;
    struct udphdr *udpheader = (struct udphdr *)(buffer + sizeof(ipheader));
    ssize_t header_size = sizeof(struct iphdr) + sizeof(struct udphdr);

    // ip of source and destination obtained from header
    struct in_addr src_addr, dest_addr;
    src_addr.s_addr = ipheader->saddr;
    dest_addr.s_addr = ipheader->daddr;

    // total length of packet
    unsigned short total_length = ntohs(ipheader->tot_len);

    printf("Data from headers: \n");
    printf("Source IP: %s\n", inet_ntoa(src_addr));
    printf("Destination IP: %s\n", inet_ntoa(dest_addr));
    printf("Total length of packet: %d\n", total_length);

    if (data_len <= header_size)
    {
        printf("No message received!\n");
    }
    else
    {
        buffer[data_len] = '\0';
        printf("\n[Message from a client]: %s\n", buffer + header_size);
    }
    // close the file descriptor
    close(server_sockfd);
}
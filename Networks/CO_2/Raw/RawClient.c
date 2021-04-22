/****************************************************************************************
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: COMPUTER NETWORKS LABORATORY (UG-2, SEM-2)
 * ASSIGNMENT: CO-2 Assignment 3 - RAW SOCKETS - CLIENT
 ****************************************************************************************/

/****************************************************************************************
 * COMPILATION: gcc RawClient.c -o RawClient
 * EXECUTION: sudo ./RawClient
 ***************************************************************************************/

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

#define MAX_LEN 1024

const char localhost[10] = "127.0.0.1";
const int DEST_PORT = 9090;
const int SRC_PORT = 9191;

/**
 * ---------------------------------------------------------------------------------
 * Check sum calculation for IP Header:
 * ---------------------------------------------------------------------------------
 * To calculate the IP checksum of a datagram, first set the checksum field to
 * 0. Then perform the binary complement summation  for  each  16-bit in the
 * header (the whole header is composed of a string of 16-bit words), and the
 * result is stored in the checksum field. When an IP datagram is received, the
 * binary complement sum is also performed on each 16-bit in the header. Since
 * the receiver includes the checksum in the header of the sender in the
 * calculation process, if the header does not cause any errors during
 * transmission, the result of the receiver's calculation should be all ones.
 * Otherwise, it means that an error occurs in the data transmission process,
 * and IP will discard the received datagram. However, no error message is
 * generated, and the lost datagram is found by the upper zone and retransmitted
 * ---------------------------------------------------------------------------------
 */

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

unsigned short csum(unsigned short *ptr, int nbytes)
{
    unsigned long sum = 0;
    unsigned short last_odd_byte;
    while (nbytes > 1)
    {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1)
    {
        last_odd_byte = (unsigned short)*((unsigned char *)(ptr));
        sum += last_odd_byte;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum = sum + (sum >> 16);
    return (unsigned short)~sum;
}

int main()
{
    // variable to store file descriptor returned by socket()
    int client_sockfd;

    // create a socket
    if ((client_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0)
    {
        error("socket() returned error code! ");
    }

    printf("Socket created\n");

    // port on which the server we want to send packet to is listening
    unsigned short dest_port = DEST_PORT;

    // sockaddr_in for server we want to connect to
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;        // AF_INET implies IPv4
    server_addr.sin_port = htons(dest_port); // from host BO to network BO
    server_addr.sin_addr.s_addr = inet_addr(localhost);
    bzero(&server_addr.sin_zero, 8); // set padding to zeroes

    // buffer for the packet
    char buffer[MAX_LEN + 1];

    // ip header
    struct iphdr *ipheader = (struct iphdr *)buffer;
    // udp header
    struct udphdr *udpheader = (struct udphdr *)(buffer + sizeof(struct iphdr));
    ssize_t header_size = sizeof(struct iphdr) + sizeof(struct udphdr);

    // data to send
    char *data = (char *)(buffer + header_size);
    strcpy(data, "This is client's message sent using raw socket!");

    // forming the ipheader
    ipheader->ihl = 5;     // header length (IP datagram): 5
    ipheader->version = 4; // IPv4: version 4
    ipheader->tos = 0;     // Type of service: 0 for general TOS
    ipheader->tot_len = header_size + strlen(data);
    ipheader->id = htonl(79135);                   // ID of this packet
    ipheader->frag_off = 0;                        // fragment off-set keeep it zero
    ipheader->ttl = 255;                           // max no. of routers it can pass through
    ipheader->protocol = IPPROTO_UDP;              // porotocol used
    ipheader->check = 0;                           // check sum initially set to zero
    ipheader->saddr = inet_addr(localhost);        // source IP address
    ipheader->daddr = server_addr.sin_addr.s_addr; // destination IP address
    // set the check sum :
    ipheader->check = csum((unsigned short *)buffer, ipheader->tot_len);

    // forming the udpheader
    udpheader->source = htons(SRC_PORT);
    udpheader->dest = htons(DEST_PORT);
    udpheader->len = htons(sizeof(struct udphdr) + strlen(data));

    int one = 1;
    const int *val = &one;

    // set option on socket
    // inform the kernel to not fill up the packet structure
    // as we filled it on our own
    if (setsockopt(client_sockfd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0)
    {
        close(client_sockfd);
        error("setsockopt() returned error code! ");
    }

    printf("Message to be sent: %s\n", buffer + header_size);
    printf("Source IP: %s\n", localhost);
    printf("Source port: %d\n", SRC_PORT);
    printf("Destination IP: %s\n", localhost);
    printf("Destination port: %d\n", DEST_PORT);

    if (sendto(client_sockfd, buffer, ipheader->tot_len, 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        close(client_sockfd);
        error("sendto() returned error code! ");
    }
    printf("Message sent successfully!\n");
    close(client_sockfd);
}
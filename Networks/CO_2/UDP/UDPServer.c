/****************************************************************************************
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: COMPUTER NETWORKS LABORATORY (UG-2, SEM-2)
 * ASSIGNMENT: CO-2 Assignment 2 - DATAGRAM SOCKETS - SERVER
 ****************************************************************************************/

/****************************************************************************************
 * COMPILATION: gcc UDPServer.c -o UDPServer
 * EXECUTION: ./UDPServer
 ***************************************************************************************/

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_LEN 1024

const int PORT = 9000;
const int MAX_CLIENTS = 5;

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
    if ((server_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        error("Server socket cannot be created\n");
    }

    // port to which we will bind the socket
    unsigned short port = PORT;

    // create a sockaddr_in for server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;         // AF_INET implies IPv4
    server_addr.sin_port = htons(port);       // converting host BO to network BO
    server_addr.sin_addr.s_addr = INADDR_ANY; // listen on all interfaces

    bzero(&server_addr.sin_zero, 8); // set padding to zeroes

    // bind the socket to the port and ip
    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        error("Bind failure\n");
    }

    struct sockaddr_in client_addr; // a sockaddr_in for client
    unsigned int addrlen = sizeof(client_addr);

    // buffer to send and receieve data
    char buffer[MAX_LEN];

    printf("Server listening on PORT %d\n\n", PORT);

    while (1)
    {
        printf("[Waiting for client's message]\n");

        // recive message from client
        int data_len = recvfrom(server_sockfd, buffer, MAX_LEN, 0,
                                (struct sockaddr *)&client_addr, &addrlen);
        if (data_len)
        {
            buffer[data_len] = '\0';
            printf("\nClient: %s\n", buffer);

            printf("Server (enter 'EXIT' to exit): ");
            fgets(buffer, MAX_LEN, stdin);

            if (strncmp("EXIT", buffer, 4) == 0)
                break;

            // send message to client
            sendto(server_sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&client_addr, sizeof(client_addr));
        }
        else
        {
            printf("Client disconnected!\n");
        }
    }
    // close the file descriptor
    close(server_sockfd);
}
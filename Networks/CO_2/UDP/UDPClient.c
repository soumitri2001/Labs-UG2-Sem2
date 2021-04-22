/****************************************************************************************
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: COMPUTER NETWORKS LABORATORY (UG-2, SEM-2)
 * ASSIGNMENT: CO-2 Assignment 2 - DATAGRAM SOCKETS - CLIENT
 ****************************************************************************************/

/****************************************************************************************
 * COMPILATION: gcc UDPClient.c -o UDPClient
 * EXECUTION: ./UDPClient
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
const char localhost[10] = "127.0.0.1";

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    // variable to store file descriptor returned by socket()
    int client_sockfd;

    // create a socket
    if ((client_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        error("Client socket cannot be created\n");
    }

    // port on which the server we want to send message to is listening
    unsigned short port = PORT;

    // sockaddr_in for server we want to connect to
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;   // AF_INET implies IPv4
    server_addr.sin_port = htons(port); // converting host BO to network BO
    server_addr.sin_addr.s_addr = inet_addr(localhost);
    bzero(&server_addr.sin_zero, 8); // set padding to zeroes

    // buffer to read and write data
    char buffer[MAX_LEN];

    printf("Server listening on PORT %d\n\n", PORT);

    while (1)
    {
        printf("Client (Enter 'EXIT' to exit): ");
        fgets(buffer, MAX_LEN, stdin);
        if (strncmp("EXIT", buffer, 4) == 0)
        {
            break;
        }
        // send message to server
        sendto(client_sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));

        printf("[Waiting for server to respond]\n");
        // receive message from server
        int len = recvfrom(client_sockfd, buffer, MAX_LEN, 0, NULL, NULL);
        if (len != 0)
        {
            buffer[len] = '\0';
            printf("\nServer: %s\n", buffer);
        }
        else
        {
            break;
        }
    }

    // close the file descriptor
    close(client_sockfd);
}
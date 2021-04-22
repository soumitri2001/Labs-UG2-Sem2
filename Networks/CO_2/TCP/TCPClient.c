/****************************************************************************************
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: COMPUTER NETWORKS LABORATORY (UG-2, SEM-2)
 * ASSIGNMENT: CO-2 Assignment 1 - STREAM SOCKETS - CLIENT
 ****************************************************************************************/

/****************************************************************************************
 * COMPILATION: gcc TCPClient.c -o TCPClient
 * EXECUTION: ./TCPClient
 ***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_LEN 1024

const int PORT = 9999;
const char localhost[10] = "127.0.0.1";

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[MAX_LEN];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Client socket cannot be created\n");

    server = gethostbyname(localhost);

    printf("Server: %s\n", server->h_name);
    if (server == NULL)
    {
        error("Server not found !\n");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);

    serv_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Client connection error\n");

    printf("Client listening on PORT %d\n\n", PORT);

    while (1)
    {
        bzero(buffer, MAX_LEN);

        printf("Client (enter 'EXIT' to exit): ");
        fgets(buffer, MAX_LEN, stdin);

        int flag = write(sockfd, buffer, strlen(buffer));
        if (flag < 0)
            error("Write error\n");

        if (strncmp("EXIT", buffer, 4) == 0)
            break;

        bzero(buffer, MAX_LEN);

        printf("[Waiting for server to respond]\n");

        flag = read(sockfd, buffer, MAX_LEN);
        if (flag < 0)
            error("Read error\n");

        printf("Server: %s\n", buffer);

        if (strncmp("EXIT", buffer, 4) == 0)
            break;
    }
    close(sockfd);
    return 0;
}